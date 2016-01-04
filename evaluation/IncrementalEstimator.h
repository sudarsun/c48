#ifndef _INCREMENTALESTIMATOR_
#define _INCREMENTALESTIMATOR_

/**
 * Interface for an incremental probability estimators.
 *
 */
class IncrementalEstimator {

public:
    /**
     * Add a new data value to the current estimator.
     *
     * @param data the new data value
     * @param weight the weight assigned to the data value
     */
    virtual void addValue(double data, double weight) = 0;

};
#endif //_INCREMENTALESTIMATOR_