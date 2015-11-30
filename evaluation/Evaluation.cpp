#include "Evaluation.h"

class UnivariateKernelEstimator;
class AbstractEvaluationMetric;

#include "CostMatrix.h"
#include "Prediction.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "c48/Classifier.h"
#include "core/Utils.h"
#include "NominalPrediction.h"
#include "Estimator.h"
#include "KernelEstimator.h"
#include <exception>

Evaluation::Evaluation()
{
    //ctor
}

Evaluation::~Evaluation()
{
    //dtor
}

Evaluation::Evaluation(Instances data)
{

}

Evaluation::Evaluation(Instances data, CostMatrix costMatrix)
{

}

std::vector<double> Evaluation::evaluateModel(Classifier *classifier, Instances *data)
{
	return { 0.0 };
}

void Evaluation::setPriors(Instances train)
{

}

double Evaluation::evaluateModelOnceAndRecordPrediction(Classifier *classifier, Instances *data)
{
	return 0.0;
}

double Evaluation::evaluateModelOnceAndRecordPrediction(std::vector<double> dist, Instance *instance)
{
	return 0.0;
}
double Evaluation::evaluateModelOnceAndRecordPrediction(Classifier *classifier, Instance *instance)
{
	Instance *classMissing = instance;
	double pred = 0;
	//classMissing.setDataset(instance.dataset());
	//classMissing.setClassMissing();
	if (mClassIsNominal) {
		std::vector<double> dist = classifier->distributionForInstance(classMissing);
		pred = Utils::maxIndex(dist);
		if (dist[(int)pred] <= 0) {
			pred = instance->missingValue();
		}
		updateStatsForClassifier(dist, instance);
		mPredictions.push_back(new NominalPrediction(instance->classValue(), dist, instance->weight()));
	}
	else {
		pred = classifier->classifyInstance(classMissing);
		updateStatsForPredictor(pred, instance);
	}
	return 0.0;
}

double Evaluation::evaluationForSingleInstance(Classifier * classifier, Instance *instance, bool storePredictions)
{
	return 0.0;
}

double Evaluation::evaluationForSingleInstance(std::vector<double> dist, Instance *instance, bool storePredictions)
{
	return 0.0;
}

void Evaluation::updateStatsForClassifier(std::vector<double> predictedDistribution, Instance *instance)
{
	int actualClass = (int)instance->classValue();

	if (!instance->classIsMissing()) {
		updateMargins(predictedDistribution, actualClass, instance->weight());

		// Determine the predicted class (doesn't detect multiple
		// classifications)
		int predictedClass = -1;
		double bestProb = 0.0;
		for (int i = 0; i < mNumClasses; i++) {
			if (predictedDistribution[i] > bestProb) {
				predictedClass = i;
				bestProb = predictedDistribution[i];
			}
		}

		mWithClass += instance->weight();

		// Determine misclassification cost
		if (mCostMatrix != nullptr) {
			if (predictedClass < 0) {
				// For missing predictions, we assume the worst possible cost.
				// This is pretty harsh.
				// Perhaps we could take the negative of the cost of a correct
				// prediction (-mCostMatrix.getElement(actualClass,actualClass)),
				// although often this will be zero
				mTotalCost += instance->weight() * mCostMatrix->getMaxCost(actualClass, instance);
			}
			else {
				mTotalCost += instance->weight()
					* mCostMatrix->getElement(actualClass, predictedClass, instance);
			}
		}

		// Update counts when no class was predicted
		if (predictedClass < 0) {
			mUnclassified += instance->weight();
			return;
		}

		double predictedProb =
			std::max(MIN_SF_PROB, predictedDistribution[actualClass]);
		double priorProb =
			std::max(MIN_SF_PROB, mClassPriors[actualClass] / mClassPriorsSum);
		if (predictedProb >= priorProb) {
			mSumKBInfo +=
				(Utils::getLog2(predictedProb) - Utils::getLog2(priorProb))
				* instance->weight();
		}
		else {
			mSumKBInfo -=
				(Utils::getLog2(1.0 - predictedProb) - Utils::getLog2(1.0 - priorProb))
				* instance->weight();
		}

		mSumSchemeEntropy -= Utils::getLog2(predictedProb) * instance->weight();
		mSumPriorEntropy -= Utils::getLog2(priorProb) * instance->weight();

		updateNumericScores(predictedDistribution, makeDistribution(instance->classValue()), instance->weight());

		// Update other stats
		mConfusionMatrix[actualClass][predictedClass] += instance->weight();
		if (predictedClass != actualClass) {
			mIncorrect += instance->weight();
		}
		else {
			mCorrect += instance->weight();
		}
	}
	else {
		mMissingClass += instance->weight();
	}
}

