#ifndef _CLASSIFIER_
#define _CLASSIFIER_

#include <vector>
#include "core/Instances.h"
#include "core/Instance.h"


/**
 * Classifier interface. All schemes for numeric or nominal prediction in
 * C48 implement this interface. Note that a classifier MUST either implement
 * distributionForInstance() or classifyInstance().
 */
class Classifier {

public:
    /**
     * Generates a classifier. Must initialize all fields of the classifier
     * that are not being set via options (i.e. multiple calls of buildClassifier
     * must always lead to the same result). Must not change the dataset
     * in any way.
     *
     * @param data set of instances serving as training data
     * @exception Exception if the classifier has not been
     * generated successfully
     */

    virtual void buildClassifier(Instances *data) = 0;

    /**
     * Classifies the given test instance. The instance has to belong to a
     * dataset when it's being classified. Note that a classifier MUST
     * implement either this or distributionForInstance().
     *
     * @param instance the instance to be classified
     * @return the predicted most likely class for the instance or
     * Utils::missingValue() if no prediction is made
     * @exception Exception if an error occurred during the prediction
     */
    virtual double classifyInstance(Instance *instance) = 0;

    /**
     * Predicts the class memberships for a given instance. If
     * an instance is unclassified, the returned array elements
     * must be all zero. If the class is numeric, the array
     * must consist of only one element, which contains the
     * predicted value. Note that a classifier MUST implement
     * either this or classifyInstance().
     *
     * @param instance the instance to be classified
     * @return an array containing the estimated membership
     * probabilities of the test instance in each class
     * or the numeric prediction
     * @exception Exception if distribution could not be
     * computed successfully
     */
    virtual double_array distributionForInstance(Instance *instance) = 0;

};

#endif    //_CLASSIFIER_
