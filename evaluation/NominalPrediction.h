#ifndef _NOMINALPREDICTION_
#define _NOMINALPREDICTION_

#include "Prediction.h"
#include <string>
#include <vector>

/**
 * Encapsulates an evaluatable nominal prediction: the predicted probability
 * distribution plus the actual class value.
 *
 */
class NominalPrediction : public Prediction {

    /** The predicted probabilities  */
private:
    double_array mDistribution;

    /** The actual class value  */
    double mActual = MISSING_VALUE;

    /** The predicted class value  */
    double mPredicted = MISSING_VALUE;

    /** The weight assigned to this prediction  */
    double mWeight = 1;

    /**
     * Determines the predicted class (doesn't detect multiple
     * classifications). If no prediction was made (i.e. all zero
     * probababilities in the distribution), m_Prediction is set to
     * MISSING_VALUE.
     */
    void updatePredicted();

public:
    /**
     * Creates the NominalPrediction object with a default weight of 1.0.
     *
     * @param actual the actual value, or MISSING_VALUE.
     * @param distribution the predicted probability distribution. Use
     * NominalPrediction.makeDistribution() if you only know the predicted value.
     */
    NominalPrediction(double actual, double_array &distribution);

    /**
     * Creates the NominalPrediction object.
     *
     * @param actual the actual value, or MISSING_VALUE.
     * @param distribution the predicted probability distribution. Use
     * NominalPrediction.makeDistribution() if you only know the predicted value.
     * @param weight the weight assigned to the prediction.
     */
    NominalPrediction(double actual, double_array &distribution, double weight);

    /**
     * Gets the predicted probabilities
     *
     * @return the predicted probabilities
     */
    virtual double_array distribution();

    /** Gets the actual class value.
    ///  */
    /// <returns> the actual class value, or MISSING_VALUE if no
    /// prediction was made.    */
    virtual double actual();

    /**
     * Gets the predicted class value.
     *
     * @return the predicted class value, or MISSING_VALUE if no
     * prediction was made.
     */
    virtual double predicted();

    /**
     * Gets the weight assigned to this prediction. This is typically the weight
     * of the test instance the prediction was made for.
     *
     * @return the weight assigned to this prediction.
     */
    virtual double weight();

    /**
    * Calculates the prediction margin. This is defined as the difference
    * between the probability predicted for the actual class and the highest
    * predicted probability of the other classes.
    *
    * @return the margin for this prediction, or
    * MISSING_VALUE if either the actual or predicted value
    * is missing.
    */
    virtual double margin();

    /**
    * Convert a single prediction into a probability distribution
    * with all zero probabilities except the predicted value which
    * has probability 1.0. If no prediction was made, all probabilities
    * are zero.
    *
    * @param predictedClass the index of the predicted class, or
    * MISSING_VALUE if no prediction was made.
    * @param numClasses the number of possible classes for this nominal
    * prediction.
    * @return the probability distribution.
    */
    static double_array makeDistribution(double predictedClass, int numClasses);

    /**
     * Creates a uniform probability distribution -- where each of the
     * possible classes is assigned equal probability.
     *
     * @param numClasses the number of possible classes for this nominal
     * prediction.
     * @return the probability distribution.
     */
    static double_array makeUniformDistribution(int numClasses);

    /**
     * Gets a human readable representation of this prediction.
     *
     * @return a human readable representation of this prediction.
     */
    virtual string toString();

};

#endif    //#ifndef _NOMINALPREDICTION_
