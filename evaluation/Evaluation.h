#ifndef EVALUATION_H_
#define EVALUATION_H_

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class CostMatrix;
class UnivariateKernelEstimator;
class Prediction;
class Instances;
class AbstractEvaluationMetric;
class Classifier;

class Evaluation
{
public:
	Evaluation();
	~Evaluation();
	double[] evaluateModel(Classifier classifier, Instances data);
	double evaluateModelOnceAndRecordPrediction(Classifier classifier, Instances data);
	double evaluateModelOnceAndRecordPrediction(double[] dist, Instance instance);
	double evaluationForSingleInstance(double[] dist, Instance instance, bool storePredictions);
protected:
	double evaluationForSingleInstance(Classifier classifier, Instance instance, bool storePredictions)

		/// <summary>
		/// The number of classes. </summary>
protected:
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
	std::vector<std::vector<double>> mConfusionMatrix;

	/// <summary>
	/// The names of the classes. </summary>
	std::vector<std::string> mClassNames;

	/// <summary>
	/// Is the class nominal or numeric? </summary>
	bool mClassIsNominal = false;

	/// <summary>
	/// The prior probabilities of the classes. </summary>
	std::vector<double> mClassPriors;

	/// <summary>
	/// The sum of counts for priors. </summary>
	double mClassPriorsSum = 0;

	/// <summary>
	/// The cost matrix (if given). </summary>
	//CostMatrix *mCostMatrix;

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
	static int k_MarginResolution;

	/// <summary>
	/// Cumulative margin distribution. </summary>
	std::vector<double> mMarginCounts;

	/// <summary>
	/// Number of non-missing class training instances seen. </summary>
	int mNumTrainClassVals = 0;

	/// <summary>
	/// Array containing all numeric training class values seen. </summary>
	std::vector<double> mTrainClassVals;

	/// <summary>
	/// Array containing all numeric training class weights. </summary>
	std::vector<double> mTrainClassWeights;

	/// <summary>
	/// Numeric class estimator for prior. </summary>
	UnivariateKernelEstimator *mPriorEstimator;

	/// <summary>
	/// Whether complexity statistics are available. </summary>
	bool mComplexityStatisticsAvailable = true;

	/// <summary>
	/// The minimum probablility accepted from an estimator to avoid taking log(0)
	/// in Sf calculations.
	/// </summary>
	static const double MIN_SF_PROB = Prediction::MISSING_VALUE;

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
	std::vector<AbstractEvaluationMetric*> mpluginMetrics;

	/// <summary>
	/// The list of metrics to display in the output </summary>
	std::vector<std::wstring> mmetricsToDisplay;


};

#endif // EVALUATION_H_
