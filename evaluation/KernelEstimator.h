#ifndef _KERNELESTIMATOR_
#define _KERNELESTIMATOR_

#include "IncrementalEstimator.h"
#include "Estimator.h"
#include <string>
#include <vector>
#include <cmath>

/// <summary>
/// Simple kernel density estimator. Uses one gaussian kernel per observed
/// data value.
/// 
/// </summary>
class KernelEstimator : public Estimator, public IncrementalEstimator {


  /// <summary>
  /// Vector containing all of the values seen </summary>
  private:
  double_array m_Values;

  /// <summary>
  /// Vector containing the associated weights </summary>
  double_array m_Weights;

  /// <summary>
  /// Number of values stored in m_Weights and m_Values so far </summary>
  int m_NumValues = 0;

  /// <summary>
  /// The sum of the weights so far </summary>
  double m_SumOfWeights = 0;

  /// <summary>
  /// The standard deviation </summary>
  double m_StandardDev = 0;

  /// <summary>
  /// The precision of data values </summary>
  double m_Precision = 0;

  /// <summary>
  /// Whether we can optimise the kernel summation </summary>
  bool m_AllWeightsOne = false;

  /// <summary>
  /// Maximum percentage error permitted in probability calculations </summary>
  static double MAX_ERROR;


  /// <summary>
  /// Execute a binary search to locate the nearest data value
  /// </summary>
  /// <param name="the"> data value to locate </param>
  /// <returns> the index of the nearest data value </returns>
  int findNearestValue( double key );

  /// <summary>
  /// Round a data value using the defined precision for this estimator
  /// </summary>
  /// <param name="data"> the value to round </param>
  /// <returns> the rounded data value </returns>
  double round( double data );

  // ===============
  // Public methods.
  // ===============

  /// <summary>
  /// Constructor that takes a precision argument.
  /// </summary>
  /// <param name="precision"> the  precision to which numeric values are given. For
  /// example, if the precision is stated to be 0.1, the values in the
  /// interval (0.25,0.35] are all treated as 0.3.  </param>
  public:
  KernelEstimator( double precision );

  /// <summary>
  /// Add a new data value to the current estimator.
  /// </summary>
  /// <param name="data"> the new data value </param>
  /// <param name="weight"> the weight assigned to the data value  </param>
  virtual void addValue( double data, double weight );

  /// <summary>
  /// Get a probability estimate for a value.
  /// </summary>
  /// <param name="data"> the value to estimate the probability of </param>
  /// <returns> the estimated probability of the supplied value </returns>
  virtual double getProbability( double data );

  /// <summary>
  /// Display a representation of this estimator </summary>
  virtual string toString();

  /// <summary>
  /// Return the number of kernels in this kernel estimator
  /// </summary>
  /// <returns> the number of kernels </returns>
  virtual int getNumKernels();

  /// <summary>
  /// Return the means of the kernels.
  /// </summary>
  /// <returns> the means of the kernels </returns>
  virtual double_array getMeans();

  /// <summary>
  /// Return the weights of the kernels.
  /// </summary>
  /// <returns> the weights of the kernels </returns>
  virtual double_array getWeights();

  /// <summary>
  /// Return the precision of this kernel estimator.
  /// </summary>
  /// <returns> the precision </returns>
  virtual double getPrecision();

  /// <summary>
  /// Return the standard deviation of this kernel estimator.
  /// </summary>
  /// <returns> the standard deviation </returns>
  virtual double getStdDev();

};


#endif	//#ifndef _KERNELESTIMATOR_
