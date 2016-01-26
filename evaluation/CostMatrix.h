#ifndef _COSTMATRIX_
#define _COSTMATRIX_

#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include "core/Typedefs.h"

// Forward class declarations:
class Instances;
class Instance;

/**
 * Class for storing and manipulating a misclassification cost matrix. The
 * element at position i,j in the matrix is the penalty for classifying an
 * instance of class j as class i. Cost values can be fixed or computed on a
 * per-instance basis (cost sensitive evaluation only) from the value of an
 * attribute or a mathematical expression involving attribute(s).<br>
 * <br>
 *
 * Values in an instance are accessed in an expression by prefixing their index
 * (starting at 1) with the character 'a'. E.g.<br>
 * <br>
 *
 * a1 &circ; 2 * a5 / log(a7 * 4.0) <br>
 *
 * Supported opperators: +, -, *, /, ^, log, abs, cos, exp, sqrt, floor, ceil,
 * rint, tan, sin, (, ).
 *
 *
 *
 */
class CostMatrix {

protected:

    /** [rows][columns] */
    std::vector<std::vector<void*>> mMatrix;

public:

    /** The deafult file extension for cost matrix files */
    static string FILE_EXTENSION;

    /**
      * Creates a default cost matrix of a particular size. All diagonal values
      * will be 0 and all non-diagonal values 1.
      *
      * @param numOfClasses the number of classes that the cost matrix holds.
      */
    CostMatrix(int numOfClasses);

    /**
     * Creates a cost matrix that is a copy of another.
     *
     * @param toCopy the matrix to copy.
     */
    CostMatrix(CostMatrix *toCopy);

    /**
     * Initializes the matrix
     */
    void initialize();

    /**
     * The number of rows (and columns)
     *
     */
    int size() const;

    /**
     * Same as size
     *
     * @return the number of columns
     */
    int numColumns() const;

    /**
    * Same as size
    *
    * @return the number of rows
    */
    int numRows() const;

    /**
     * Calculates the expected misclassification cost for each possible class
     * value, given class probability estimates.
     *
     * @param classProbs the class probability estimates.
     * @return the expected costs.
     * @exception Exception if the wrong number of class probabilities is
     *              supplied.
     */
    double_array expectedCosts(const double_array &classProbs) const;

    /**
     * Gets the maximum cost for a particular class value.
     *
     * @param classVal the class value.
     * @return the maximum cost.
     * @exception Exception if cost matrix contains non-fixed costs
     */
    double getMaxCost(const int classVal) const;

    /**
     * Gets the maximum cost for a particular class value.
     *
     * @param classVal the class value.
     * @return the maximum cost.
     * @exception Exception if cost matrix contains non-fixed costs
     */
    double getMaxCost(const int classVal, Instance *inst) const;

    /**
     * Normalizes the matrix so that the diagonal contains zeros.
     *
     */
    void normalize();

    /**
    * Set the value of a particular cell in the matrix
    *
    * @param rowIndex the row
    * @param columnIndex the column
    * @param value the value to set
    */
    void setCell(const int rowIndex, const int columnIndex, void *value);

    /**
    * Return the contents of a particular cell. Note: this method returns the
    * Object stored at a particular cell.
    *
    * @param rowIndex the row
    * @param columnIndex the column
    * @return the value at the cell
    */
    void *getCell(const int rowIndex, const int columnIndex) const;

    /**
     * Return the value of a cell as a double (for legacy code)
     *
     * @param rowIndex the row
     * @param columnIndex the column
     * @return the value at a particular cell as a double
     * @exception Exception if the value is not a double
     */
    double getElement(const int rowIndex, const int columnIndex) const;

    /**
     * Return the value of a cell as a double. Computes the value for non-fixed
     * costs using the supplied Instance
     *
     * @param rowIndex the row
     * @param columnIndex the column
     * @return the value from a particular cell
     * @exception Exception if something goes wrong
     */
    double getElement(const int rowIndex, const int columnIndex, Instance *inst) const;

    /**
    * Set the value of a cell as a double
    *
    * @param rowIndex the row
    * @param columnIndex the column
    * @param value the value (double) to set
    */
    void setElement(const int rowIndex, const int columnIndex, double value);

private:
    int mSize = 0;
    bool replaceStrings(Instances *dataset) const;
};


#endif    // _COSTMATRIX_
