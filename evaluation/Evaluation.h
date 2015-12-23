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
//class AbstractEvaluationMetric;
class Classifier;
class CostMatrix;

/// <summary>
// Class for evaluating machine learning models.
/// </summary>
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
	string toSummaryString(string title,  bool printComplexityStatistics);
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
		/// <summary>
		/// The number of classes. </summary>
	int mNumClasses = 0;

	/// <summary>
	/// The number of folds for a cross-validation. </summary>
	int mNumFolds = 0;

	/// <summary>
	/// The weight of all incorrectly classified instances. </summary>
	double mIncorrect = 0;

	/// <summary>
	/// The weight of all correctly classified instances. </summary>
	double mCorrect = 0;

	/// <summary>
	/// The weight of all unclassified instances. </summary>
	double mUnclassified = 0;

	/// <summary>
	///* The weight of all instances that had no class assigned to them. </summary>
	double mMissingClass = 0;

	/// <summary>
	/// The weight of all instances that had a class assigned to them. </summary>
	double mWithClass = 0;

	/// <summary>
	/// Array for storing the confusion matrix. </summary>
	double_2D_array mConfusionMatrix;

	/// <summary>
	/// The names of the classes. </summary>
	string_array mClassNames;

	/// <summary>
	/// Is the class nominal or numeric? </summary>
	bool mClassIsNominal = false;

	/// <summary>
	/// The prior probabilities of the classes. </summary>
	double_array mClassPriors;

	/// <summary>
	/// The sum of counts for priors. </summary>
	double mClassPriorsSum = 0;

	/// <summary>
	/// The cost matrix (if given). </summary>
	CostMatrix *mCostMatrix;

	/// <summary>
	/// The total cost of predictions (includes instance weights). </summary>
	double mTotalCost = 0;

	/// <summary>
	/// Sum of errors. </summary>
	double mSumErr = 0;

	/// <summary>
	/// Sum of absolute errors. </summary>
	double mSumAbsErr = 0;

	/// <summary>
	/// Sum of squared errors. </summary>
	double mSumSqrErr = 0;

	/// <summary>
	/// Sum of class values. </summary>
	double mSumClass = 0;

	/// <summary>
	/// Sum of squared class values. </summary>
	double mSumSqrClass = 0;

	/// <summary>
	///* Sum of predicted values. </summary>
	double mSumPredicted = 0;

	/// <summary>
	/// Sum of squared predicted values. </summary>
	double mSumSqrPredicted = 0;

	/// <summary>
	/// Sum of predicted * class values. </summary>
	double mSumClassPredicted = 0;

	/// <summary>
	/// Sum of absolute errors of the prior. </summary>
	double mSumPriorAbsErr = 0;

	/// <summary>
	/// Sum of absolute errors of the prior. </summary>
	double mSumPriorSqrErr = 0;

	/// <summary>
	/// Total Kononenko & Bratko Information. </summary>
	double mSumKBInfo = 0;

	/// <summary>
	///* Resolution of the margin histogram. </summary>
	static int kMarginResolution;

	/// <summary>
	/// Cumulative margin distribution. </summary>
	double_array mMarginCounts;

	/// <summary>
	/// Number of non-missing class training instances seen. </summary>
	int mNumTrainClassVals = 0;

	/// <summary>
	/// Array containing all numeric training class values seen. </summary>
	double_array mTrainClassVals;

	/// <summary>
	/// Array containing all numeric training class weights. </summary>
	double_array mTrainClassWeights;

	/// <summary>
	/// Numeric class estimator for prior. </summary>
	Estimator *mPriorErrorEstimator;

	/// <summary>
	/// Numeric class estimator for scheme. </summary>
	Estimator *mErrorEstimator;

	/// <summary>
	/// Whether complexity statistics are available. </summary>
	bool mComplexityStatisticsAvailable = true;

	/// <summary>
	/// The minimum probablility accepted from an estimator to avoid taking log(0)
	/// in Sf calculations.
	/// </summary>
	static const double MIN_SF_PROB;

	/// <summary>
	/// Total entropy of prior predictions. </summary>
	double mSumPriorEntropy = 0;

	/// <summary>
	/// Total entropy of scheme predictions. </summary>
	double mSumSchemeEntropy = 0;

	/// <summary>
	/// Whether coverage statistics are available. </summary>
	bool mCoverageStatisticsAvailable = true;

	/// <summary>
	/// The confidence level used for coverage statistics. </summary>
	double mConfLevel = 0.95;

	/// <summary>
	/// Total size of predicted regions at the given confidence level. </summary>
	double mTotalSizeOfRegions = 0;

	/// <summary>
	/// Total coverage of test cases at the given confidence level. </summary>
	double mTotalCoverage = 0;

	/// <summary>
	/// Minimum target value. </summary>
	double mMinTarget = 0;

	/// <summary>
	/// Maximum target value. </summary>
	double mMaxTarget = 0;

	/// <summary>
	/// The list of predictions that have been generated (for computing AUC). </summary>
	std::vector<Prediction*> mPredictions;

	/// <summary>
	/// enables/disables the use of priors, e.g., if no training set is present in
	/// case of de-serialized schemes.
	/// </summary>
	bool mNoPriors = false;

	/// <summary>
	/// The header of the training set. </summary>
	Instances *mHeader;

	/// <summary>
	/// whether to discard predictions (and save memory). </summary>
	bool mDiscardPredictions = false;

	/// <summary>
	/// Holds plugin evaluation metrics </summary>
	//std::vector<AbstractEvaluationMetric*> mPluginMetrics;

	/// <summary>
	/// The list of metrics to display in the output </summary>
	string_array mmetricsToDisplay;

	//double evaluationForSingleInstance(double_array dist, Instance *instance, bool storePredictions);
	//double evaluationForSingleInstance(Classifier *classifier, Instance *instance, bool storePredictions);
	void updateStatsForClassifier(double_array predictedDistribution, Instance *instance);
	void updateStatsForPredictor(double predictedValue, Instance *instance);
	void updateMargins(double_array predictedDistribution, int actualClass, double weight);
	void updateNumericScores(double_array predicted, double_array actual, double weight);
	double_array makeDistribution(double predictedClass);
	/// <summary>
	/// Sets up the priors for numeric class attributes from the training class
	/// values that have been seen so far.
	/// </summary>
	void setNumericPriorsFromBuffer();
	void addNumericTrainClass(double classValue, double weight);
	string num2ShortID(int num, std::vector<char> &IDChars, int IDWidth);

};

#endif // EVALUATION_H_
