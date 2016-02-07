#ifndef EVALUATION_H_
#define EVALUATION_H_

#include <string>
#include <vector>
#include "core/Typedefs.h"

// Forward class declarations:
class CostMatrix;
class Estimator;
class Prediction;
class Instances;
class Instance;
class Classifier;
class CostMatrix;

/**
 * Class for evaluating machine learning models.
 */
class Evaluation
{
public:

    /**
     * Initializes all the counters for the evaluation.
     *
     * @param data set of training instances, to get some header information and
     *          prior class distribution information
     * @throws Exception if the class is not defined
     */
    Evaluation(Instances data);

    /**
     * Initializes all the counters for the evaluation and also takes a cost
     * matrix as parameter.
     *
     * @param data set of training instances, to get some header information and
     *          prior class distribution information
     * @param costMatrix the cost matrix---if null, default costs will be used
     * @throws Exception if cost matrix is not compatible with data, the class is
     *           not defined or the class is numeric
     */
    Evaluation(Instances data, CostMatrix costMatrix);

    /**
     * Destructor.
     *
     */
    ~Evaluation();

    /**
     * Set the instance for evaluation
     * @param instance the train instance to be classified
     */
    void setPriors(Instances &train);

    /**
     * Evaluates the classifier on a single instance and records the prediction
     * (if the class is nominal).
     *
     * @param classifier machine learning classifier
     * @param instance the test instance to be classified
     * @return the prediction made by the clasifier
     * @throws Exception if model could not be evaluated successfully or the data
     *           contains string attributes
     */
    double evaluateModelOnceAndRecordPrediction(Classifier &classifier, Instance &instance);

    /**
     * Evaluates the supplied distribution on a single instance.
     *
     * @param dist the supplied distribution
     * @param instance the test instance to be classified
     * @return the prediction
     * @throws Exception if model could not be evaluated successfully
     */
    double evaluateModelOnceAndRecordPrediction(const double_array dist, Instance &instance);

    /**
     * Calls toSummaryString() with a default title.
     *
     * @param printComplexityStatistics if true, complexity statistics are
     *          returned as well
     * @return the summary string
     */
    string toSummaryString(const bool printComplexityStatistics) const;

    /**
     * Outputs the performance statistics in summary form. Lists number (and
     * percentage) of instances classified correctly, incorrectly and
     * unclassified. Outputs the total number of instances classified, and the
     * number of instances (if any) that had no class value provided.
     *
     * @param title the title for the statistics
     * @param printComplexityStatistics if true, complexity statistics are
     *          returned as well
     * @return the summary as a String
     */
    string toSummaryString(const string &title, bool printComplexityStatistics) const;

    /**
      * Generates a breakdown of the accuracy for each class, incorporating various
      * information-retrieval statistics, such as true/false positive rate,
      * precision/recall/F-Measure. Should be useful for ROC curves,
      * recall/precision curves.
      *
      * @param title the title to prepend the stats string with
      * @return the statistics presented as a string
      * @throws Exception if class is not nominal
      */
    string toClassDetailsString(const string &title) const;

    /**
     * Generates a breakdown of the accuracy for each class (with default title),
     * incorporating various information-retrieval statistics, such as true/false
     * positive rate, precision/recall/F-Measure. Should be useful for ROC curves,
     * recall/precision curves.
     *
     * @return the statistics presented as a string
     * @throws Exception if class is not nominal
     */
    string toClassDetailsString() const;

    /**
     * Calls toMatrixString() with a default title.
     *
     * @return the confusion matrix as a string
     * @throws Exception if the class is numeric
     */
    string  toMatrixString() const;

    /**
     * Outputs the performance statistics as a classification confusion matrix.
     * For each class value, shows the distribution of predicted class values.
     *
     * @param title the title for the confusion matrix
     * @return the confusion matrix as a String
     * @throws Exception if the class is numeric
     */
    string  toMatrixString(const string &title) const;

    /**
     * Gets the number of instances correctly classified (that is, for which a
     * correct prediction was made). (Actually the sum of the weights of these
     * instances)
     *
     * @return the number of correctly classified instances
     */
    const double correct() const;

    /**
     * Gets the total cost, that is, the cost of each prediction times the weight
     * of the instance, summed over all instances.
     *
     * @return the total cost
     */
    const double totalCost() const;

    /**
     * Gets the average cost, that is, total cost of misclassifications (incorrect
     * plus unclassified) over the total number of instances.
     *
     * @return the average cost.
     */
    const double avgCost() const;

