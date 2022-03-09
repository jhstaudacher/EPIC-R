#ifndef EPIC_INDEX_SHAPLEYSHUBIK_H_
#define EPIC_INDEX_SHAPLEYSHUBIK_H_

#include "ItfUpperBoundApproximation.h"
#include "RawShapleyShubik.h"
#include "ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Shapley Shubik</i> power index (`SH`) based on the <i>raw Shapley Shubik</i> power index.
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 * Then the <i>Shapley Shubik</i> index of player \f$i\f$ is defined as
 * \f[
 *		\phi_i = \sum_{c = 1}^n \frac{\eta_i(\nu, c)}{c{n \choose c}}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 */
class ShapleyShubik : public RawShapleyShubik {
public:
	ShapleyShubik();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_SHAPLEYSHUBIK_H_ */
