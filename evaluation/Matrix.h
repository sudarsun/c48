#ifndef _MATRIX_
#define _MATRIX_

#include <string>
#include <vector>
#include "core/Typedefs.h"

/**
 *
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
    double_2D_array A;

    /**
     * Row and column dimensions.
     * @serial row dimension.
     * @serial column dimension.
     */
    int m = 0, n = 0;

public:
    /**
     * Construct an m-by-n matrix of zeros.
     * @param m    Number of rows.
     * @param n    Number of colums.
     */
    Matrix(int m, int n);

    /**
     * Construct an m-by-n constant matrix.
     * @param m    Number of rows.
     * @param n    Number of colums.
     * @param s    Fill the matrix with this scalar value.
     */
    Matrix(int m, int n, double s);

    /**
    * Construct a matrix from a 2-D array.
    * @param A    Two-dimensional array of doubles.
    * @throws  IllegalArgumentException All rows must have the same length
    * @see        #constructWithCopy
    */
    Matrix(double_2D_array &A);

    /**
     * Construct a matrix quickly without checking arguments.
     * @param A    Two-dimensional array of doubles.
     * @param m    Number of rows.
     * @param n    Number of colums.
     */
    Matrix(double_2D_array &A, int m, int n);

    /**
     * Construct a matrix from a one-dimensional packed array
     * @param vals One-dimensional array of doubles, packed by columns (ala
     * Fortran).
     * @param m    Number of rows.
     * @throws  IllegalArgumentException Array length must be a multiple of m.
     */
    Matrix(double_array &vals, int m);

    /**
    * Get a single element.
    * @param i    Row index.
    * @param j    Column index.
    * @return     A(i,j)
    * @throws  ArrayIndexOutOfBoundsException
    */
    virtual double get(int i, int j);

    /**
     * Set a single element.
     * @param i    Row index.
     * @param j    Column index.
     * @param s    A(i,j).
     * @throws  ArrayIndexOutOfBoundsException
     */
    virtual void set(int i, int j, double s);

    /**
     * Access the internal two-dimensional array.
     * @return     Pointer to the two-dimensional array of matrix elements.
     */
    virtual double_2D_array getArray();

    /**
     * Converts a matrix to a string.
     *
     * @return    the converted string
     */
    virtual string toString();

    /**
     * Get row dimension.
     * @return     m, the number of rows.
     */
    virtual int getRowDimension();

    /**
     * Get column dimension.
     * @return     n, the number of columns.
     */
    virtual int getColumnDimension();
};


#endif    //#ifndef _MATRIX_
