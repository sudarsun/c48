#ifndef _MATRIX_
#define _MATRIX_

#include <string>
#include <vector>

class Matrix {

  /// <summary>
  /// Array for internal storage of elements.
  /// @serial internal array storage.
  /// </summary>
  protected:
  std::vector<std::vector<double>> A;

  /// <summary>
  /// Row and column dimensions.
  /// @serial row dimension.
  /// @serial column dimension.
  /// </summary>
  int m = 0;
  int n = 0;

  /// <summary>
  /// Construct an m-by-n matrix of zeros. </summary>
  /// <param name="m">    Number of rows. </param>
  /// <param name="n">    Number of colums. </param>
  public:
  Matrix( int m, int n );

  /// <summary>
  /// Construct an m-by-n constant matrix. </summary>
  /// <param name="m">    Number of rows. </param>
  /// <param name="n">    Number of colums. </param>
  /// <param name="s">    Fill the matrix with this scalar value. </param>
  Matrix( int m, int n, double s );

  /// <summary>
  /// Construct a matrix from a 2-D array. </summary>
  /// <param name="A">    Two-dimensional array of doubles. </param>
  /// <exception cref="IllegalArgumentException"> All rows must have the same length </exception>
  /// <seealso cref=        #constructWithCopy </seealso>
  Matrix( std::vector<std::vector<double>> &A );

  /// <summary>
  /// Construct a matrix quickly without checking arguments. </summary>
  /// <param name="A">    Two-dimensional array of doubles. </param>
  /// <param name="m">    Number of rows. </param>
  /// <param name="n">    Number of colums. </param>
  Matrix( std::vector<std::vector<double>> &A, int m, int n );

  /// <summary>
  /// Construct a matrix from a one-dimensional packed array </summary>
  /// <param name="vals"> One-dimensional array of doubles, packed by columns (ala
  /// Fortran). </param>
  /// <param name="m">    Number of rows. </param>
  /// <exception cref="IllegalArgumentException"> Array length must be a multiple of m. </exception>
  Matrix( std::vector<double> &vals, int m );

  /// <summary>
  /// Get a single element. </summary>
  /// <param name="i">    Row index. </param>
  /// <param name="j">    Column index. </param>
  /// <returns>     A(i,j) </returns>
  /// <exception cref="ArrayIndexOutOfBoundsException"> </exception>
  virtual double get( int i, int j );

  /**
  * Set a single element.
  * @param i    Row index.
  * @param j    Column index.
  * @param s    A(i,j).
  * @throws  ArrayIndexOutOfBoundsException
  */
  virtual void set(int i, int j, double s);

  /// <summary>
  /// Access the internal two-dimensional array. </summary>
  /// <returns>     Pointer to the two-dimensional array of matrix elements. </returns>
  virtual std::vector<std::vector<double>> getArray();

  /// <summary>
  /// Converts a matrix to a string.
  /// (FracPete: taken from old weka.core.Matrix class)
  /// </summary>
  /// <returns>    the converted string </returns>
  virtual std::string toString();

  virtual int getRowDimension();

  virtual int getColumnDimension();
};


#endif	//#ifndef _MATRIX_
