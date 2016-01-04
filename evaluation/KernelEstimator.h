#ifndef _KERNELESTIMATOR_
#define _KERNELESTIMATOR_

#include "IncrementalEstimator.h"
#include "Estimator.h"
#include <string>
#include <vector>
#include <cmath>

/** 
 * Simple kernel density estimator. Uses one gaussian kernel per observed
 * data value.
 *
 */
class KernelEstimator : public Estimator, public IncrementalEstimator {


  /** Vector containing all of the values seen  */
  private:
  double_array mValues;

  /** Vector containing the associated weights  */
  double_array mWeights;

  /** Number of values stored in mWeights and mValues so far  */
  int mNumValues = 0;

  /** The sum of the weights so far  */
  double mSumOfWeights = 0;

  /** The standard deviation  */
  double mStandardDev = 0;

  /** The precision of data values  */
  double mPrecision = 0;

  /** Whether we can optimise the kernel summation  */
  bool mAllWeightsOne;

  /** Maximum percentage error permitted in probability calculations  */
  static double MAX_ERROR;


  /**
   * Execute a binary search to locate the nearest data value
   *
   * @param the data value to locate
   * @return the index of the nearest data value
   */
  int findNearestValue( double key );

  /**
   * Round a data value using the defined precision for this estimator
   *
   * @param data the value to round
   * @return the rounded data value
   */
  double round( double data );

  // ===============
  // Public methods.
  // ===============

  public:
  /**
   * Constructor that takes a precision argument.
   *
   * @param precision the  precision to which numeric values are given. For
   * example, if the precision is stated to be 0.1, the values in the
   * interval (0.25,0.35] are all treated as 0.3. 
   */
   KernelEstimator( double precision );

  /**
   * Add a new data value to the current estimator.
   *
   * @param data the new data value 
   * @param weight the weight assigned to the data value 
   */
  virtual void addValue( double data, double weight );

   /**
   * Get a probability estimate for a value.
   *
   * @param data the value to estimate the probability of
   * @return the estimated probability of the supplied value
   */
  virtual double getProbability( double data );

  /** Display a representation of this estimator */
  virtual string toString();

  /**
   * Return the number of kernels in this kernel estimator
   *
   * @return the number of kernels
   */
  virtual int getNumKernels();

  /**
   * Return the means of the kernels.
   *
   * @return the means of the kernels
   */
  virtual double_array getMeans();

   /**
   * Return the weights of the kernels.
   *
   * @return the weights of the kernels
   */
  virtual double_array getWeights();

  /**
   * Return the precision of this kernel estimator.
   *
   * @return the precision
   */
  virtual double getPrecision();

  /**
   * Return the standard deviation of this kernel estimator.
   *
   * @return the standard deviation
   */
  virtual double getStdDev();

};


#endif    //#ifndef _KERNELESTIMATOR_
