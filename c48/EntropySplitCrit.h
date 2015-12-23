#ifndef _ENTROPYSPLITCRIT_
#define _ENTROPYSPLITCRIT_

#include "EntropyBasedSplitCrit.h"
#include <string>
#include <cmath>

class Distribution;

/**
 * Class for computing the entropy for a given distribution.
 */
class EntropySplitCrit : public EntropyBasedSplitCrit {

public:

	/**
	 * Computes entropy for given distribution.
	 */
	double splitCritValue(Distribution *bags);

	/**
	 * Computes entropy of test distribution with respect to training distribution.
	 */
	double splitCritValue(Distribution *train, Distribution *test);
};

#endif	//#ifndef _ENTROPYSPLITCRIT_