    /**
     * Returns value of kappa statistic if class is nominal.
     *
     * @return the value of the kappa statistic
     */
    const double kappa() const;

    /**
     * Gets the number of instances incorrectly classified (that is, for which an
     * incorrect prediction was made). (Actually the sum of the weights of these
     * instances)
     *
     * @return the number of incorrectly classified instances
     */
    const double inCorrect() const;

    /**
     * Gets the percentage of instances incorrectly classified (that is, for which
     * an incorrect prediction was made).
     *
     * @return the percent of incorrectly classified instances (between 0 and 100)
     */
    const double pctCorrect() const;

    /**
     * Gets the percentage of instances incorrectly classified (that is, for which
     * an incorrect prediction was made).
     *
     * @return the percent of incorrectly classified instances (between 0 and 100)
     */
    const double pctIncorrect() const;

    /**
     * Return the Kononenko & Bratko Relative Information score
     *
     * @return the K&B relative information score
     * @throws Exception if the class is not nominal
     */
    const double KBRelativeInformation() const;

    /**
     * Return the total Kononenko & Bratko Information score in bits
     *
     * @return the K&B information score
     * @throws Exception if the class is not nominal
     */
    const double KBInformation() const;

    /**
     * Return the Kononenko & Bratko Information score in bits per instance.
     *
     * @return the K&B information score
     * @throws Exception if the class is not nominal
     */
    const double KBMeanInformation() const;

    /**
     * Returns the correlation coefficient if the class is numeric.
     *
     * @return the correlation coefficient
     * @throws Exception if class is not numeric
     */
    const double correlationCoefficient() const;

    /**
     * Returns the total entropy for the null model
     *
     * @return the total null model entropy
     */
    const double SFPriorEntropy() const;

    /**
     * Returns the entropy per instance for the null model
     *
     * @return the null model entropy per instance
     */
    const double SFMeanPriorEntropy() const;

    /**
     * Returns the total entropy for the scheme
     *
     * @return the total scheme entropy
     */
    const double SFSchemeEntropy() const;

    /**
     * Returns the entropy per instance for the scheme
     *
     * @return the scheme entropy per instance
     */
    const double SFMeanSchemeEntropy() const;

    /**
     * Returns the total SF, which is the null model entropy minus the scheme
     * entropy.
     *
     * @return the total SF
     */
    const double SFEntropyGain() const;

    /**
     * Returns the SF per instance, which is the null model entropy minus the
     * scheme entropy, per instance.
     *
     * @return the SF per instance
     */
    const double SFMeanEntropyGain() const;

    /**
     * Returns the mean absolute error. Refers to the error of the predicted
     * values for numeric classes, and the error of the predicted probability
     * distribution for nominal classes.
     *
     * @return the mean absolute error
     */
    const double meanAbsoluteError() const;

    /**
     * Returns the root mean squared error.
     *
     * @return the root mean squared error
     */
    const double rootMeanSquaredError() const;

    /**
     * Returns the relative absolute error.
     *
     * @return the relative absolute error
     * @throws Exception if it can't be computed
     */
    const double relativeAbsoluteError() const;

    /**
     * Returns the root relative squared error if the class is numeric.
     *
     * @return the root relative squared error
     */
    const double rootRelativeSquaredError() const;

    /**
     * Gets the number of instances not classified (that is, for which no
     * prediction was made by the classifier). (Actually the sum of the weights of
     * these instances)
     *
     * @return the number of unclassified instances
     */
    const double unclassified() const;

    /**
     * Gets the percentage of instances not classified (that is, for which no
     * prediction was made by the classifier).
     *
     * @return the percent of unclassified instances (between 0 and 100)
     */
    const double pctUnclassified() const;

    /**
    * Returns the mean absolute error of the prior.
    *
    * @return the mean absolute error
    */
    const double meanPriorAbsoluteError() const;

    /**
     * Calculate the entropy of the prior distribution
     *
     * @return the entropy of the prior distribution
     * @throws Exception if the class is not nominal
     */
    const double priorEntropy() const;

    /**
     * Returns the root mean prior squared error.
     *
     * @return the root mean prior squared error
     */
    const double rootMeanPriorSquaredError() const;

    /**
     * Calculate the true positive rate with respect to a particular class. This
     * is defined as
     *
     * correctly classified positives
     * ------------------------------
     *       total positives
     *
     * @param classIndex the index of the class to consider as "positive"
     * @return the true positive rate
     */
    double truePositiveRate(const int classIndex) const;

