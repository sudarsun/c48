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
    int mIntCount = 0;

    /** The number of real-like values (i.e. have a fractional part) */
    int mRealCount = 0;

    /** The number of missing values */
    int mMissingCount = 0;

    /** The number of distinct values */
    int mDistinctCount = 0;

    /** The number of values that only appear once */
    int mUniqueCount = 0;

    /** The total number of values (i.e. number of instances) */
    int mTotalCount = 0;

    /** Stats on numeric value distributions */
    Statistics *mNumericStats;

    /** Counts of each nominal value */
    int_array mNominalCounts;

    /** Weight mass for each nominal value */
    double_array mNominalWeights;

    /**
     * Updates the counters for one more observed distinct value.
     *
     * @param value the value that has just been seen
     * @param count the number of times the value appeared
     */
    virtual void addDistinct(const double value, const int count, const double weight);

};

#endif    //#ifndef _ATTRIBUTESTATS_
