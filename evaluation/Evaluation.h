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
    Evaluation(Instances data);
    Evaluation(Instances data, CostMatrix costMatrix);
    ~Evaluation();

    void setPriors(Instances train);
    double evaluateModelOnceAndRecordPrediction(Classifier * classifier, Instance * instance);
    double evaluateModelOnceAndRecordPrediction(double_array dist, Instance *instance);
    string toSummaryString(bool printComplexityStatistics);
    string toSummaryString(string title, bool printComplexityStatistics);
    string toClassDetailsString(string title);
    string toClassDetailsString();
    string  toMatrixString();
    string  toMatrixString(string title);
    const double correct();
    const double totalCost();
    const double avgCost();
    const double kappa();
    const double inCorrect();
    const double pctCorrect();
    const double pctIncorrect();
    const double KBRelativeInformation();
    const double KBInformation();
    const double KBMeanInformation();
    const double correlationCoefficient();
    const double SFPriorEntropy();
    const double SFMeanPriorEntropy();
    const double SFSchemeEntropy();
    const double SFMeanSchemeEntropy();
    const double SFEntropyGain();
    const double SFMeanEntropyGain();
    const double meanAbsoluteError();
    const double rootMeanSquaredError();
    const double relativeAbsoluteError();
    const double rootRelativeSquaredError();
    const double unclassified();
    const double pctUnclassified();
    const double meanPriorAbsoluteError();
    const double priorEntropy();
    const double rootMeanPriorSquaredError();
    double truePositiveRate(int classIndex);
    double falsePositiveRate(int classIndex);
    double precision(int classIndex);
    double recall(int classIndex);
    double fMeasure(int classIndex);
    double areaUnderROC(int classIndex);
    double weightedTruePositiveRate();
    double weightedFalsePositiveRate();
    double weightedPrecision();
    double weightedRecall();
    double weightedFMeasure();
    double weightedAreaUnderROC();


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

    void updateStatsForClassifier(double_array predictedDistribution, Instance *instance);
    void updateStatsForPredictor(double predictedValue, Instance *instance);
    void updateMargins(double_array predictedDistribution, int actualClass, double weight);
    void updateNumericScores(double_array predicted, double_array actual, double weight);
    double_array makeDistribution(double predictedClass);

    /** Sets up the priors for numeric class attributes from the training class
     * values that have been seen so far.
     */
    void setNumericPriorsFromBuffer();

    void addNumericTrainClass(double classValue, double weight);

    /**
     * Helper method to convert the confusion matrix into string format
     */
    string num2ShortID(int num, char_array &IDChars, int IDWidth);

};

#endif // EVALUATION_H_
