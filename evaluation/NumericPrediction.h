#ifndef _NUMERICPREDICTION_
#define _NUMERICPREDICTION_

#include "Prediction.h"
#include <string>
#include <vector>

/**
 * Encapsulates an evaluatable numeric prediction: the predicted class value
 * plus the actual class value.
 *
 */
class NumericPrediction : public Prediction {

private:

    /** The actual class value */
    double mActual = MISSING_VALUE;

    /** The predicted class value */
    double mPredicted = MISSING_VALUE;

    /** The weight assigned to this prediction. */
    double mWeight = 1;

public:
    /**
     * Creates the NumericPrediction object with a default weight of 1.0.
     *
     * @param actual the actual value, or MISSING_VALUE.
     * @param predicted the predicted value, or MISSING_VALUE.
     */
    NumericPrediction(double actual, double predicted);

    /**
     * Creates the NumericPrediction object.
     *
     * @param actual the actual value, or MISSING_VALUE.
     * @param predicted the predicted value, or MISSING_VALUE.
     * @param weight the weight assigned to the prediction.
     */
    NumericPrediction(double actual, double predicted, double weight);

    /**
     * Gets the actual class value.
     *
     * @return the actual class value, or MISSING_VALUE if no
     * prediction was made.
     */
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
     * Calculates the prediction error. This is defined as the predicted
     * value minus the actual value.
     *
     * @return the error for this prediction, or
     * MISSING_VALUE if either the actual or predicted value
     * is missing.
     */
    virtual double error();

    /**
     * Gets a human readable representation of this prediction.
     *
     * @return a human readable representation of this prediction.
     */
    virtual string toString();

};


#endif    //#ifndef _NUMERICPREDICTION_