    /**
     * Calculate the false positive rate with respect to a particular class. This
     * is defined as
     *
     *
     *
     * incorrectly classified negatives
     * --------------------------------
     *        total negatives
     *
     *
     * @param classIndex the index of the class to consider as "positive"
     * @return the false positive rate
     */
    double falsePositiveRate(const int classIndex) const;

    /**
     * Calculate the precision with respect to a particular class. This is defined
     * as
     *
     * correctly classified positives
     * ------------------------------
     *  total predicted as positive
     *
     * @param classIndex the index of the class to consider as "positive"
     * @return the precision
     */
    double precision(const int classIndex) const;

    /**
     * Calculate the recall with respect to a particular class. This is defined as
     *
     * correctly classified positives
     * ------------------------------
     *       total positives

     * (Which is also the same as the truePositiveRate.)
     *
     * @param classIndex the index of the class to consider as "positive"
     * @return the recall
     */
    double recall(const int classIndex) const;

    /**
     * Calculate the F-Measure with respect to a particular class. This is defined
     * as
     *
     * 2 * recall * precision
     * ----------------------
     *   recall + precision
     *
     * @param classIndex the index of the class to consider as "positive"
     * @return the F-Measure
     */
    double fMeasure(const int classIndex) const;

    /**
     * Returns the area under ROC for those predictions that have been collected
     * in the evaluateClassifier(Classifier, Instances) method. Returns
     * Instance.missingValue() if the area is not available.
     *
     * @param classIndex the index of the class to consider as "positive"
     * @return the area under the ROC curve or not a number
     */
    double areaUnderROC(const int classIndex) const;

    /**
     * Calculates the weighted (by class size) true positive rate.
     *
     * @return the weighted true positive rate.
     */
    double weightedTruePositiveRate() const;
    /**
     * Calculates the weighted (by class size) false positive rate.
     *
     * @return the weighted false positive rate.
     */
    double weightedFalsePositiveRate() const;
    /**
     * Calculates the weighted (by class size) precision.
     *
     * @return the weighted precision.
     */
    double weightedPrecision() const;

    /**
    * Calculates the weighted (by class size) recall.
    *
    * @return the weighted recall.
    */
    double weightedRecall() const;

    /**
     * Calculates the weighted (by class size) F-Measure.
     *
     * @return the weighted F-Measure.
     */
    double weightedFMeasure() const;

    /**
     * Calculates the weighted AreaUnderROC.
     *
     * @return the weighted AreaUnderROC.
     */
    double weightedAreaUnderROC() const;

protected:
    /** The number of classes.  */
    int mNumClasses = 0;

    /** The number of folds for a cross-validation.  */
    int mNumFolds = 0;

    /** The weight of all incorrectly classified instances.  */
    double mIncorrect = 0;

    /** The weight of all correctly classified instances.  */
    double mCorrect = 0;

    /** The weight of all unclassified instances.  */
    double mUnclassified = 0;

    /** The weight of all instances that had no class assigned to them.  */
    double mMissingClass = 0;

    /** The weight of all instances that had a class assigned to them.  */
    double mWithClass = 0;

    /** Array for storing the confusion matrix.  */
    double_2D_array mConfusionMatrix;

    /** The names of the classes.  */
    string_array mClassNames;

    /** Is the class nominal or numeric?  */
    bool mClassIsNominal;

    /** The prior probabilities of the classes.  */
    double_array mClassPriors;

    /** The sum of counts for priors.  */
    double mClassPriorsSum = 0;

    /** The cost matrix (if given).  */
    CostMatrix *mCostMatrix;

    /** The total cost of predictions (includes instance weights).  */
    double mTotalCost = 0;

    /** Sum of errors.  */
    double mSumErr = 0;

    /** Sum of absolute errors.  */
    double mSumAbsErr = 0;

    /** Sum of squared errors.  */
    double mSumSqrErr = 0;

    /** Sum of class values.  */
    double mSumClass = 0;

    /** Sum of squared class values.  */
    double mSumSqrClass = 0;

    /*** Sum of predicted values.  */
    double mSumPredicted = 0;

    /** Sum of squared predicted values.  */
    double mSumSqrPredicted = 0;

    /** Sum of predicted * class values.  */
    double mSumClassPredicted = 0;

    /** Sum of absolute errors of the prior.  */
    double mSumPriorAbsErr = 0;

