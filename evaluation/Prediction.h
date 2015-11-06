#ifndef _PREDICTION_
#define _PREDICTION_

#include <limits>
/// <summary>
/// Encapsulates a single evaluatable prediction: the predicted value plus the
/// actual class value.
///
/// </summary>
class Prediction {

  /// <summary>
  /// Constant representing a missing value. This should have the same value
  /// as weka.core.Instance.MISSING_VALUE
  /// </summary>
  public:
#ifdef _WIN64
	  static const double MISSING_VALUE;
#else
  static constexpr double MISSING_VALUE = std::numeric_limits<double>::quiet_NaN();
#endif

  /// <summary>
  /// Gets the weight assigned to this prediction. This is typically the weight
  /// of the test instance the prediction was made for.
  /// </summary>
  /// <returns> the weight assigned to this prediction. </returns>
  virtual double weight() = 0;

  /// <summary>
  /// Gets the actual class value.
  /// </summary>
  /// <returns> the actual class value, or MISSING_VALUE if no
  /// prediction was made.   </returns>
  virtual double actual() = 0;

  /// <summary>
  /// Gets the predicted class value.
  /// </summary>
  /// <returns> the predicted class value, or MISSING_VALUE if no
  /// prediction was made.   </returns>
  virtual double predicted() = 0;

};


#endif	//#ifndef _PREDICTION_