void Evaluation::updateStatsForPredictor(double predictedValue, Instance *instance)
{
	if (!instance->classIsMissing()) {

		// Update stats
		mWithClass += instance->weight();
		if (Utils::isMissingValue(predictedValue)) {
			mUnclassified += instance->weight();
			return;
		}
		mSumClass += instance->weight() * instance->classValue();
		mSumSqrClass +=
			instance->weight() * instance->classValue() * instance->classValue();
		mSumClassPredicted +=
			instance->weight() * instance->classValue() * predictedValue;
		mSumPredicted += instance->weight() * predictedValue;
		mSumSqrPredicted += instance->weight() * predictedValue * predictedValue;

		if (mErrorEstimator == nullptr) {
			setNumericPriorsFromBuffer();
		}
		double predictedProb =
			std::max(mErrorEstimator->getProbability(predictedValue - instance->classValue()), MIN_SF_PROB);
		double priorProb =
			std::max(mPriorErrorEstimator->getProbability(instance->classValue()), MIN_SF_PROB);

		mSumSchemeEntropy -= Utils::getLog2(predictedProb) * instance->weight();
		mSumPriorEntropy -= Utils::getLog2(priorProb) * instance->weight();
		mErrorEstimator->addValue(predictedValue - instance->classValue(), instance->weight());

		updateNumericScores(makeDistribution(predictedValue),
			makeDistribution(instance->classValue()), instance->weight());

	}
	else {
		mMissingClass += instance->weight();
	}
}

void Evaluation::updateMargins(std::vector<double> predictedDistribution, int actualClass,
	double weight) {

	double probActual = predictedDistribution[actualClass];
	double probNext = 0;

	for (int i = 0; i < mNumClasses; i++) {
		if ((i != actualClass) && (predictedDistribution[i] > probNext)) {
			probNext = predictedDistribution[i];
		}
	}

	double margin = probActual - probNext;
	int bin = (int)((margin + 1.0) / 2.0 * kMarginResolution);
	mMarginCounts[bin] += weight;
}

void Evaluation::updateNumericScores(std::vector<double> predicted, std::vector<double> actual, double weight)
{
	double diff;
	double sumErr = 0, sumAbsErr = 0, sumSqrErr = 0;
	double sumPriorAbsErr = 0, sumPriorSqrErr = 0;
	for (int i = 0; i < mNumClasses; i++) {
		diff = predicted[i] - actual[i];
		sumErr += diff;
		sumAbsErr += std::abs(diff);
		sumSqrErr += diff * diff;
		diff = (mClassPriors[i] / mClassPriorsSum) - actual[i];
		sumPriorAbsErr += std::abs(diff);
		sumPriorSqrErr += diff * diff;
	}
	mSumErr += weight * sumErr / mNumClasses;
	mSumAbsErr += weight * sumAbsErr / mNumClasses;
	mSumSqrErr += weight * sumSqrErr / mNumClasses;
	mSumPriorAbsErr += weight * sumPriorAbsErr / mNumClasses;
	mSumPriorSqrErr += weight * sumPriorSqrErr / mNumClasses;
}

std::vector<double> Evaluation::makeDistribution(double predictedClass)
{
	std::vector<double> result = std::vector<double>(mNumClasses);
	if (Utils::isMissingValue(predictedClass)) {
		return result;
	}
	if (mClassIsNominal) {
		result[(int)predictedClass] = 1.0;
	}
	else {
		result[0] = predictedClass;
	}
	return result;
}

void Evaluation::setNumericPriorsFromBuffer() {

	double numPrecision = 0.01; // Default value
	if (mNumTrainClassVals > 1) {
		std::vector<double> temp(mNumTrainClassVals);
		std::copy(mTrainClassVals.begin(), mTrainClassVals.begin()+mNumTrainClassVals, temp);
		std::vector<int> index = Utils::sort(temp);
		double lastVal = temp[index[0]];
		double deltaSum = 0;
		int distinct = 0;
		for (int i = 1; i < temp.size(); i++) {
			double current = temp[index[i]];
			if (current != lastVal) {
				deltaSum += current - lastVal;
				lastVal = current;
				distinct++;
			}
		}
		if (distinct > 0) {
			numPrecision = deltaSum / distinct;
		}
	}
	mPriorErrorEstimator = new KernelEstimator(numPrecision);
	mErrorEstimator = new KernelEstimator(numPrecision);
	mClassPriors[0] = mClassPriorsSum = 0;
	for (int i = 0; i < mNumTrainClassVals; i++) {
		mClassPriors[0] += mTrainClassVals[i] * mTrainClassWeights[i];
		mClassPriorsSum += mTrainClassWeights[i];
		mPriorErrorEstimator->addValue(mTrainClassVals[i], mTrainClassWeights[i]);
	}
}

std::string Evaluation::toSummaryString(bool printComplexityStatistics)
{
	return toSummaryString("=== Summary ===\n", printComplexityStatistics);
}

