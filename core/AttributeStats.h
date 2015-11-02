#ifndef _ATTRIBUTESTATS_
#define _ATTRIBUTESTATS_

#include <string>
#include <vector>
#include <cmath>
#include "core/statistics.h"


    /// <summary>
    /// A Utility class that contains summary information on an
    /// the values that appear in a dataset for a particular attribute.
    ///
    /// </summary>
    class AttributeStats {

      /// <summary>
      /// The number of int-like values </summary>
  public:
      int intCount = 0;

      /// <summary>
      /// The number of real-like values (i.e. have a fractional part) </summary>
      int realCount = 0;

      /// <summary>
      /// The number of missing values </summary>
      int missingCount = 0;

      /// <summary>
      /// The number of distinct values </summary>
      int distinctCount = 0;

      /// <summary>
      /// The number of values that only appear once </summary>
      int uniqueCount = 0;

      /// <summary>
      /// The total number of values (i.e. number of instances) </summary>
      int totalCount = 0;

      /// <summary>
      /// Stats on numeric value distributions </summary>
      // perhaps Stats should be moved from weka.experiment to weka.core
      Statistics *numericStats;

      /// <summary>
      /// Counts of each nominal value </summary>
      std::vector<int> nominalCounts;

      /// <summary>
      /// Weight mass for each nominal value </summary>
      std::vector<double> nominalWeights;

      /// <summary>
      /// Updates the counters for one more observed distinct value.
      /// </summary>
      /// <param name="value"> the value that has just been seen </param>
      /// <param name="count"> the number of times the value appeared </param>
      /// <param name="weight"> the weight mass of the value </param>

      virtual void addDistinct( double value, int count, double weight );

      /// <summary>
      /// Returns a human readable representation of this AttributeStats instance.
      /// </summary>
      /// <returns> a String represtinging these AttributeStats. </returns>
  public:
      virtual std::string toString();
    };

#endif	//#ifndef _ATTRIBUTESTATS_
