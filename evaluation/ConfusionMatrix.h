#ifndef _CONFUSIONMATRIX_
#define _CONFUSIONMATRIX_

#include "Matrix.h"
#include "Prediction.h"
#include "NominalPrediction.h"
#include <string>
#include <vector>

// Forward class declarations:
class CostMatrix;
class TwoClassStats;

/**
 * Cells of this matrix correspond to counts of the number (or weight) of
 * predictions for each actual value / predicted value combination.
 *
 */
class ConfusionMatrix : public Matrix {

protected:
    /** Stores the names of the classes */
    string_array mClassNames;

public:
    /**
     * Creates the confusion matrix with the given class names.
     *
     * @param classNames an array containing the names the classes.
     */
    ConfusionMatrix(string_array classNames);

    /**
    * Makes a copy of this ConfusionMatrix after applying the supplied CostMatrix
    * to the cells. The resulting ConfusionMatrix can be used to get
    * cost-weighted statistics.
    *
    * @param costs the CostMatrix.
    * @return a ConfusionMatrix that has had costs applied.
    * @exception Exception if the CostMatrix is not of the same size as this
    *              ConfusionMatrix.
    */
    virtual ConfusionMatrix *makeWeighted(CostMatrix *costs);

    /**
    * Gets the number of classes.
    *
    * @return the number of classes
    */
    virtual int size();

    /**
     * Gets the name of one of the classes.
     *
     * @param index the index of the class.
     * @return the class name.
     */
    virtual string className(int index);

    /**
    * Includes a prediction in the confusion matrix.
    *
    * @param pred the NominalPrediction to include
    * @exception Exception if no valid prediction was made (i.e. unclassified).
    */
    virtual void addPrediction(NominalPrediction *pred);

    /**
     * Includes a whole bunch of predictions in the confusion matrix.
     *
     * @param predictions a FastVector containing the NominalPredictions to
     *          include
     * @exception Exception if no valid prediction was made (i.e. unclassified).
     */
    virtual void addPredictions(std::vector<Prediction*> &predictions);

    /**
      * Gets the performance with respect to one of the classes as a TwoClassStats
      * object.
      *
      * @param classIndex the index of the class of interest.
      * @return the generated TwoClassStats object.
      */
    virtual TwoClassStats *getTwoClassStats(int classIndex);

    /**
     * Gets the number of correct classifications (that is, for which a correct
     * prediction was made). (Actually the sum of the weights of these
     * classifications)
     *
     * @return the number of correct classifications
     */
    virtual double correct();

    /**
     * Gets the number of incorrect classifications (that is, for which an
     * incorrect prediction was made). (Actually the sum of the weights of these
     * classifications)
     *
     * @return the number of incorrect classifications
     */
    virtual double incorrect();

    /**
     * Gets the number of predictions that were made (actually the sum of the
     * weights of predictions where the class value was known).
     *
     * @return the number of predictions with known class
     */
    virtual double total();

    /**
     * Returns the estimated error rate.
     *
     * @return the estimated error rate (between 0 and 1).
     */
    virtual double errorRate();

    /**
     * Calls toString() with a default title.
     *
     * @return the confusion matrix as a string
     */
    virtual string toString();

    /**
     * Outputs the performance statistics as a classification confusion matrix.
     * For each class value, shows the distribution of predicted class values.
     *
     * @param title the title for the confusion matrix
     * @return the confusion matrix as a String
     */
    virtual string toString(const string &title);

    /**
    * Get a single element.
    *
    * @param i Row index.
    * @param j Column index.
    * @return A(i,j)
    * @throws ArrayIndexOutOfBoundsException
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
     *
     * @return Pointer to the two-dimensional array of matrix elements.
     */
    virtual double_2D_array getArray();

    /**
     * Get row dimension.
     *
     * @return m, the number of rows.
     *
     */
    virtual int getRowDimension();
    /**
     * Get column dimension.
     *
     * @return n, the number of columns.
     */
    virtual int getColumnDimension();

private:

    /**
     * Method for generating indices for the confusion matrix.
     *
     * @param num integer to format
     * @param IDChars the characters to use
     * @param IDWidth the width of the entry
     * @return the formatted integer as a string
     */
    static string num2ShortID(int num, char_array &IDChars, int IDWidth);

};

#endif    //#ifndef _CONFUSIONMATRIX_
