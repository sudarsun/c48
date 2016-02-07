#ifndef _ESTIMATORUTILS_
#define _ESTIMATORUTILS_

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "core/Typedefs.h"

// Forward class declarations:
class Instances;
class Instance;
class Estimator;

/**
 * Contains static utility functions for Estimators.<p>
 */
class EstimatorUtils {

public:
    /**
     * Find the minimum distance between values
     * @param inst sorted instances, sorted
     * @param attrIndex index of the attribute, they are sorted after
     * @return the minimal distance
     */
    static double findMinDistance(Instances &inst, const int attrIndex);

    /**
     * Find the minimum and the maximum of the attribute and return it in
     * the last parameter..
     * @param inst instances used to build the estimator
     * @param attrIndex index of the attribute
     * @param minMax the array to return minimum and maximum in
     * @return number of not missing values
     * @exception Exception if parameter minMax wasn't initialized properly
     */
    static int getMinMax(Instances &inst, const int attrIndex, double_array &minMax);

    /**
     * Returns a dataset that contains all instances of a certain class value.
     *
     * @param data dataset to select the instances from
     * @param attrIndex index of the relevant attribute
     * @param classIndex index of the class attribute
     * @param classValue the relevant class value
     * @return a dataset with only
     */
     //static std::vector getInstancesFromClass( Instances *data, int attrIndex, int classIndex, double classValue, Instances *workData );


    /**
    * Returns a dataset that contains of all instances of a certain class value.
    * @param data dataset to select the instances from
    * @param classIndex index of the class attribute
    * @param classValue the class value
    * @return a dataset with only instances of one class value
    */
    static Instances *getInstancesFromClass(Instances &data, const int classIndex, const double classValue);

    /**
     * Returns a dataset that contains of all instances of a certain value
     * for the given attribute.
     * @param data dataset to select the instances from
     * @param index the index of the attribute
     * @param v the value
     * @return a subdataset with only instances of one value for the attribute
     */
    static Instances *getInstancesFromValue(Instances &data, const int index, const double v);


    /**
     * Returns a string representing the cutpoints
     */
    static string cutpointsToString(const double_array &cutPoints, const bool_array &cutAndLeft);
};


#endif    //#ifndef _ESTIMATORUTILS_