std::string Evaluation::toSummaryString(std::string title, bool printComplexityStatistics)
{
	std::string text = "";

	if (printComplexityStatistics && mNoPriors) {
		printComplexityStatistics = false;
		std::cout << "Priors disabled, cannot print complexity statistics!";
	}
	text.append(title + "\n");
	try {
		if (mWithClass > 0) {
			if (mClassIsNominal) {

				text.append("Correctly Classified Instances     ");
				text.append(Utils::doubleToString(correct(), 12, 4) + "     "
					+ Utils::doubleToString(pctCorrect(), 12, 4) + " %\n");
				text.append("Incorrectly Classified Instances   ");
				text.append(Utils::doubleToString(inCorrect(), 12, 4) + "     "
					+ Utils::doubleToString(pctIncorrect(), 12, 4) + " %\n");
				text.append("Kappa statistic                    ");
				text.append(Utils::doubleToString(kappa(), 12, 4) + "\n");

				if (mCostMatrix != nullptr) {
					text.append("Total Cost                         ");
					text.append(Utils::doubleToString(totalCost(), 12, 4) + "\n");
					text.append("Average Cost                       ");
					text.append(Utils::doubleToString(avgCost(), 12, 4) + "\n");
				}
				if (printComplexityStatistics) {
					text.append("K&B Relative Info Score            ");
					text.append(Utils::doubleToString(KBRelativeInformation(), 12, 4)
						+ " %\n");
					text.append("K&B Information Score              ");
					text.append(Utils::doubleToString(KBInformation(), 12, 4) + " bits");
					text.append(Utils::doubleToString(KBMeanInformation(), 12, 4)
						+ " bits/instance\n");
				}
			}
			else {
				text.append("Correlation coefficient            ");
				text.append(Utils::doubleToString(correlationCoefficient(), 12, 4)
					+ "\n");
			}
			if (printComplexityStatistics) {
				text.append("Class complexity | order 0         ");
				text.append(Utils::doubleToString(SFPriorEntropy(), 12, 4) + " bits");
				text.append(Utils::doubleToString(SFMeanPriorEntropy(), 12, 4)
					+ " bits/instance\n");
				text.append("Class complexity | scheme          ");
				text.append(Utils::doubleToString(SFSchemeEntropy(), 12, 4) + " bits");
				text.append(Utils::doubleToString(SFMeanSchemeEntropy(), 12, 4)
					+ " bits/instance\n");
				text.append("Complexity improvement     (Sf)    ");
				text.append(Utils::doubleToString(SFEntropyGain(), 12, 4) + " bits");
				text.append(Utils::doubleToString(SFMeanEntropyGain(), 12, 4)
					+ " bits/instance\n");
			}

			text.append("Mean absolute error                ");
			text.append(Utils::doubleToString(meanAbsoluteError(), 12, 4) + "\n");
			text.append("Root mean squared error            ");
			text.append(Utils::doubleToString(rootMeanSquaredError(), 12, 4) + "\n");
			if (!mNoPriors) {
				text.append("Relative absolute error            ");
				text.append(Utils::doubleToString(relativeAbsoluteError(), 12, 4)
					+ " %\n");
				text.append("Root relative squared error        ");
				text.append(Utils::doubleToString(rootRelativeSquaredError(), 12, 4)
					+ " %\n");
			}
		}
		if (Utils::gr(unclassified(), 0)) {
			text.append("UnClassified Instances             ");
			text.append(Utils::doubleToString(unclassified(), 12, 4) + "     "
				+ Utils::doubleToString(pctUnclassified(), 12, 4) + " %\n");
		}
		text.append("Total Number of Instances          ");
		text.append(Utils::doubleToString(mWithClass, 12, 4) + "\n");
		if (mMissingClass > 0) {
			text.append("Ignored Class Unknown Instances            ");
			text.append(Utils::doubleToString(mMissingClass, 12, 4) + "\n");
		}
	}
	catch (std::exception ex)
	{
		std::cout << "Arggh - Must be a bug in Evaluation class" << ex.what() << std::endl;
	}
	return text;
}

const double Evaluation::correct()
{
	return mCorrect;
}

const double Evaluation::totalCost()
{
	return mTotalCost;
}

const double Evaluation::avgCost()
{
	return mTotalCost/mWithClass;
}

const double Evaluation::kappa()
{
	std::vector<double> sumRows = std::vector<double>(mConfusionMatrix.size());
	std::vector<double> sumColumns = std::vector<double>(mConfusionMatrix.size());
	double sumOfWeights = 0;
	for (int i = 0; i < mConfusionMatrix.size(); i++) {
		for (int j = 0; j < mConfusionMatrix.size(); j++) {
			sumRows[i] += mConfusionMatrix[i][j];
			sumColumns[j] += mConfusionMatrix[i][j];
			sumOfWeights += mConfusionMatrix[i][j];
		}
	}
	double correct = 0, chanceAgreement = 0;
	for (int i = 0; i < mConfusionMatrix.size(); i++) {
		chanceAgreement += (sumRows[i] * sumColumns[i]);
		correct += mConfusionMatrix[i][i];
	}
	chanceAgreement /= (sumOfWeights * sumOfWeights);
	correct /= sumOfWeights;

	if (chanceAgreement < 1) {
		return (correct - chanceAgreement) / (1 - chanceAgreement);
	}
	else {
		return 1;
	}
}

const double Evaluation::inCorrect()
{
	return mIncorrect;
}

const double Evaluation::pctCorrect()
{
	return 100 * mCorrect / mWithClass;;
}
const double Evaluation::pctIncorrect()
{
	return 100 * mIncorrect / mWithClass;;
}
