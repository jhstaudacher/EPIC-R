#include "index/IndexFactory.h"

#include "FastUpperBoundApproximation.h"
#include "index/AbsoluteBanzhaf.h"
#include "index/Banzhaf.h"
#include "index/ColemanCollective.h"
#include "index/ColemanInitiative.h"
#include "index/ColemanPreventive.h"
#include "index/DeeganPackel.h"
#include "index/FelsenthalIndex.h"
#include "index/HarsanyiNashIndex.h"
#include "index/Johnston.h"
#include "index/KoenigBraeuninger.h"
#include "index/KoenigBraeuningerPH.h"
#include "index/Nevison.h"
#include "index/NevisonPH.h"
#include "index/PowerIndexF.h"
#include "index/PowerIndexG.h"
#include "index/PowerIndexGPH.h"
#include "index/PublicGood.h"
#include "index/PublicHelpTheta.h"
#include "index/PublicHelpThetaPH.h"
#include "index/PublicHelpXi.h"
#include "index/Rae.h"
#include "index/RawBanzhaf.h"
#include "index/RawDeeganPackel.h"
#include "index/RawFelsenthal.h"
#include "index/RawJohnston.h"
#include "index/RawPowerIndexF.h"
#include "index/RawPowerIndexG.h"
#include "index/RawPublicGood.h"
#include "index/RawPublicHelpTheta.h"
#include "index/RawPublicHelpThetaSD.h"
#include "index/RawShapleyShubik.h"
#include "index/ShapleyShubik.h"
#include "index/Tijs.h"