    /** Sum of absolute errors of the prior.  */
    double mSumPriorSqrErr = 0;

    /** Total Kononenko & Bratko Information.  */
    double mSumKBInfo = 0;

    /*** Resolution of the margin histogram.  */
    static int kMarginResolution;

    /** Cumulative margin distribution.  */
    double_array mMarginCounts;

    /** Number of non-missing class training instances seen.  */
    int mNumTrainClassVals = 0;

    /** Array containing all numeric training class values seen.  */
    double_array mTrainClassVals;

    /** Array containing all numeric training class weights.  */
    double_array mTrainClassWeights;

    /** Numeric class estimator for prior.  */
    Estimator *mPriorErrorEstimator;

    /** Numeric class estimator for scheme.  */
    Estimator *mErrorEstimator;

    /** Whether complexity statistics are available.  */
    bool mComplexityStatisticsAvailable = true;

    /**
     * The minimum probablility accepted from an estimator to avoid taking log(0)
     * in Sf calculations.
     */
    static const double MIN_SF_PROB;

    /** Total entropy of prior predictions.  */
    double mSumPriorEntropy = 0;

    /** Total entropy of scheme predictions.  */
    double mSumSchemeEntropy = 0;

    /** Whether coverage statistics are available.  */
    bool mCoverageStatisticsAvailable = true;

    /** The confidence level used for coverage statistics.  */
    double mConfLevel = 0.95;

    /** Total size of predicted regions at the given confidence level.  */
    double mTotalSizeOfRegions = 0;

    /** Total coverage of test cases at the given confidence level.  */
    double mTotalCoverage = 0;

    /** Minimum target value.  */
    double mMinTarget = 0;

    /** Maximum target value.  */
    double mMaxTarget = 0;

    /** The list of predictions that have been generated (for computing AUC).  */
    std::vector<Prediction*> mPredictions;

    /** enables/disables the use of priors, e.g., if no training set is present in
     * case of de-serialized schemes.
     */
    bool mNoPriors;

    /** The header of the training set.  */
    Instances *mHeader;

    /** whether to discard predictions (and save memory).  */
    bool mDiscardPredictions;

    /** The list of metrics to display in the output  */
    string_array mmetricsToDisplay;

    /**
     * Updates all the statistics about a classifiers performance for the current
     * test instance.
     *
     * @param predictedDistribution the probabilities assigned to each class
     * @param instance the instance to be classified
     * @throws Exception if the class of the instance is not set
     */
    void updateStatsForClassifier(const double_array predictedDistribution, Instance &instance);
    /**
     * Updates all the statistics about a predictors performance for the current
     * test instance.
     *
     * @param predictedValue the numeric value the classifier predicts
     * @param instance the instance to be classified
     * @throws Exception if the class of the instance is not set
     */
    void updateStatsForPredictor(const double predictedValue, Instance &instance);

    /**
     * Update the cumulative record of classification margins
     *
     * @param predictedDistribution the probability distribution predicted for the
     *          current instance
     * @param actualClass the index of the actual instance class
     * @param weight the weight assigned to the instance
     */
    void updateMargins(const double_array predictedDistribution, const int actualClass, const double weight);

    /**
     * Update the numeric accuracy measures. For numeric classes, the accuracy is
     * between the actual and predicted class values. For nominal classes, the
     * accuracy is between the actual and predicted class probabilities.
     *
     * @param predicted the predicted values
     * @param actual the actual value
     * @param weight the weight associated with this prediction
     */
    void updateNumericScores(double_array predicted, double_array actual, double weight);

    /**
     * Convert a single prediction into a probability distribution with all zero
     * probabilities except the predicted value which has probability 1.0;
     *
     * @param predictedClass the index of the predicted class
     * @return the probability distribution
     */
    double_array makeDistribution(const double predictedClass);

    /** Sets up the priors for numeric class attributes from the training class
     * values that have been seen so far.
     */
    void setNumericPriorsFromBuffer();

    /**
     * Adds a numeric (non-missing) training class value and weight to the buffer
     * of stored values.
     *
     * @param classValue the class value
     * @param weight the instance weight
     */
    void addNumericTrainClass(const double classValue, const double weight);

    /**
     * Method for generating indices for the confusion matrix.
     *
     * @param num integer to format
     * @param IDChars the characters to use
     * @param IDWidth the width of the entry
     * @return the formatted integer as a string
     */
    string num2ShortID(int num, const char_array &IDChars, const int IDWidth) const;

};

#endif // EVALUATION_H_
