#ifndef EPIC_INDEX_RAWFELSENTHAL_H_
#define EPIC_INDEX_RAWFELSENTHAL_H_

#include "Array.h"
#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>absolute Felsenthal</i> power index (`AFT`).
 *
 * Let \f$W_i^s\f$ be the set of minimal winning coalitions of smallest cardinality containing player \f$i\f$.
 * Then the <i>absolute Felsenthal</i> index of player \f$i\f$ is defined as
 * \f[
 *		PI_i = \sum_{S \in W_i^s} \frac{1}{|S|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class RawFelsenthal : public ItfPowerIndex {
public:
	/**
	 * Construct the RawFelsenthal index object
	 *
	 * @param g The Game for which the RawFelsenthal index should later be calculated.
	 *
	 * Nearly similar to the Deegan Packel algorithm. But at this time in achieving
	 * the minimum winning coalitions of the smallest size are interested only.
	 * For this reason, the loops do not iterate over all players, only up to the number of players, which
	 * the cumulative sum of there weights are bigger or equivalent to the Quota (this number called minsize).
	 * Please note, the weights are sorted in descending order.
	 * Furthermore, the number of minimally winning coalitions of least size of each player is now determined.
	 * Using this, the index is calculated as follows: (MWCS of player i) / (SUM of MWCS of all players)
	 */
	RawFelsenthal(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

protected:
	/**
	 * calculating the number of minimal winning coalitions of least size per player
	 *
	 * @param g The Game object for the current calculation
	 * @param mwcs The return array, mwcs[x]: the number of minimal winning coalitions of least size player x belongs to. The array mus have enough memory for at least numberOfPlayers entries. Each entry must be initialized with zero!
	 */
	void minimal_winning_coalitions_of_least_size(Game* g, lint::LargeNumber mwcs[]);

private:
	/**
	 * minimal winning coalitions of least size
	 */
	longUInt mMinsize;
};

} // namespace epic::index

#endif /* EPIC_INDEX_RAWFELSENTHAL_H_ */
