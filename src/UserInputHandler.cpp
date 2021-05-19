#include "UserInputHandler.h"

#include "Logging.h"
#include "IndexFactory.h"
#include "DataInput.h"

#include <getopt.h>
#include <string>

epic::io::UserInputHandler::UserInputHandler(const std::string& index, const std::vector<longUInt>& weights, longUInt quota, OutputType outputType, bool filterNullPlayers, bool verbose) {
	mIndex = index;
	mWeights = weights;
	mQuota = quota;
	mFloatQuota = 0.0;
	mOutputType = outputType;
	mTestFlag = false;
	mInputFloatWeights = false;
	mFilterNullPlayers = filterNullPlayers;
	mIntRepresentation = DEFAULT;
	mWeightsFile = "";
	
	if (verbose) {
	  log::out.setLogLevel(log::info);
	}
	else {
	  log::out.setLogLevel(log::error);
	}
}

epic::io::UserInputHandler::UserInputHandler() {
	mIndex = "Invalid";
	mWeights.clear();
	mQuota = 0;
	mFloatQuota = 0.0;
	mOutputType = screen;
	mTestFlag = false;
	mInputFloatWeights = false;
	mFilterNullPlayers = false;
	mIntRepresentation = DEFAULT;
	mWeightsFile = "";
}

bool epic::io::UserInputHandler::handleWeightsAndQuota(const std::string& fileName) {
	if (mInputFloatWeights) {
		std::vector<double> floatWeights;
		floatWeights = DataInput::inputFloatCSV(fileName, mTestFlag);
		floatWeights.push_back(mFloatQuota);
		UpscaleFloatToIntAndReturnMultiplicator(floatWeights, mWeights);
		mQuota = mWeights.back();
		mWeights.pop_back();
	} else {
		if (static_cast<longUInt>(mFloatQuota) == mFloatQuota) {
  			mQuota = static_cast<longUInt>(mFloatQuota);
		} else {
			Rcpp::Rcout << "Float quota specified without the --float flag." << std::endl;
			return false;
		}
		mWeights = DataInput::inputCSV(fileName, mTestFlag);
	}

	if (mWeights.empty()) {
		log::out << log::warning << "The --weights option requires a non empty csv file." << log::endl;
		return false;
	}

	return true;
}

bool epic::io::UserInputHandler::handleQuotaFromWeightfile(const std::string& fileName) {
	//detect quota from weight file
	mFloatQuota = DataInput::getQuotaFromCSV(fileName);

	if (mFloatQuota <= 0) {
		log::out << log::warning << "The --quota option requires an argument > 0." << log::endl;
		return false;
	}

	mTestFlag = true;
	return true;
}

bool epic::io::UserInputHandler::handleQuota(char* value) {
	mFloatQuota = std::atof(value);

	if (mFloatQuota > 0) {
		return true;
	} else {
		log::out << log::warning << "The --quota option requires an argument > 0." << log::endl;
		return false;
	}
}

bool epic::io::UserInputHandler::handleIndex(char* value) {
	bool ret = false;

	std::string i_str = std::string(value);
	if (index::IndexFactory::validateIndex(i_str)) {
		mIndex = i_str;
		ret = true;
	} else {
		log::out << log::warning << "The --index option needs one of the following arguments:" << log::endl;
		index::IndexFactory::printIndexList(Rcpp::Rcout);
	}

	return ret;
}

