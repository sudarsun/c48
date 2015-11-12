#ifndef _COSTMATRIX_
#define _COSTMATRIX_

#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

// Forward class declarations:
class Instances;
class InstanceExpression;
class Instance;

/// <summary>
/// Class for storing and manipulating a misclassification cost matrix. The
/// element at position i,j in the matrix is the penalty for classifying an
/// instance of class j as class i. Cost values can be fixed or computed on a
/// per-instance basis (cost sensitive evaluation only) from the value of an
/// attribute or a mathematical expression involving attribute(s).<br>
/// <br>
/// 
/// Values in an instance are accessed in an expression by prefixing their index
/// (starting at 1) with the character 'a'. E.g.<br>
/// <br>
/// 
/// a1 &circ; 2 * a5 / log(a7 * 4.0) <br>
/// 
/// Supported opperators: +, -, *, /, ^, log, abs, cos, exp, sqrt, floor, ceil,
/// rint, tan, sin, (, ).
/// 
/// </summary>
class CostMatrix {

  private:
  int m_size = 0;

  bool replaceStrings(Instances *dataset);
  /// <summary>
  /// [rows][columns] </summary>
  protected:
  std::vector<std::vector<void*>> m_matrix;

  /// <summary>
  /// The deafult file extension for cost matrix files </summary>
  public:
  static std::string FILE_EXTENSION;

  /// <summary>
  /// Creates a default cost matrix of a particular size. All diagonal values
  /// will be 0 and all non-diagonal values 1.
  /// </summary>
  /// <param name="numOfClasses"> the number of classes that the cost matrix holds. </param>
  CostMatrix( int numOfClasses );

  /// <summary>
  /// Creates a cost matrix that is a copy of another.
  /// </summary>
  /// <param name="toCopy"> the matrix to copy. </param>
  CostMatrix( CostMatrix *toCopy );

  /// <summary>
  /// Initializes the matrix
  /// </summary>
  void initialize();

  /// <summary>
  /// The number of rows (and columns)
  /// </summary>
  /// <returns> the size of the matrix </returns>
  int size();

  /// <summary>
  /// Same as size
  /// </summary>
  /// <returns> the number of columns </returns>
  int numColumns();

  /// <summary>
  /// Same as size
  /// </summary>
  /// <returns> the number of rows </returns>
  int numRows();

  /// <summary>
  /// Applies the cost matrix to a set of instances. If a random number generator
  /// is supplied the instances will be resampled, otherwise they will be
  /// rewighted. Adapted from code once sitting in Instances.java
  /// </summary>
  /// <param name="data"> the instances to reweight. </param>
  /// <param name="random"> a random number generator for resampling, if null then
  ///          instances are rewighted. </param>
  /// <returns> a new dataset reflecting the cost of misclassification. </returns>
  /// <exception cref="Exception"> if the data has no class or the matrix in
  ///              inappropriate. </exception>
 
  //Instances *applyCostMatrix( Instances *data, Random *random );

  /// <summary>
  /// Calculates the expected misclassification cost for each possible class
  /// value, given class probability estimates.
  /// </summary>
  /// <param name="classProbs"> the class probability estimates. </param>
  /// <returns> the expected costs. </returns>
  /// <exception cref="Exception"> if the wrong number of class probabilities is
  ///              supplied. </exception>
  std::vector<double> expectedCosts( std::vector<double> &classProbs );

  /// <summary>
  /// Calculates the expected misclassification cost for each possible class
  /// value, given class probability estimates.
  /// </summary>
  /// <param name="classProbs"> the class probability estimates. </param>
  /// <param name="inst"> the current instance for which the class probabilites apply. Is
  ///          used for computing any non-fixed cost values. </param>
  /// <returns> the expected costs. </returns>
  /// <exception cref="Exception"> if something goes wrong </exception>
  //std::vector<double> expectedCosts( std::vector<double> &classProbs, Instance *inst );

  /// <summary>
  /// Gets the maximum cost for a particular class value.
  /// </summary>
  /// <param name="classVal"> the class value. </param>
  /// <returns> the maximum cost. </returns>
  /// <exception cref="Exception"> if cost matrix contains non-fixed costs </exception>
  double getMaxCost( int classVal );

  /// <summary>
  /// Gets the maximum cost for a particular class value.
  /// </summary>
  /// <param name="classVal"> the class value. </param>
  /// <returns> the maximum cost. </returns>
  /// <exception cref="Exception"> if cost matrix contains non-fixed costs </exception>
  //double getMaxCost( int classVal, Instance *inst );

  /// <summary>
  /// Normalizes the matrix so that the diagonal contains zeros.
  /// 
  /// </summary>
  void normalize();

  /// <summary>
  /// Set the value of a particular cell in the matrix
  /// </summary>
  /// <param name="rowIndex"> the row </param>
  /// <param name="columnIndex"> the column </param>
  /// <param name="value"> the value to set </param>
  void setCell( int rowIndex, int columnIndex, void *value );

  /// <summary>
  /// Return the contents of a particular cell. Note: this method returns the
  /// Object stored at a particular cell.
  /// </summary>
  /// <param name="rowIndex"> the row </param>
  /// <param name="columnIndex"> the column </param>
  /// <returns> the value at the cell </returns>
  void *getCell( int rowIndex, int columnIndex );

  /// <summary>
  /// Return the value of a cell as a double (for legacy code)
  /// </summary>
  /// <param name="rowIndex"> the row </param>
  /// <param name="columnIndex"> the column </param>
  /// <returns> the value at a particular cell as a double </returns>
  /// <exception cref="Exception"> if the value is not a double </exception>
  double getElement( int rowIndex, int columnIndex );

  /// <summary>
  /// Return the value of a cell as a double. Computes the value for non-fixed
  /// costs using the supplied Instance
  /// </summary>
  /// <param name="rowIndex"> the row </param>
  /// <param name="columnIndex"> the column </param>
  /// <returns> the value from a particular cell </returns>
  /// <exception cref="Exception"> if something goes wrong </exception>
  double getElement( int rowIndex, int columnIndex, Instance *inst );

  /// <summary>
  /// Set the value of a cell as a double
  /// </summary>
  /// <param name="rowIndex"> the row </param>
  /// <param name="columnIndex"> the column </param>
  /// <param name="value"> the value (double) to set </param>
  void setElement( int rowIndex, int columnIndex, double value );
};


#endif	// _COSTMATRIX_
