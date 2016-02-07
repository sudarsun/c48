#ifndef _ABSTRACTCLASSIFIER_
#define _ABSTRACTCLASSIFIER_

#include "Classifier.h"
#include "evaluation/BatchPredictor.h"
#include <string>
#include <vector>
#include <stdexcept>

// Forward class declarations:
class Instance;
class Instances;

/**
 * Abstract classifier Class. All schemes for numeric or nominal prediction in C48
 * extend this class. Note that a classifier MUST either implement
 * distributionForInstance() or classifyInstance().
 */
class AbstractClassifier : public Classifier, public BatchPredictor {

protected:

    /* Whether the classifier is run in debug mode. */
    bool mDebug;

    int mNumDecimalPlaces = 2;
    /* The number of decimal places used when printing numbers in the model.*/

public:
    static const int NUM_DECIMAL_PLACES_DEFAULT = 2;

    /* Default preferred batch size for batch predictions */
    string mBatchSize = "100";

    /**
     * Classifies the given test instance. The instance has to belong to a dataset
     * when it's being classified. Note that a classifier MUST implement either
     * this or distributionForInstance().
     *
     * @param instance the instance to be classified
     * @return the predicted most likely class for the instance or
     *         Utils::missingValue() if no prediction is made
     * @exception Exception if an error occurred during the prediction
     */
public:
    virtual double classifyInstance(Instance &instance) const;


    /**
     * Predicts the class memberships for a given instance. If an instance is
     * unclassified, the returned array elements must be all zero. If the class is
     * numeric, the array must consist of only one element, which contains the
     * predicted value. Note that a classifier MUST implement either this or
     * classifyInstance().
     *
     * @param instance the instance to be classified
     * @return an array containing the estimated membership probabilities of the
     *         test instance in each class or the numeric prediction
     * @exception Exception if distribution could not be computed successfully
     */
    virtual double_array distributionForInstance(Instance &instance) const;


    /**
     * Get whether debugging is turned on.
     *
     * @return true if debugging output is on
     */
    virtual bool getDebug() const;


    /**
     * Set debugging mode.
     *
     * @param debug true if debug output should be printed
     */
    virtual void setDebug(bool debug);


    /**
     * Get the number of decimal places.
     */
    virtual int getNumDecimalPlaces() const;


    /**
     * Set the number of decimal places.
     */
    virtual void setNumDecimalPlaces(int num);


    /**
     * Set the preferred batch size for batch prediction.
     *
     * @param size the batch size to use
     */
    virtual void setBatchSize(const string &size);


    /**
     * Get the preferred batch size for batch prediction.
     *
     * @return the preferred batch size
     */
    virtual string getBatchSize() const;


    /**
     * Return true if this classifier can generate batch predictions in an
     * efficient manner. Default implementation here returns false. Subclasses to
     * override as appropriate.
     *
     * @return true if this classifier can generate batch predictions in an
     *         efficient manner.
     */
    virtual bool implementsMoreEfficientBatchPrediction() const;


    /**
     * Batch prediction method. This default implementation simply calls
     * distributionForInstance() for each instance in the batch. If subclasses can
     * produce batch predictions in a more efficient manner than this they should
     * override this method and also return true from
     * implementsMoreEfficientBatchPrediction()
     *
     * @param batch the instances to get predictions for
     * @return an array of probability distributions, one for each instance in the
     *         batch
     * @throws Exception if a problem occurs.
     */
    virtual double_2D_array distributionsForInstances(Instances &batch) const;
};


#endif    // _ABSTRACTCLASSIFIER_
