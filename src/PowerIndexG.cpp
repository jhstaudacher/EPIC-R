#include "PowerIndexG.h"

#include "Array.h"
#include "Logging.h"

epic::index::PowerIndexG::PowerIndexG(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPowerIndexG(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::PowerIndexG::calculate() {
	auto solution = RawPowerIndexG::calculate();

	bigFloat sum_swc = 0;
	for (const auto& it : solution) {
		sum_swc += it;
	}

	if (log::out.getLogLevel() <= log::info) {		
		log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << GMPHelper::mpf_class_to_string(solution[i]) << log::endl;
		}
	}

	for (auto& it : solution) {
		it /= sum_swc;
	}

	return solution;
}

std::string epic::index::PowerIndexG::getFullName() {
	return "PowerIndexG";
}
