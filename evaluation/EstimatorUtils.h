#ifndef _ESTIMATORUTILS_
#define _ESTIMATORUTILS_

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <stdexcept>

// Forward class declarations:
class Instances;
class Instance;
class Estimator;

/// <summary>
/// Contains static utility functions for Estimators.<para>
/// </para>
/// </summary>
class EstimatorUtils {

  /// <summary>
  /// Find the minimum distance between values </summary>
  /// <param name="inst"> sorted instances, sorted </param>
  /// <param name="attrIndex"> index of the attribute, they are sorted after </param>
  /// <returns> the minimal distance </returns>
  public:
  static double findMinDistance( Instances *inst, int attrIndex );

  /// <summary>
  /// Find the minimum and the maximum of the attribute and return it in 
  /// the last parameter.. </summary>
  /// <param name="inst"> instances used to build the estimator </param>
  /// <param name="attrIndex"> index of the attribute </param>
  /// <param name="minMax"> the array to return minimum and maximum in </param>
  /// <returns> number of not missing values </returns>
  /// <exception cref="Exception"> if parameter minMax wasn't initialized properly </exception>
  static int getMinMax( Instances *inst, int attrIndex, std::vector<double> &minMax ) ;

  /// <summary>
  /// Returns a dataset that contains all instances of a certain class value.
  /// </summary>
  /// <param name="data"> dataset to select the instances from </param>
  /// <param name="attrIndex"> index of the relevant attribute </param>
  /// <param name="classIndex"> index of the class attribute </param>
  /// <param name="classValue"> the relevant class value </param>
  /// <returns> a dataset with only  </returns>
  //static std::vector getInstancesFromClass( Instances *data, int attrIndex, int classIndex, double classValue, Instances *workData );


  /// <summary>
  /// Returns a dataset that contains of all instances of a certain class value. </summary>
  /// <param name="data"> dataset to select the instances from </param>
  /// <param name="classIndex"> index of the class attribute </param>
  /// <param name="classValue"> the class value </param>
  /// <returns> a dataset with only instances of one class value </returns>
  static Instances *getInstancesFromClass( Instances *data, int classIndex, double classValue );

  /// <summary>
  /// Returns a dataset that contains of all instances of a certain value
  /// for the given attribute. </summary>
  /// <param name="data"> dataset to select the instances from </param>
  /// <param name="index"> the index of the attribute </param>
  /// <param name="v"> the value </param>
  /// <returns> a subdataset with only instances of one value for the attribute  </returns>
  static Instances *getInstancesFromValue( Instances *data, int index, double v );


  /// <summary>
  /// Returns a string representing the cutpoints
  /// </summary>
  static std::string cutpointsToString( std::vector<double> &cutPoints, std::vector<bool> &cutAndLeft );
};


#endif	//#ifndef _ESTIMATORUTILS_
