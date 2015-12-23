#ifndef _ESTIMATOR_
#define _ESTIMATOR_

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "core/Typedefs.h"

//Forward class declarations:
class Instances;

/// 
/// <summary>
/// Abstract class for all estimators.
/// 
/// </summary>
class Estimator {


  /// <summary>
  /// Debugging mode </summary>
  private:
  bool m_Debug = false;

  /// <summary>
  /// The class value index is > -1 if subset is taken with specific class value only </summary>
  protected:
  double m_classValueIndex = -1.0;

  /// <summary>
  /// set if class is not important </summary>
  bool m_noClass = true;

  /// <summary>
  /// Class to support a building process of an estimator.
  /// </summary>
  private:
  class Builder {

    /// <summary>
    /// instances of the builder </summary>
public:
    Instances *m_instances = nullptr;

    /// <summary>
    /// attribute index of the builder </summary>
    int m_attrIndex = -1;

    /// <summary>
    /// class index of the builder, only relevant if class value index is set </summary>
    int m_classIndex = -1;

    /// <summary>
    /// class value index of the builder </summary>
    int m_classValueIndex = -1;

  };

  /// <summary>
  /// Add a new data value to the current estimator.
  /// </summary>
  /// <param name="data"> the new data value </param>
  /// <param name="weight"> the weight assigned to the data value  </param>
  public:
  virtual void addValue( double data, double weight );

  /// <summary>
  /// Initialize the estimator with a new dataset.
  /// Finds min and max first.
  /// </summary>
  /// <param name="data"> the dataset used to build this estimator </param>
  /// <param name="attrIndex"> attribute the estimator is for </param>
  /// <exception cref="Exception"> if building of estimator goes wrong </exception>
  virtual void addValues( Instances *data, int attrIndex );

  /// <summary>
  /// Initialize the estimator with all values of one attribute of a dataset. 
  /// Some estimator might ignore the min and max values.
  /// </summary>
  /// <param name="data"> the dataset used to build this estimator </param>
  /// <param name="attrIndex"> attribute the estimator is for </param>
  /// <param name="min"> minimal border of range </param>
  /// <param name="max"> maximal border of range </param>
  /// <param name="factor"> number of instances has been reduced to that factor </param>
  /// <exception cref="Exception"> if building of estimator goes wrong </exception>
  virtual void addValues( Instances *data, int attrIndex, double min, double max, double factor );

  /// <summary>
  /// Initialize the estimator using only the instance of one class. 
  /// It is using the values of one attribute only.
  /// </summary>
  /// <param name="data"> the dataset used to build this estimator </param>
  /// <param name="attrIndex"> attribute the estimator is for </param>
  /// <param name="classIndex"> index of the class attribute </param>
  /// <param name="classValue"> the class value </param>
  /// <exception cref="Exception"> if building of estimator goes wrong </exception>
  virtual void addValues( Instances *data, int attrIndex, int classIndex, int classValue );

  /// <summary>
  /// Initialize the estimator using only the instance of one class. 
  /// It is using the values of one attribute only.
  /// </summary>
  /// <param name="data"> the dataset used to build this estimator </param>
  /// <param name="attrIndex"> attribute the estimator is for </param>
  /// <param name="classIndex"> index of the class attribute </param>
  /// <param name="classValue"> the class value </param>
  /// <param name="min"> minimal value of this attribute </param>
  /// <param name="max"> maximal value of this attribute </param>
  /// <exception cref="Exception"> if building of estimator goes wrong </exception>
  virtual void addValues( Instances *data, int attrIndex, int classIndex, int classValue, double min, double max );


  /// <summary>
  /// Returns a dataset that contains all instances of a certain class value.
  /// </summary>
  /// <param name="data"> dataset to select the instances from </param>
  /// <param name="attrIndex"> index of the relevant attribute </param>
  /// <param name="classIndex"> index of the class attribute </param>
  /// <param name="classValue"> the relevant class value </param>
  /// <returns> a dataset with only  </returns>
  private:
  double getInstancesFromClass( Instances *data, int attrIndex, int classIndex, double classValue, Instances *workData );

  /// <summary>
  /// Get a probability estimate for a value.
  /// </summary>
  /// <param name="data"> the value to estimate the probability of </param>
  /// <returns> the estimated probability of the supplied value </returns>
  public:
  virtual double getProbability( double data ) = 0;

  /// <summary>
  /// Tests whether the current estimation object is equal to another
  /// estimation object
  /// </summary>
  /// <param name="obj"> the object to compare against </param>
  /// <returns> true if the two objects are equal </returns>
  virtual bool equals( void *obj );

 /// <summary>
 /// Set debugging mode.
 /// </summary>
 /// <param name="debug"> true if debug output should be printed </param>
  virtual void setDebug( bool debug );

  /// <summary>
  /// Get whether debugging is turned on.
  /// </summary>
  /// <returns> true if debugging output is on </returns>
  virtual bool getDebug();


};

#endif	//#ifndef _ESTIMATOR_
