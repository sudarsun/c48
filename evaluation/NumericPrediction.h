#ifndef _NUMERICPREDICTION_
#define _NUMERICPREDICTION_

#include "Prediction.h"
#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class StringBuffer;

/// <summary>
/// Encapsulates an evaluatable numeric prediction: the predicted class value
/// plus the actual class value.
///
/// @author Len Trigg (len@reeltwo.com)
/// @version $Revision: 8034 $
/// </summary>
class NumericPrediction : public Prediction {


	/// <summary>
	/// The actual class value. </summary>
private:
	double mActual = MISSING_VALUE;

	/// <summary>
	/// The predicted class value. </summary>
	double mPredicted = MISSING_VALUE;

	/// <summary>
	/// The weight assigned to this prediction. </summary>
	double mWeight = 1;

	/// <summary>
	/// the prediction intervals. </summary>
	std::vector<std::vector<double>> mPredictionIntervals;

	/// <summary>
	/// Creates the NumericPrediction object with a default weight of 1.0.
	/// </summary>
	/// <param name="actual"> the actual value, or MISSING_VALUE. </param>
	/// <param name="predicted"> the predicted value, or MISSING_VALUE. </param>
public:
	NumericPrediction(double actual, double predicted);

	/// <summary>
	/// Creates the NumericPrediction object.
	/// </summary>
	/// <param name="actual"> the actual value, or MISSING_VALUE. </param>
	/// <param name="predicted"> the predicted value, or MISSING_VALUE. </param>
	/// <param name="weight"> the weight assigned to the prediction. </param>
	NumericPrediction(double actual, double predicted, double weight);

	/// <summary>
	/// Creates the NumericPrediction object.
	/// </summary>
	/// <param name="actual"> the actual value, or MISSING_VALUE. </param>
	/// <param name="predicted"> the predicted value, or MISSING_VALUE. </param>
	/// <param name="weight"> the weight assigned to the prediction. </param>
	/// <param name="predInt"> the prediction intervals from classifiers implementing
	/// the <code>IntervalEstimator</code> interface. </param>
	/// <seealso cref= IntervalEstimator </seealso>
	NumericPrediction(double actual, double predicted, double weight, std::vector<std::vector<double>> &predInt);

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
	/// Calculates the prediction error. This is defined as the predicted
	/// value minus the actual value.
	/// </summary>
	/// <returns> the error for this prediction, or
	/// MISSING_VALUE if either the actual or predicted value
	/// is missing.   </returns>
	virtual double error();

	/// <summary>
	/// Sets the prediction intervals for this prediction.
	/// </summary>
	/// <param name="predInt"> the prediction intervals </param>
	virtual void setPredictionIntervals(std::vector<std::vector<double>> &predInt);

	/// <summary>
	/// Returns the predictions intervals. Only classifiers implementing the
	/// <code>IntervalEstimator</code> interface.
	/// </summary>
	/// <returns> the prediction intervals. </returns>
	/// <seealso cref= IntervalEstimator </seealso>
	virtual std::vector<std::vector<double>> predictionIntervals();

	/// <summary>
	/// Gets a human readable representation of this prediction.
	/// </summary>
	/// <returns> a human readable representation of this prediction. </returns>
	virtual std::string toString();

};


#endif	//#ifndef _NUMERICPREDICTION_
