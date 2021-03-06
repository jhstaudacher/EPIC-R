#include "HardwareInfo.h"

#if defined(__linux__)
/*
 * LINUX
 */

#include <fstream>		 // read system pseudo-files
#include <sys/sysinfo.h> // sysinfo

/*
 * executes a bash command and reads the output as string
 *
 * @param cmd the bash command to execute
 * @return the stdout generated by the command
 */
std::string execute(std::string cmd) {
	std::string output;

	const int buffer_length = 1 << 8;
	char buffer[buffer_length];

	FILE* stream = popen(cmd.c_str(), "r");

	if (stream) {
		while (!feof(stream)) {
			if (fgets(buffer, buffer_length, stream) != NULL) {
				output.append(buffer);
			}
		}
		pclose(stream);
	}

	return output;
}

/*
 * extract a single double value within a string variable
 *
 * @param txt the string variable containing a double value
 * @return the double value from within the string. If no double value was found the function returns 0.
 */
double doubleInString(std::string txt) {
	double val = 0;

	auto pos = txt.find_first_of("0123456789");
	if (pos != std::string::npos) {
		std::stringstream(txt.substr(pos)) >> val;
	}

	return val;
}

epic::HardwareInfo::HardwareInfo() {
	{ // RAM size
		struct sysinfo info;
		if (sysinfo(&info) == 0) {
			// success
			mRamSizeTotal = (info.totalram * info.mem_unit) / cMemUnit_factor;
			mRamSizeFree = (info.freeram * info.mem_unit) / cMemUnit_factor;
		} else {
			// error
			mRamSizeTotal = 0;
			mRamSizeFree = 0;
		}
	}

	{ // CPU frequency
		const std::string file_name = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq";
		std::ifstream f;
		f.open(file_name);

		if (f.is_open()) {
			f >> mCpuFrequency;
			f.close();
		} else { // read info from lscpu if file file_name is nor present
			double freq = doubleInString(execute("lscpu | grep 'CPU max MHz:'"));

			if (freq == 0) {
				freq = doubleInString(execute("lscpu | grep 'CPU MHz:'"));
			}

			if (freq > 0) {
				mCpuFrequency = static_cast<longUInt>(freq * 1000);
			} else {
				mCpuFrequency = 0;
			}
		}
	}
}

#elif defined(__APPLE__)
/*
 * MacOS
 */

#include <fstream> // read system pseudo-files
#include <sys/sysctl.h>
#include <unistd.h> // sysconf

static double ParseMemValue(const char* b) {
	while ((*b) && (isdigit(*b) == false))
		b++;
	return isdigit(*b) ? atof(b) : -1.0;
}

epic::HardwareInfo::HardwareInfo() {
	// Total RAM
	{
		int mib[] = {CTL_HW, HW_MEMSIZE};
		unsigned long long mem;
		size_t length = sizeof(mem);

		// (name,
		//  length of name,
		//  ptr to save it to,
		//  length of ptr,
		//  NULL and 0 indicate that results should not be stored in separate variables)
		if (sysctl(mib, 2, &mem, &length, NULL, 0) == -1) {
			// error
			mRamSizeTotal = 0;
		} else {
			mRamSizeTotal = mem / cMemUnit_factor;
		}
	}
	// RAM used
	{
		FILE* fpIn = popen("/usr/bin/vm_stat", "r");
		if (!fpIn) {
			// error
			mRamSizeFree = 0;
		} else {
			double pagesUsed = 0.0, totalPages = 0.0;
			char buf[512];
			while (fgets(buf, sizeof(buf), fpIn) != NULL) {
				if (strncmp(buf, "Pages", 5) == 0) {
					double val = ParseMemValue(buf);
					if (val >= 0.0) {
						if ((strncmp(buf, "Pages wired", 11) == 0) || (strncmp(buf, "Pages active", 12) == 0))
							pagesUsed += val;
						totalPages += val;
					}
				} else if (strncmp(buf, "Mach Virtual Memory Statistics", 30) != 0)
					// Stop at "Translation Faults", we don't care about anything at or below that
					break;
			}
			pclose(fpIn);

			// pagesUsed / totalPages => percentage of how much RAM is used
			mRamSizeFree = mRamSizeTotal - static_cast<epic::longUInt>(mRamSizeTotal * (pagesUsed / totalPages));
		}
	}

	// CPU frequency
	{
		int mib[] = {CTL_HW, HW_CPU_FREQ};
		size_t length = sizeof(mCpuFrequency);

		// (name, length of name, ptr to save it to, length of ptr, NULL and 0 indicate that results should not be stored in separate variables)
		if (sysctl(mib, 2, &mCpuFrequency, &length, NULL, 0) == -1) {
			// error
			mCpuFrequency = 0;
		}
	}
}

#elif defined(__WIN32__) || defined(__CYGWIN__)
/*
 * WINDOWS
 */

#include <iostream>
#include <windows.h>

epic::HardwareInfo::HardwareInfo() {
	{ // RAM size
		MEMORYSTATUSEX state = {sizeof(state)};

		if (GlobalMemoryStatusEx(&state)) {
			mRamSizeTotal = state.ullTotalPhys / cMemUnit_factor;
			mRamSizeFree = state.ullAvailPhys / cMemUnit_factor;
		} else {
			mRamSizeTotal = 0;
			mRamSizeFree = 0;
		}
	}

	{ // CPU frequency
		DWORD bufferSize = MAX_PATH;
		DWORD mhz = MAX_PATH;
		HKEY hKey;

		long error = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
								   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
								   0,
								   KEY_READ,
								   &hKey);

		if (error == ERROR_SUCCESS) {
			RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&mhz, &bufferSize);

			mCpuFrequency = mhz * 1000;
		} else {
			mCpuFrequency = 0;
		}
	}
}

#else
/*
 * UNSUPPORTED OS
 */

epic::HardwareInfo::HardwareInfo() {
	mRamSizeTotal = 0;
	mRamSizeFree = 0;
	mCpuFrequency = 0;
}

#endif

epic::longUInt epic::HardwareInfo::getTotalRamSize() const {
	return mRamSizeTotal;
}

epic::longUInt epic::HardwareInfo::getFreeRamSize() const {
	return mRamSizeFree;
}

epic::longUInt epic::HardwareInfo::getCPUFrequency() const {
	return mCpuFrequency;
}
