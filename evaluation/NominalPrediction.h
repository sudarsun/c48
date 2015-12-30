#ifndef _NOMINALPREDICTION_
#define _NOMINALPREDICTION_

#include "Prediction.h"
#include <string>
#include <vector>

/// <summary>
/// Encapsulates an evaluatable nominal prediction: the predicted probability
/// distribution plus the actual class value.
/// 
/// </summary>
class NominalPrediction : public Prediction {

    /// <summary>
    /// The predicted probabilities </summary>
private:
    double_array mDistribution;

    /// <summary>
    /// The actual class value </summary>
    double mActual = MISSING_VALUE;

    /// <summary>
    /// The predicted class value </summary>
    double mPredicted = MISSING_VALUE;

    /// <summary>
    /// The weight assigned to this prediction </summary>
    double mWeight = 1;

    /// <summary>
    /// Creates the NominalPrediction object with a default weight of 1.0.
    /// </summary>
    /// <param name="actual"> the actual value, or MISSING_VALUE. </param>
    /// <param name="distribution"> the predicted probability distribution. Use 
    /// NominalPrediction.makeDistribution() if you only know the predicted value. </param>
public:
    NominalPrediction(double actual, double_array &distribution);

    /// <summary>
    /// Creates the NominalPrediction object.
    /// </summary>
    /// <param name="actual"> the actual value, or MISSING_VALUE. </param>
    /// <param name="distribution"> the predicted probability distribution. Use 
    /// NominalPrediction.makeDistribution() if you only know the predicted value. </param>
    /// <param name="weight"> the weight assigned to the prediction. </param>
    NominalPrediction(double actual, double_array &distribution, double weight);

    /// <summary>
    /// Gets the predicted probabilities
    /// </summary>
    /// <returns> the predicted probabilities </returns>
    virtual double_array distribution();

    /// <summary>
    /// Gets the actual class value.
    /// </summary>
    /// <returns> the actual class value, or MISSING_VALUE if no
    /// prediction was made.   </returns>
    virtual double actual();

    /// <summary>
    /// Gets the predicted class value.
    /// </summary>
    /// <returns> the predicted class value, or MISSING_VALUE if no
    /// prediction was made.   </returns>
    virtual double predicted();

    /// <summary>
    /// Gets the weight assigned to this prediction. This is typically the weight
    /// of the test instance the prediction was made for.
    /// </summary>
    /// <returns> the weight assigned to this prediction. </returns>
    virtual double weight();

    /// <summary>
    /// Calculates the prediction margin. This is defined as the difference
    /// between the probability predicted for the actual class and the highest
    /// predicted probability of the other classes.
    /// </summary>
    /// <returns> the margin for this prediction, or
    /// MISSING_VALUE if either the actual or predicted value
    /// is missing.   </returns>
    virtual double margin();

    /// <summary>
    /// Convert a single prediction into a probability distribution
    /// with all zero probabilities except the predicted value which
    /// has probability 1.0. If no prediction was made, all probabilities
    /// are zero.
    /// </summary>
    /// <param name="predictedClass"> the index of the predicted class, or 
    /// MISSING_VALUE if no prediction was made. </param>
    /// <param name="numClasses"> the number of possible classes for this nominal
    /// prediction. </param>
    /// <returns> the probability distribution.   </returns>
    static double_array makeDistribution(double predictedClass, int numClasses);

    /// <summary>
    /// Creates a uniform probability distribution -- where each of the
    /// possible classes is assigned equal probability.
    /// </summary>
    /// <param name="numClasses"> the number of possible classes for this nominal
    /// prediction. </param>
    /// <returns> the probability distribution.   </returns>
    static double_array makeUniformDistribution(int numClasses);

    /// <summary>
    /// Determines the predicted class (doesn't detect multiple 
    /// classifications). If no prediction was made (i.e. all zero
    /// probababilities in the distribution), mPrediction is set to
    /// MISSING_VALUE.
    /// </summary>
private:
    void updatePredicted();

    /// <summary>
    /// Gets a human readable representation of this prediction.
    /// </summary>
    /// <returns> a human readable representation of this prediction. </returns>
public:
    virtual string toString();

};



#endif    //#ifndef _NOMINALPREDICTION_