const std::map<epic::index::IndexFactory::IndexType, std::pair<std::string, std::string>> epic::index::IndexFactory::cIndexNames = {
	{INVALID_INDEX, {"INVALID", "invalid index"}},
	{ABZ, {"ABZ", "absolute Banzhaf"}},
	{BZ, {"BZ", "Banzhaf"}},
	{CC, {"CC", "Coleman collective"}},
	{CI, {"CI", "Coleman initiative"}},
	{CP, {"CP", "Coleman collective"}},
	{DP, {"DP", "Deegan Packel"}},
	{FT, {"FT", "Felsenthal"}},
	{HN, {"HN", "Harsanyi Nash"}},
	{J, {"J", "Johnston"}},
	{KB, {"KB", "Koenig Braeuninger (based on the Dubey-Shapley identity)"}},
	{KBPH, {"KBPH", "Koenig Braeuninger (based on raw Public Help theta)"}},
	{N, {"N", "Nevison (based on the Dubey-Shapley identity)"}},
	{NPH, {"NPHA", "Nevsion (based on raw Public Help theta)"}},
	{PG, {"PG", "Public Good"}},
	{PHT, {"PHT", "Public Help theta (based on the Dubey-Shapley identity)"}},
	{PHTPH, {"PHTPH", "Public Help theta (based on the raw Public Help theta"}},
	{PHX, {"PHX", "Public Help xi"}},
	{PIF, {"PIF", "null-player-free Power Index F"}},
	{PIG, {"PIG", "null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{PIGPH, {"PIGPH", "null-player-free Power Index G (based on raw Public Help theta)"}},
	{RA, {"RA", "Rae"}},
	{RBZ, {"RBZ", "raw Banzhaf"}},
	{RDP, {"RDP", "raw Deegan Packel"}},
	{RFT, {"RFT", "raw Felsenthal"}},
	{RJ, {"RJ", "raw Johnston"}},
	{RPG, {"RPG", "raw Public Good"}},
	{RPHT, {"RPHT", "raw Public Help theta"}},
	{RPHTSD, {"RPHTSD", "raw Public Help theta (based on the Dubey-Shapley identity)"}},
	{RPIG, {"RPIG", "raw null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{RPIF, {"RPIF", "raw null-player-free Power Index F"}},
	{RSH, {"RSH", "raw Shapley Shubik"}},
	{SH, {"SH", "Shapley Shubik"}},
	{T, {"T", "Tijs"}},
};

bool epic::index::IndexFactory::validateIndex(const std::string& short_name) {
	return getIndexType(short_name) != INVALID_INDEX;
}

epic::index::ItfPowerIndex* epic::index::IndexFactory::new_powerIndex(const std::string& short_name, Game& g, IntRepresentation int_representation) {
	ItfPowerIndex* index = nullptr;
	ItfUpperBoundApproximation* approx = new FastUpperBoundApproximation(g);

	switch (getIndexType(short_name)) {
		case IndexType::INVALID_INDEX: index = nullptr; break;
		case IndexType::ABZ: index = new AbsoluteBanzhaf(g, approx, int_representation); break;
		case IndexType::BZ: index = new Banzhaf(g, approx, int_representation); break;
		case IndexType::CC: index = new ColemanCollective(g, approx, int_representation); break;
		case IndexType::CI: index = new ColemanInitiative(g, approx, int_representation); break;
		case IndexType::CP: index = new ColemanPreventive(g, approx, int_representation); break;
		case IndexType::DP: index = new DeeganPackel(g, approx, int_representation); break;
		case IndexType::FT: index = new FelsenthalIndex(g, approx, int_representation); break;
		case IndexType::HN: index = new HarsanyiNashIndex(g); break;
		case IndexType::J: index = new Johnston(g, approx, int_representation); break;
		case IndexType::KB: index = new KoenigBraeuninger(g, approx, int_representation); break;
		case IndexType::KBPH: index = new KoenigBraeuningerPH(g, approx, int_representation); break;
		case IndexType::N: index = new Nevison(g, approx, int_representation); break;
		case IndexType::NPH: index = new NevisonPH(g, approx, int_representation); break;
		case IndexType::PG: index = new PublicGood(g, approx, int_representation); break;
		case IndexType::PHT: index = new PublicHelpTheta(g, approx, int_representation); break;
		case IndexType::PHTPH: index = new PublicHelpThetaPH(g, approx, int_representation); break;
		case IndexType::PHX: index = new PublicHelpXi(g, approx, int_representation); break;
		case IndexType::PIF: index = new PowerIndexF(g, approx, int_representation); break;
		case IndexType::PIG: index = new PowerIndexG(g, approx, int_representation); break;
		case IndexType::PIGPH: index = new PowerIndexGPH(g, approx, int_representation); break;
		case IndexType::RA: index = new Rae(g, approx, int_representation); break;
		case IndexType::RBZ: index = new RawBanzhaf(g, approx, int_representation); break;
		case IndexType::RDP: index = new RawDeeganPackel(g, approx, int_representation); break;
		case IndexType::RFT: index = new RawFelsenthal(g, approx, int_representation); break;
		case IndexType::RJ: index = new RawJohnston(g, approx, int_representation); break;
		case IndexType::RPG: index = new RawPublicGood(g, approx, int_representation); break;
		case IndexType::RPHT: index = new RawPublicHelpTheta(g, approx, int_representation); break;
		case IndexType::RPHTSD: index = new RawPublicHelpThetaSD(g, approx, int_representation); break;
		case IndexType::RPIG: index = new RawPowerIndexG(g, approx, int_representation); break;
		case IndexType::RPIF: index = new RawPowerIndexF(g, approx, int_representation); break;
		case IndexType::RSH: index = new RawShapleyShubik(g, approx, int_representation); break;
		case IndexType::SH: index = new ShapleyShubik(g, approx, int_representation); break;
		case IndexType::T: index = new Tijs(g); break;
	}

	delete approx;
	return index;
}

void epic::index::IndexFactory::delete_powerIndex(ItfPowerIndex* index) {
	delete index;
}

void epic::index::IndexFactory::printIndexList(std::ostream& os) {
	for (const auto& it : cIndexNames) {
		os << it.second.first << ":\t" << it.second.second << std::endl;
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
