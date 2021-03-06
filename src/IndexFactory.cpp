#include "IndexFactory.h"

#include "FastUpperBoundApproximation.h"
#include "AbsoluteBanzhaf.h"
#include "AbsolutePublicGood.h"
#include "AbsolutePowerIndexG.h"
#include "Banzhaf.h"
#include "BanzhafOwen.h"
#include "BanzhafBelow.h"
#include "ColemanCollective.h"
#include "ColemanInitiative.h"
#include "ColemanPreventive.h"
#include "DeeganPackel.h"
#include "FelsenthalIndex.h"
#include "HarsanyiNashIndex.h"
#include "Johnston.h"
#include "KoenigBraeuninger.h"
#include "KoenigBraeuningerPH.h"
#include "Nevison.h"
#include "NevisonPH.h"
#include "Owen.h"
#include "OwenExtendedPGI.h"
#include "PowerIndexF.h"
#include "PowerIndexG.h"
#include "PowerIndexGPH.h"
#include "PublicGood.h"
#include "PublicHelpTheta.h"
#include "PublicHelpThetaPH.h"
#include "PublicHelpXi.h"
#include "Rae.h"
#include "RawBanzhaf.h"
#include "RawBanzhafBelow.h"
#include "RawDeeganPackel.h"
#include "RawFelsenthal.h"
#include "RawJohnston.h"
#include "RawPowerIndexF.h"
#include "RawPowerIndexG.h"
#include "RawPublicGood.h"
#include "RawPublicHelpTheta.h"
#include "RawPublicHelpThetaSD.h"
#include "RawShapleyShubik.h"
#include "RawShapleyShubikBelow.h"
#include "ShapleyShubik.h"
#include "ShapleyShubikBelow.h"
#include "Tijs.h"
#include "SingleValueW.h"
#include "SingleValueWM.h"
#include "SingleValueWS.h"
#include "SolidarityPGI.h"
#include "SymmetricCoalitionalBanzhaf.h"
#include "ThreatPGI1.h"
#include "ThreatPGI2.h"
#include "ThreatPGI3.h"
#include "Tijs.h"
#include "UnionPGI.h"

