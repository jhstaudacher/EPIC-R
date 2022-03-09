#ifndef EPIC_INDEX_ITFPOWERINDEX_H_
#define EPIC_INDEX_ITFPOWERINDEX_H_

#include "Game.h"
#include "ItfUpperBoundApproximation.h"
#include "ItfLargeNumberCalculator.h"

namespace epic::index {

class ItfPowerIndex {
public:
	virtual ~ItfPowerIndex() = default;

	/**
	 * Calculating the power index for each player.
	 *
	 * @param g A Game-object for which the power index should be calculated
	 * @return A vector of the calculated power index (sorted for decreasing player weights)
	 */
	virtual std::vector<bigFloat> calculate(Game* g) = 0;

	/**
	 * A function to get the full name of the index.
	 *
	 * @return The full name of the power index
	 */
	virtual std::string getFullName() = 0;

	/**
	 * Getting an approximated size of memory (RAM) needed for computation
	 *
	 * @return The approximated RAM size in <cMemUnit_name>. If 0 is returned the approximated memory is either to complex for calculation, not implemented or larger than 2^64 Bytes (does not fit in to a longUInt variable).
	 */
	virtual longUInt getMemoryRequirement(Game* g) = 0;

	/**
	 * An upper bound for the greatest value that possibly needs to be handled as LargeNumber during the calculate()-call
	 *
	 * @param approx The object used for the approximation
	 * @return The calculated upper bound
	 */
	virtual bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) = 0;
	
	/**
	 * Get the most complex that must be evaluated on LargeNumber objects during the calculate()-call
	 *
	 * @return The specified operation
	 */
	virtual lint::Operation getOperationRequirement() = 0;
	
protected:
}; /* ItfPowerIndex */

} /* namespace epic::index */

#endif /* EPIC_INDEX_ITFPOWERINDEX_H_ */