bool epic::io::UserInputHandler::parseCommandLine(int numberOfArguments, char* vectorOfArguments[]) {
	enum LongOpts {
		OPT_GMP,
		OPT_PRIMES,
		OPT_FLOAT,
		OPT_CSV,
		OPT_QUIET
	};

	static struct option long_options[] = {
		{"index", required_argument, nullptr, 'i'},
		{"weights", required_argument, nullptr, 'w'},
		{"quota", required_argument, nullptr, 'q'},
		{"verbose", no_argument, nullptr, 'v'},
		{"filter-null", no_argument, nullptr, 'f'},
		{"gmp", no_argument, nullptr, OPT_GMP},
		{"primes", no_argument, nullptr, OPT_PRIMES},
		{"float", no_argument, nullptr, OPT_FLOAT},
		{"csv", no_argument, nullptr, OPT_CSV},
		{"quiet", no_argument, nullptr, OPT_QUIET},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, 0, nullptr, 0}};
	int arg_count = 0;

	log::out.setLogLevel(log::warning); // default setting

	while (true) {
		int index = -1;
		//struct option * opt = 0;
		int result = getopt_long(numberOfArguments, vectorOfArguments, ":i:w:q:vfmh", long_options, &index);

		if (result == -1) {
			if (arg_count < 3) {
				log::out << log::warning << "Missing required options: -i | --index, -w | --weights, -q | --quota" << log::endl;
				return false;
			}
			break;
		}

		//opt = (struct option *)&(long_options[index]);
		switch (result) {
			case 'i':
				if (!handleIndex(optarg)) {
					return false;
				}
				arg_count++;
				break;

			case 'w':
				mWeightsFile = optarg;
				arg_count++;
				break;

			case 'q':
				if (std::stof(optarg) == 0) {
					//No quota specified -> search in weightfile
					if (!handleQuotaFromWeightfile(mWeightsFile)) {
						return false;
					}
				} else {
					if (!handleQuota(optarg)) {
						return false;
					}
				}
				arg_count++;
				break;

			case 'v':
				if (log::out.getLogLevel() == log::error) {
					log::out << log::warning << "Incompatible options: -v/--verbose and --quiet" << log::endl;
					return false;
				}
				log::out.setLogLevel(log::info);
				break;

			case OPT_QUIET:
				if (log::out.getLogLevel() == log::info) {
					log::out << log::warning << "Incompatible options: -v/--verbose and --quiet" << log::endl;
					return false;
				}
				log::out.setLogLevel(log::error);
				break;

			case 'f':
				mFilterNullPlayers = true;
				break;

			case 'h':
			  Rcpp::Rcout << "epic [OPTIONS]" << std::endl
						  << std::endl;
			  Rcpp::Rcout << "required OPTIONS:" << std::endl
						  << MSG_REQUIRED_OPTS << std::endl;
			  Rcpp::Rcout << "optional OPTIONS:" << std::endl
						  << MSG_OPTIONAL_OPTS << std::endl;
			  Rcpp::Rcout << "Index abbreviations:" << std::endl;
				index::IndexFactory::printIndexList(Rcpp::Rcout);
				Rcpp::stop("");

			case OPT_GMP:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = GMP;
				} else {
					log::out << log::warning << "Incompatible options: --gmp and --primes" << log::endl;
					return false;
				}
				break;

			case OPT_PRIMES:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = PRIMES;
				} else {
					log::out << log::warning << "Incompatible options: --gmp and --primes" << log::endl;
					return false;
				}
				break;

			case OPT_FLOAT:
				mInputFloatWeights = true;
				break;

			case OPT_CSV:
				mOutputType = csv;
				break;

			case '?':
				log::out << log::warning << "Unknown option: " << vectorOfArguments[optind - 1] << log::endl;
				return false;

			case ':':
				log::out << log::warning << "Missing arg for: " << vectorOfArguments[optind - 1] << log::endl;
				return false;

			default:
				return false;
		}
	}

	if (!handleWeightsAndQuota(mWeightsFile)) {
		return false;
	}

	while (optind < numberOfArguments) {
		log::out << log::info << "Other parameter: " << vectorOfArguments[optind++] << log::endl;
	}

	return true;
}

std::vector<epic::longUInt>& epic::io::UserInputHandler::getWeights() {
	return mWeights;
}

epic::longUInt epic::io::UserInputHandler::getQuota() const {
	return mQuota;
}

epic::io::OutputType epic::io::UserInputHandler::getOutputType() const {
	return mOutputType;
}

std::string epic::io::UserInputHandler::getIndexToCompute() const {
	return mIndex;
}

bool epic::io::UserInputHandler::doFilterNullPlayers() const {
	return mFilterNullPlayers;
}

epic::IntRepresentation epic::io::UserInputHandler::getIntRepresentation() const {
	return mIntRepresentation;
}

std::string epic::io::UserInputHandler::getWeightsFileName() const {
	return mWeightsFile;
}
