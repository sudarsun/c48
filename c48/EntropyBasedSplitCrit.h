#ifndef _ENTROPYBASEDSPLITCRIT_
#define _ENTROPYBASEDSPLITCRIT_

#include "SplitCriterion.h"

class Distribution;

/**
 * "Abstract" class for computing splitting criteria
 * based on the entropy of a class distribution.
 */
class EntropyBasedSplitCrit : public SplitCriterion {

public:

    /**
     * Help method for computing entropy.
     */
    double lnFunc(double num) const;

    /**
     * Computes entropy of distribution before splitting.
     */
    double oldEnt(Distribution &bags) const;

    /**
     * Computes entropy of distribution after splitting.
     */
    double newEnt(Distribution &bags) const;

    /**
     * Computes entropy after splitting without considering the
     * class values.
     */
    double splitEnt(Distribution &bags) const;
};


#endif    //#ifndef _ENTROPYBASEDSPLITCRIT_
