#ifndef _GAINRATIOSPLITCRIT_
#define _GAINRATIOSPLITCRIT_

#include "EntropyBasedSplitCrit.h"
#include <string>
#include <limits>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Distribution;



                /// <summary>
                /// Class for computing the gain ratio for a given distribution.
                /// 
                /// @author Eibe Frank (eibe@cs.waikato.ac.nz)
                /// @version $Revision$
                /// </summary>
                class GainRatioSplitCrit : public EntropyBasedSplitCrit {


                  /// <summary>
                  /// This method is a straightforward implementation of the gain ratio criterion
                  /// for the given distribution.
                  /// </summary>
              public:
                  double splitCritValue( Distribution *bags );

                  /// <summary>
                  /// This method computes the gain ratio in the same way C4.5 does.
                  /// </summary>
                  /// <param name="bags"> the distribution </param>
                  /// <param name="totalnoInst"> the weight of ALL instances </param>
                  /// <param name="numerator"> the info gain </param>
                  double splitCritValue( Distribution *bags, double totalnoInst, double numerator );

                  /// <summary>
                  /// Help method for computing the split entropy.
                  /// </summary>
              private:
                  double splitEnt( Distribution *bags, double totalnoInst );

                };

#endif	//#ifndef _GAINRATIOSPLITCRIT_
