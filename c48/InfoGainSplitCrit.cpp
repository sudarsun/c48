#include "InfoGainSplitCrit.h"
#include "Distribution.h"


                double InfoGainSplitCrit::splitCritValue( Distribution *bags ) {

                  double numerator;

                  numerator = oldEnt( bags ) - newEnt( bags );

                  // Splits with no gain are useless.
                  if( Utils::eq( numerator, 0 ) ) {
                    return std::numeric_limits<double>::max();
                  }

                  // We take the reciprocal value because we want to minimize the
                  // splitting criterion's value.
                  return bags->total() / numerator;
                }

                double InfoGainSplitCrit::splitCritValue( Distribution *bags, double totalNoInst ) {

                  double numerator;
                  double noUnknown;
                  double unknownRate;
                  noUnknown = totalNoInst - bags->total();
                  unknownRate = noUnknown / totalNoInst;
                  numerator = ( oldEnt( bags ) - newEnt( bags ) );
                  numerator = ( 1 - unknownRate ) * numerator;

                  // Splits with no gain are useless.
                  if( Utils::eq( numerator, 0 ) ) {
                    return 0;
                  }

                  return numerator / bags->total();
                }

                double InfoGainSplitCrit::splitCritValue( Distribution *bags, double totalNoInst, double oldEnt ) {

                  double numerator;
                  double noUnknown;
                  double unknownRate;
                  noUnknown = totalNoInst - bags->total();
                  unknownRate = noUnknown / totalNoInst;
                  numerator = ( oldEnt - newEnt( bags ) );
                  numerator = ( 1 - unknownRate ) * numerator;

                  // Splits with no gain are useless.
                  if( Utils::eq( numerator, 0 ) ) {
                    return 0;
                  }

                  return numerator / bags->total();
                }

              