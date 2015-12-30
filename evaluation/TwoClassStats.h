#ifndef _TWOCLASSSTATS_
#define _TWOCLASSSTATS_

#include <string>
#include <vector>
#include "core/Typedefs.h"

// Forward class declarations:
class ConfusionMatrix;

/**
 * Encapsulates performance functions for two-class problems.
 *
 */
class TwoClassStats {

private:

    /** The names used when converting this object to a confusion matrix */
    static string_array CATEGORY_NAMES;

    /** Pos predicted as pos */
    double mTruePos = 0;

    /** Neg predicted as pos */
    double mFalsePos = 0;

    /** Neg predicted as neg */
    double mTrueNeg = 0;

    /** Pos predicted as neg */
    double mFalseNeg = 0;

public:

    /**
     * Creates the TwoClassStats with the given initial performance values.
     *
     * @param tp the number of correctly classified positives
     * @param fp the number of incorrectly classified negatives
     * @param tn the number of correctly classified negatives
     * @param fn the number of incorrectly classified positives
     */
    TwoClassStats(double tp, double fp, double tn, double fn);

    /** Sets the number of positive instances predicted as positive */
    virtual void setTruePositive(double tp);

    /** Sets the number of negative instances predicted as positive */
    virtual void setFalsePositive(double fp);

    /** Sets the number of negative instances predicted as negative */
    virtual void setTrueNegative(double tn);

    /** Sets the number of positive instances predicted as negative */
    virtual void setFalseNegative(double fn);

    /** Gets the number of positive instances predicted as positive  */
    virtual double getTruePositive();

    /** Gets the number of negative instances predicted as positive  */
    virtual double getFalsePositive();

    /** Gets the number of negative instances predicted as negative  */
    virtual double getTrueNegative();

    /** Gets the number of positive instances predicted as negative  */
    virtual double getFalseNegative();

    /**
     * Calculate the true positive rate. This is defined as
     *
     * correctly classified positives
     * ------------------------------
     *       total positives
     *
     *
     * @return the true positive rate
     */
    virtual double getTruePositiveRate();

    /**
    * Calculate the false positive rate. This is defined as
    *
    * incorrectly classified negatives
    * --------------------------------
    *        total negatives
    *
    * @return the false positive rate
    */
    virtual double getFalsePositiveRate();

    /**
     * Calculate the precision. This is defined as
     *
     * correctly classified positives
     * ------------------------------
     *  total predicted as positive
     *
     * @return the precision
     */
    virtual double getPrecision();

    /**
     * Calculate the recall. This is defined as
     * <p>
     *
     * <pre>
     * correctly classified positives
     * ------------------------------
     *       total positives
     * (Which is also the same as the truePositiveRate.)
     *
     * @return the recall
     */
    virtual double getRecall();

    /**
     * Calculate the F-Measure. This is defined as
     *
     * 2 * recall * precision
     * ----------------------
     *   recall + precision
     *
     *
     * @return the F-Measure
     */
    virtual double getFMeasure();

    /**
     * Calculate the fallout. This is defined as
     *
     * incorrectly classified negatives
     * --------------------------------
     *   total predicted as positive
     *
     * @return the fallout
     */
    virtual double getFallout();

    /**
     * Generates a <code>ConfusionMatrix</code> representing the current two-class
     * statistics, using class names "negative" and "positive".
     *
     * @return a <code>ConfusionMatrix</code>.
     */
    virtual ConfusionMatrix *getConfusionMatrix();

    /**
     * Returns a string containing the various performance measures for the
     * current object
     */
    virtual string toString();
};


#endif    //#ifndef _TWOCLASSSTATS_
