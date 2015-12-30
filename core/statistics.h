#ifndef _STATISTICS_
#define _STATISTICS_

#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <limits>

/**
 * A class to store simple statistics.<para>
 *
 */
class Statistics {

public:

    /** The number of values seen */
    double count = 0;

    //** The sum of values seen */
    double sum = 0;

    //** The sum of values squared seen */
    double sumSq = 0;

    //** The std deviation of values at the last calculateDerived() call */
    double stdDev = std::numeric_limits<double>::quiet_NaN();

    //** The mean of values at the last calculateDerived() call */
    double mean = std::numeric_limits<double>::quiet_NaN();

    //** The minimum value seen, or Double.NaN if no values seen */
    double min = std::numeric_limits<double>::quiet_NaN();

    //** The maximum value seen, or Double.NaN if no values seen */
    double max = std::numeric_limits<double>::quiet_NaN();

    /**
     * Adds a value to the observed values
     *
     * @param value the observed value
     */
    virtual void add(double value);

    /**
    * Adds a value that has been seen n times to the observed values
    *
    * @param value the observed value
    * @param n the number of times to add value
    */
    virtual void add(double value, double weight);

    /**
     * Removes a value to the observed values (no checking is done
     * that the value being removed was actually added).
     *
     * @param value the observed value
     */
    virtual void subtract(double value);

    /**
    * Subtracts a value that has been seen n times from the observed values
    *
    * @param value the observed value
    * @param n the number of times to subtract value
    */
    virtual void subtract(double value, double weight);

    /**
     * Tells the object to calculate any statistics that don't have their
     * values automatically updated during add. Currently updates the mean
     * and standard deviation.
     */
    virtual void calculateDerived();

private:

    double stdDevFactor = 0;

    /** Reset the statistic parameters */
    void reset();

    /** Set the negative value */
    void negativeCount();

    /** Reset the negative value */
    void goInvalid();

    /** Check the count is valid */
    bool isInvalid();

}; // Stats


#endif    // _STATISTICS_
