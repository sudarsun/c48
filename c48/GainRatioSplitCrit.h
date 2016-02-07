#ifndef _GAINRATIOSPLITCRIT_
#define _GAINRATIOSPLITCRIT_

#include "EntropyBasedSplitCrit.h"
#include <string>
#include <limits>

// Forward class declarations:
class Distribution;

/**
 * Class for computing the gain ratio for a given distribution.
 *
 */
class GainRatioSplitCrit : public EntropyBasedSplitCrit {


public:
    /**
     * This method is a straightforward implementation of the gain
     * ratio criterion for the given distribution.
     */
    double splitCritValue(Distribution &bags) const;

    /**
     * This method computes the gain ratio in the same way C4.5 does.
     *
     * @param bags the distribution
     * @param totalnoInst the weight of ALL instances
     * @param numerator the info gain
     */
    double splitCritValue(Distribution &bags, double totalnoInst, double numerator) const;

private:
    /**
     * Help method for computing the split entropy.
     */
    double splitEnt(Distribution &bags, double totalnoInst) const;

};

#endif    //#ifndef _GAINRATIOSPLITCRIT_
