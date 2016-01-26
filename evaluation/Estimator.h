#ifndef _ESTIMATOR_
#define _ESTIMATOR_

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "core/Typedefs.h"

//Forward class declarations:
class Instances;

/**
 * Abstract class for all estimators.
 */
class Estimator {

protected:

    /** The class value index is > -1 if subset is taken with specific class value only*/
    double mClassValueIndex = -1.0;

    /** set if class is not important */
    bool mNoClass = true;

public:
    /**
     * Add a new data value to the current estimator.
     *
     * @param data the new data value
     * @param weight the weight assigned to the data value
     */
    virtual void addValue( double data, const double weight);

    /**
     * Initialize the estimator with a new dataset.
     * Finds min and max first.
     *
     * @param data the dataset used to build this estimator
     * @param attrIndex attribute the estimator is for
     * @exception Exception if building of estimator goes wrong
     */
    virtual void addValues(Instances *data, const int attrIndex);

    /**
     * Initialize the estimator with all values of one attribute of a dataset.
     * Some estimator might ignore the min and max values.
     *
     * @param data the dataset used to build this estimator
     * @param attrIndex attribute the estimator is for
     * @param min minimal border of range
     * @param max maximal border of range
     * @param factor number of instances has been reduced to that factor
     * @exception Exception if building of estimator goes wrong
     */
    virtual void addValues(Instances *data, const int attrIndex, const double min, const double max, const double factor);

    /**
    * Initialize the estimator using only the instance of one class.
    * It is using the values of one attribute only.
    *
    * @param data the dataset used to build this estimator
    * @param attrIndex attribute the estimator is for
    * @param classIndex index of the class attribute
    * @param classValue the class value
    * @exception Exception if building of estimator goes wrong
    */
    virtual void addValues(Instances *data, const int attrIndex, const int classIndex, const int classValue);

    /**
     * Initialize the estimator using only the instance of one class.
     * It is using the values of one attribute only.
     *
     * @param data the dataset used to build this estimator
     * @param attrIndex attribute the estimator is for
     * @param classIndex index of the class attribute
     * @param classValue the class value
     * @param min minimal value of this attribute
     * @param max maximal value of this attribute
     * @exception Exception if building of estimator goes wrong
     */
    virtual void addValues(Instances *data, const int attrIndex, const int classIndex, const int classValue, const double min, const double max);

    /**
     * Get a probability estimate for a value.
     *
     * @param data the value to estimate the probability of
     * @return the estimated probability of the supplied value
     */
    virtual double getProbability(const double data) const = 0;

    /**
      * Set debugging mode.
      *
      * @param debug true if debug output should be printed
      */
    virtual void setDebug(const bool debug);

    /**
     * Get whether debugging is turned on.
     *
     * @return true if debugging output is on
     */
    virtual bool getDebug() const;

private:

    /** Debugging mode */
    bool mDebug;

    /**
     * Returns a dataset that contains all instances of a certain class value.
     *
     * @param data dataset to select the instances from
     * @param attrIndex index of the relevant attribute
     * @param classIndex index of the class attribute
     * @param classValue the relevant class value
     * @return a dataset with only
     */
    double getInstancesFromClass(Instances *data, const int attrIndex, const int classIndex, const double classValue, Instances *workData);

    /**
     * Class to support a building process of an estimator.
     */
    class Builder {

    public:
        /** instances of the builder */
        Instances *mInstances = nullptr;

        /** attribute index of the builder */
        int mAttrIndex = -1;

        /** class index of the builder, only relevant if class value index is set*/
        int mClassIndex = -1;

        /** class value index of the builder */
        int mClassValueIndex = -1;
    };

};

#endif    //#ifndef _ESTIMATOR_
