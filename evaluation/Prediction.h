#ifndef _PREDICTION_
#define _PREDICTION_

#include <limits>
#include "core/Typedefs.h"

/**
 * Encapsulates a single evaluatable prediction: the predicted value plus the
 * actual class value.
 *
 */
class Prediction {

public:
    /**
     * Constant representing a missing value.
     */
#ifdef _WIN64
    static const double MISSING_VALUE;
#else
    static constexpr double MISSING_VALUE = std::numeric_limits<double>::quiet_NaN();
#endif

    /**
    * Gets the weight assigned to this prediction. This is typically the weight
    * of the test instance the prediction was made for.
    *
    * @return the weight assigned to this prediction.
    */
    virtual double weight() = 0;

    /**
     * Gets the actual class value.
     *
     * @return the actual class value, or MISSING_VALUE if no
     * prediction was made.
     */
    virtual double actual() = 0;

    /**
     * Gets the predicted class value.
     *
     * @return the predicted class value, or MISSING_VALUE if no
     * prediction was made.
     */
    virtual double predicted() = 0;

};


#endif    //#ifndef _PREDICTION_
