#ifndef _ENTROPYSPLITCRIT_
#define _ENTROPYSPLITCRIT_

#include "EntropyBasedSplitCrit.h"
#include <string>
#include <cmath>

class Distribution;



                /// <summary>
                /// Class for computing the entropy for a given distribution.
                /// 
                /// @author Eibe Frank (eibe@cs.waikato.ac.nz)
                /// @version $Revision$
                /// </summary>
                class EntropySplitCrit : public EntropyBasedSplitCrit {


                  /// <summary>
                  /// Computes entropy for given distribution.
                  /// </summary>
              public:
                  double splitCritValue( Distribution *bags );

                  /// <summary>
                  /// Computes entropy of test distribution with respect to training distribution.
                  /// </summary>
                  double splitCritValue( Distribution *train, Distribution *test );

                  
                };

#endif	//#ifndef _ENTROPYSPLITCRIT_
