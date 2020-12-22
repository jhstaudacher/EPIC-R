#ifndef EPIC_INDEX_ABSOLUTEPUBLICGOOD_H_
#define EPIC_INDEX_ABSOLUTEPUBLICGOOD_H_

#include "RawPublicGood.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Absolute Public Good (Holler)</i> power index (`PG`) based on the <i>raw Public Good</i> power index.
 *
 * Let \f$W_i^m\f$ denote the set of minimal winning coalitions containing player \f$i\f$.
 * Then the <i>Public Good</i> index of player \f$i\f$ is defined as
 * \f[
 *		h_i = \frac{|W_i^m|}{|W^m|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of minimal winning coalitions as well as the number of minimal winning coalitions each player is a member of (raw Public Good).
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class AbsolutePublicGood : public RawPublicGood {
public:
	/**
	 * Construct the AbsolutePublicGood object
	 *
	 * @param g The Game for which the AbsolutePublicGood index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	AbsolutePublicGood(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;

protected:
	/*
	 * A function to compute the total number of minimal winning coalitions
	 *
	 * @param total_mwc The total number of minimal winning coalitions
	 */
	void numberOfMinimalWinningCoalitions(bigInt* total_mwc);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_ABSOLUTEPUBLICGOOD_H_ */