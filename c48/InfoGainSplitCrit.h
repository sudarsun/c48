#ifndef _INFOGAINSPLITCRIT_
#define _INFOGAINSPLITCRIT_

#include "EntropyBasedSplitCrit.h"
#include <string>
#include <limits>

// Forward class declarations:
class Distribution;

/**
 * Class for computing the information gain for a given distribution.
 *
 */
class InfoGainSplitCrit : public EntropyBasedSplitCrit {


public:
  /**
   * This method is a straightforward implementation of the information
   * gain criterion for the given distribution.
   */
	double splitCritValue(Distribution *bags);

  /**
   * This method computes the information gain in the same way 
   * C4.5 does.
   *
   * @param bags the distribution
   * @param totalNoInst weight of ALL instances (including the
   * ones with missing values).
   */
	double splitCritValue(Distribution *bags, double totalNoInst);

  /**
   * This method computes the information gain in the same way 
   * C4.5 does.
   *
   * @param bags the distribution
   * @param totalNoInst weight of ALL instances 
   * @param oldEnt entropy with respect to "no-split"-model.
   */
	double splitCritValue(Distribution *bags, double totalNoInst, double oldEnt);

};

#endif	//#ifndef _INFOGAINSPLITCRIT_
