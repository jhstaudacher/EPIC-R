#ifndef EPIC_INDEX_NEVISON_H_
#define EPIC_INDEX_NEVISON_H_

#include "ItfUpperBoundApproximation.h"
#include "RawBanzhaf.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Nevison</i> power index (`N`) based on the <i>raw Banzhaf</i> power index.
 *
 * Let \f$W_i\f$ denote the set of winning coalitions containing player \f$i\f$.
 * Then the <i>Nevison</i> index of player \f$i\f$ is defined as
 * \f[
 *		Z_i = \frac{|W_i|}{2^n}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>INVALID - this power index is null player sensitive
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index \f$Z_i = \frac{|W|}{2^n}\f$.
 */
class Nevison : public RawBanzhaf {
public:
	/**
	 * Construct the Nevison object
	 *
	 * @param g The Game for which the Nevison index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	Nevison(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_NEVISON_H_ */