const std::map<epic::index::IndexFactory::IndexType, std::pair<std::string, std::string>> epic::index::IndexFactory::cIndexNames = {
	{INVALID_INDEX, {"INVALID", "invalid index"}},
	{ABZ, {"ABZ", "absolute Banzhaf"}},
	{APG, {"APG", "absolute Public Good"}},
	{APIG, {"APIG", "absolute null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{BZ, {"BZ", "Banzhaf"}},
	{BO, {"BO", "Banzhaf-Owen"}},
	{BZB, {"BZB", "Banzhaf below"}},
	{CC, {"CC", "Coleman collective"}},
	{CI, {"CI", "Coleman initiative"}},
	{CP, {"CP", "Coleman preventive"}},
	{DP, {"DP", "Deegan Packel"}},
	{FT, {"FT", "Felsenthal"}},
	{HN, {"HN", "Harsanyi Nash"}},
	{J, {"J", "Johnston"}},
	{KB, {"KB", "Koenig Braeuninger (based on the Dubey-Shapley identity)"}},
	{KBPH, {"KBPH", "Koenig Braeuninger (based on raw Public Help theta)"}},
	{N, {"N", "Nevison (based on the Dubey-Shapley identity)"}},
	{NPH, {"NPH", "Nevsion (based on raw Public Help theta)"}},
	{O, {"O", "Owen"}},
	{OPGI, {"OPGI", "Owen Extended Public Good Index"}},
	{PG, {"PG", "Public Good"}},
	{PHT, {"PHT", "Public Help theta (based on the Dubey-Shapley identity)"}},
	{PHTPH, {"PHTPH", "Public Help theta (based on the raw Public Help theta)"}},
	{PHX, {"PHX", "Public Help xi"}},
	{PIF, {"PIF", "null-player-free Power Index F"}},
	{PIG, {"PIG", "null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{PIGPH, {"PIGPH", "null-player-free Power Index G (based on raw Public Help theta)"}},
	{RA, {"RA", "Rae"}},
	{RBZ, {"RBZ", "raw Banzhaf"}},
	{RBZB, {"RBZB", "raw Banzhaf from below"}},
	{RDP, {"RDP", "raw Deegan Packel"}},
	{RFT, {"RFT", "raw Felsenthal"}},
	{RJ, {"RJ", "raw Johnston"}},
	{RPG, {"RPG", "raw Public Good"}},
	{RPHT, {"RPHT", "raw Public Help theta"}},
	{RPHTSD, {"RPHTSD", "raw Public Help theta (based on the Dubey-Shapley identity)"}},
	{RPIG, {"RPIG", "raw null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{RPIF, {"RPIF", "raw null-player-free Power Index F"}},
	{RSH, {"RSH", "raw Shapley Shubik"}},
	{SCB, {"SCB", "Symmetric Coalitional Banzhaf"}},
	{RSHB, {"RSHB", "raw Shapley Shubik below"}},
	{SH, {"SH", "Shapley Shubik"}},
	{SHB, {"SHB", "Shapley Shubik below"}},
	{SPGI, {"SPGI", "Solidarity Public Good Index"}},
	{UPGI, {"UPGI", "Union Public Good Index"}},
	{T, {"T", "Tijs"}},
	{TPGI1, {"TPGI1", "Threat Public Good Index 1"}},
	{TPGI2, {"TPGI2", "Threat Public Good Index 2"}},
	{TPGI3, {"TPGI3", "Threat Public Good Index 3"}},
	{W, {"W", "Number of winning coalitions"}},
	{WM, {"WM", "Number of minimal winning coalitions"}},
	{WS, {"WS", "Number of minimal winning coalitions of smallest cardinality"}}};

bool epic::index::IndexFactory::validateIndex(const std::string& short_name) {
	return getIndexType(short_name) != INVALID_INDEX;
}

epic::index::ItfPowerIndex* epic::index::IndexFactory::new_powerIndex(const std::string& short_name, Game* g, IntRepresentation int_representation) {
	ItfPowerIndex* index = nullptr;

	switch (getIndexType(short_name)) {
		case IndexType::INVALID_INDEX: index = nullptr; break;
		case IndexType::ABZ: index = new AbsoluteBanzhaf(); break;
		case IndexType::BZ: index = new Banzhaf(); break;
		case IndexType::BO: index = new BanzhafOwen(); break;
		case IndexType::BZB: index = new BanzhafBelow(); break;
		case IndexType::APG: index = new AbsolutePublicGood(); break;
		case IndexType::APIG: index = new AbsolutePowerIndexG(g); break;
		case IndexType::CC: index = new ColemanCollective(); break;
		case IndexType::CI: index = new ColemanInitiative(); break;
		case IndexType::CP: index = new ColemanPreventive(); break;
		case IndexType::DP: index = new DeeganPackel(); break;
		case IndexType::FT: index = new FelsenthalIndex(g); break;
		case IndexType::HN: index = new HarsanyiNashIndex(); break;
		case IndexType::J: index = new Johnston(g); break;
		case IndexType::KB: index = new KoenigBraeuninger(g); break;
		case IndexType::KBPH: index = new KoenigBraeuningerPH(g); break;
		case IndexType::N: index = new Nevison(g); break;
		case IndexType::NPH: index = new NevisonPH(g); break;
		case IndexType::O: index = new Owen(); break;
		case IndexType::OPGI: index = new OwenExtendedPGI(); break;
		case IndexType::PG: index = new PublicGood(); break;
		case IndexType::PHT: index = new PublicHelpTheta(g); break;
		case IndexType::PHTPH: index = new PublicHelpThetaPH(g); break;
		case IndexType::PHX: index = new PublicHelpXi(g); break;
		case IndexType::PIF: index = new PowerIndexF(g); break;
		case IndexType::PIG: index = new PowerIndexG(g); break;
		case IndexType::PIGPH: index = new PowerIndexGPH(g); break;
		case IndexType::RA: index = new Rae(g); break;
		case IndexType::RBZ: index = new RawBanzhaf(); break;
		case IndexType::RBZB: index = new RawBanzhafBelow(); break;
		case IndexType::RDP: index = new RawDeeganPackel(); break;
		case IndexType::RFT: index = new RawFelsenthal(g); break;
		case IndexType::RJ: index = new RawJohnston(g); break;
		case IndexType::RPG: index = new RawPublicGood(); break;
		case IndexType::RPHT: index = new RawPublicHelpTheta(g); break;
		case IndexType::RPHTSD: index = new RawPublicHelpThetaSD(g); break;
		case IndexType::RPIG: index = new RawPowerIndexG(g); break;
		case IndexType::RPIF: index = new RawPowerIndexF(g); break;
		case IndexType::RSH: index = new RawShapleyShubik(); break;
		case IndexType::SCB: index = new SymmetricCoalitionalBanzhaf(); break;
		case IndexType::RSHB: index = new RawShapleyShubikBelow(); break;
		case IndexType::SH: index = new ShapleyShubik(); break;
		case IndexType::SHB: index = new ShapleyShubikBelow(); break;
		case IndexType::SPGI: index = new SolidarityPGI(); break;
		case IndexType::T: index = new Tijs(); break;
		case IndexType::TPGI1: index = new ThreatPGI1(); break;
		case IndexType::TPGI2: index = new ThreatPGI2(); break;
		case IndexType::TPGI3: index = new ThreatPGI3(); break;
		case IndexType::UPGI: index = new UnionPGI(); break;
		case IndexType::W: index = new SingleValueW(); break;
		case IndexType::WM: index = new SingleValueWM(); break;
		case IndexType::WS: index = new SingleValueWS(g); break;
	}

	return index;
}

void epic::index::IndexFactory::delete_powerIndex(ItfPowerIndex* index) {
	delete index;
}

void epic::index::IndexFactory::printIndexList(std::ostream& os) {
	for (auto it = ++cIndexNames.begin(); it != cIndexNames.end(); ++it) { // first entry is INVALID_INDEX (do not print that one)
		os << it->second.first << ":\t" << it->second.second << std::endl;
	}
}

epic::index::IndexFactory::IndexType epic::index::IndexFactory::getIndexType(const std::string& short_name) {
	for (const auto& it : cIndexNames) {
		if (it.second.first == short_name) {
			return it.first;
		}
	}

	return INVALID_INDEX;
}
