#ifndef _MATRIX_
#define _MATRIX_

#include <string>
#include <vector>
#include "core/Typedefs.h"

/**
 * The Matrix Class provides the fundamental operations of numerical linear
 * algebra.  Various constructors create Matrices from two dimensional arrays of
 * double precision floating point numbers.  Various "gets" and "sets" provide
 * access to submatrices and matrix elements.
 */
class Matrix {

protected:
    /**
     * Array for internal storage of elements.
     * @serial internal array storage.
     */
    double_2D_array mElements;

    /**
     * Row and column dimensions.
     * @serial row dimension.
     * @serial column dimension.
     */
    int mRow = 0, mCol = 0;

public:
    /**
     * Construct an m-by-n matrix of zeros.
     * @param row    Number of rows.
     * @param col    Number of colums.
     */
    Matrix(const int row, const int col);

    /**
     * Construct an m-by-n constant matrix.
     * @param row    Number of rows.
     * @param col    Number of colums.
     * @param scalarValue    Fill the matrix with this scalar value.
     */
    Matrix(const int row, const int col, const double scalarValue);

    /**
    * Construct a matrix from a 2-D array.
    * @param element    Two-dimensional array of doubles.
    * @throws  IllegalArgumentException All rows must have the same length
    * @see        #constructWithCopy
    */
    Matrix(double_2D_array &element);

    /**
     * Construct a matrix quickly without checking arguments.
     * @param A    Two-dimensional array of doubles.
     * @param row    Number of rows.
     * @param col    Number of colums.
     */
    Matrix(double_2D_array &A, const int row, const int col);

    /**
     * Construct a matrix from a one-dimensional packed array
     * @param vals One-dimensional array of doubles, packed by columns (ala
     * Fortran).
     * @param row    Number of rows.
     * @throws  IllegalArgumentException Array length must be a multiple of m.
     */
    Matrix(double_array &vals, const int row);

    /**
    * Get a single element.
    * @param i    Row index.
    * @param j    Column index.
    * @return     A(i,j)
    * @throws  ArrayIndexOutOfBoundsException
    */
    virtual double get(const int i, const int j) const;

    /**
     * Set a single element.
     * @param i    Row index.
     * @param j    Column index.
     * @param s    A(i,j).
     * @throws  ArrayIndexOutOfBoundsException
     */
    virtual void set(const int i, const int j, double const s);

    /**
     * Access the internal two-dimensional array.
     * @return     Pointer to the two-dimensional array of matrix elements.
     */
    virtual double_2D_array getArray() const;

    /**
     * Converts a matrix to a string.
     *
     * @return    the converted string
     */
    virtual string toString() const;

    /**
     * Get row dimension.
     * @return     m, the number of rows.
     */
    virtual int getRowDimension() const;

    /**
     * Get column dimension.
     * @return     n, the number of columns.
     */
    virtual int getColumnDimension() const;
};


#endif    //#ifndef _MATRIX_
