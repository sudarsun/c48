#ifndef _ATTRIBUTESTATS_
#define _ATTRIBUTESTATS_

#include <string>
#include <vector>
#include <cmath>
#include "statistics.h"
#include "Typedefs.h"

/**
 * A Utility class that contains summary information on an
 * the values that appear in a dataset for a particular attribute.
 */
class AttributeStats {

public:

    /** The number of int-like values */
    int intCount = 0;

    /** The number of real-like values (i.e. have a fractional part) */
    int realCount = 0;

    /** The number of missing values */
    int missingCount = 0;

    /** The number of distinct values */
    int distinctCount = 0;

    /** The number of values that only appear once */
    int uniqueCount = 0;

    /** The total number of values (i.e. number of instances) */
    int totalCount = 0;

    /** Stats on numeric value distributions */
    Statistics *numericStats;

    /** Counts of each nominal value */
    int_array nominalCounts;

    /** Weight mass for each nominal value */
    double_array nominalWeights;

    /**
     * Updates the counters for one more observed distinct value.
     *
     * @param value the value that has just been seen
     * @param count the number of times the value appeared
     */
    virtual void addDistinct(double value, int count, double weight);

};

#endif    //#ifndef _ATTRIBUTESTATS_
