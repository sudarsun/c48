#include "Evaluation.h"

#include "CostMatrix.h"
#include "Prediction.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "c48/Classifier.h"
#include "core/Utils.h"
#include "NominalPrediction.h"
#include "Estimator.h"
#include "KernelEstimator.h"
#include "ThresholdCurve.h"
#include <exception>

const double Evaluation::MIN_SF_PROB = std::numeric_limits<double>::min();
int Evaluation::kMarginResolution = 500;

Evaluation::Evaluation(Instances data) :Evaluation(data, nullptr)
{

}

Evaluation::Evaluation(Instances data, CostMatrix costMatrix)
{
    mNumClasses = data.numClasses();
    mNumFolds = 1;
    mClassIsNominal = data.classAttribute().isNominal();

    if (mClassIsNominal) {
        mConfusionMatrix.resize(mNumClasses, double_array(mNumClasses, 0));
        mClassNames = string_array(mNumClasses);
        for (int i = 0; i < mNumClasses; i++) {
            mClassNames[i] = data.classAttribute().value(i);
        }
    }
    mCostMatrix = &costMatrix;
    if (mCostMatrix->size() != 0) {
        if (!mClassIsNominal) {
            throw "Class has to be nominal if cost matrix given!";
        }
        if (mCostMatrix->size() != mNumClasses) {
            throw "Cost matrix not compatible with data!";
        }
    }
    mClassPriors = double_array(mNumClasses);
    setPriors(data);
    mMarginCounts = double_array(kMarginResolution + 1);

}

Evaluation::~Evaluation()
{
    //dtor
}

void Evaluation::setPriors(Instances &train)
{
    mNoPriors = false;

    if (!mClassIsNominal) {

        mNumTrainClassVals = 0;
        mTrainClassVals.clear();
        mTrainClassWeights.clear();
        mPriorErrorEstimator = nullptr;
        mErrorEstimator = nullptr;

        for (int i = 0; i < train.numInstances(); i++) {
            Instance &currentInst = train.instance(i);
            if (!currentInst.classIsMissing()) {
                addNumericTrainClass(currentInst.classValue(), currentInst.weight());
            }
        }
    }
    else {
        for (int i = 0; i < mNumClasses; i++) {
            mClassPriors[i] = 1;
        }
        mClassPriorsSum = mNumClasses;
        for (int i = 0; i < train.numInstances(); i++) {
            if (!train.instance(i).classIsMissing()) {
                mClassPriors[(int)train.instance(i).classValue()] +=
                    train.instance(i).weight();
                mClassPriorsSum += train.instance(i).weight();
            }
        }
    }
}

double Evaluation::evaluateModelOnceAndRecordPrediction(const double_array dist, Instance &instance)
{
    double pred = 0;
    if (mClassIsNominal) {
        pred = Utils::maxIndex(dist);
        if (dist[(int)pred] <= 0) {
            pred = instance.missingValue();
        }
        updateStatsForClassifier(dist, instance);
        mPredictions.push_back(new NominalPrediction(instance.classValue(), dist, instance.weight()));
    }
    else {
        pred = dist[0];
        updateStatsForPredictor(pred, instance);
    }
    return pred;
}
double Evaluation::evaluateModelOnceAndRecordPrediction(Classifier &classifier, Instance &instance)
{
    Instance *classMissing = instance.copy();
    double pred = 0;
    classMissing->setDataset(instance.getDataset());
    classMissing->setClassMissing();
    if (mClassIsNominal) {
        double_array dist = classifier.distributionForInstance(*classMissing);
        pred = Utils::maxIndex(dist);
        if (dist[(int)pred] <= 0) {
            pred = instance.missingValue();
        }
        updateStatsForClassifier(dist, instance);
        mPredictions.push_back(new NominalPrediction(instance.classValue(), dist, instance.weight()));
    }
    else {
        pred = classifier.classifyInstance(*classMissing);
        updateStatsForPredictor(pred, instance);
    }
    return pred;
}

void Evaluation::updateStatsForClassifier(const double_array predictedDistribution, Instance &instance)
{
    int actualClass = (int)instance.classValue();

    if (!instance.classIsMissing()) {
        updateMargins(predictedDistribution, actualClass, instance.weight());

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

        mWithClass += instance.weight();

        // Determine misclassification cost
        if (mCostMatrix->size() != 0) {
            if (predictedClass < 0) {
                // For missing predictions, we assume the worst possible cost.
                // This is pretty harsh.
                // Perhaps we could take the negative of the cost of a correct
                // prediction (-mCostMatrix.getElement(actualClass,actualClass)),
                // although often this will be zero
                mTotalCost += instance.weight() * mCostMatrix->getMaxCost(actualClass, instance);
            }
            else {
                mTotalCost += instance.weight()
                    * mCostMatrix->getElement(actualClass, predictedClass, instance);
            }
        }

        // Update counts when no class was predicted
        if (predictedClass < 0) {
            mUnclassified += instance.weight();
            return;
        }

        double predictedProb =
            std::max(MIN_SF_PROB, predictedDistribution[actualClass]);
        double priorProb =
            std::max(MIN_SF_PROB, mClassPriors[actualClass] / mClassPriorsSum);
        if (predictedProb >= priorProb) {
            mSumKBInfo +=
                (Utils::getLog2(predictedProb) - Utils::getLog2(priorProb))
                * instance.weight();
        }
        else {
            mSumKBInfo -=
                (Utils::getLog2(1.0 - predictedProb) - Utils::getLog2(1.0 - priorProb))
                * instance.weight();
        }

        mSumSchemeEntropy -= Utils::getLog2(predictedProb) * instance.weight();
        mSumPriorEntropy -= Utils::getLog2(priorProb) * instance.weight();

        updateNumericScores(predictedDistribution, makeDistribution(instance.classValue()), instance.weight());

        // Update other stats
        mConfusionMatrix[actualClass][predictedClass] += instance.weight();
        if (predictedClass != actualClass) {
            mIncorrect += instance.weight();
        }
        else {
            mCorrect += instance.weight();
        }
    }
    else {
        mMissingClass += instance.weight();
    }
}

void Evaluation::updateStatsForPredictor(const double predictedValue, Instance &instance)
{
    if (!instance.classIsMissing()) {

        // Update stats
        mWithClass += instance.weight();
        if (Utils::isMissingValue(predictedValue)) {
            mUnclassified += instance.weight();
            return;
        }
        mSumClass += instance.weight() * instance.classValue();
        mSumSqrClass +=
            instance.weight() * instance.classValue() * instance.classValue();
        mSumClassPredicted +=
            instance.weight() * instance.classValue() * predictedValue;
        mSumPredicted += instance.weight() * predictedValue;
        mSumSqrPredicted += instance.weight() * predictedValue * predictedValue;

        if (mErrorEstimator == nullptr) {
            setNumericPriorsFromBuffer();
        }
        double predictedProb =
            std::max(mErrorEstimator->getProbability(predictedValue - instance.classValue()), MIN_SF_PROB);
        double priorProb =
            std::max(mPriorErrorEstimator->getProbability(instance.classValue()), MIN_SF_PROB);

        mSumSchemeEntropy -= Utils::getLog2(predictedProb) * instance.weight();
        mSumPriorEntropy -= Utils::getLog2(priorProb) * instance.weight();
        mErrorEstimator->addValue(predictedValue - instance.classValue(), instance.weight());

        updateNumericScores(makeDistribution(predictedValue),
            makeDistribution(instance.classValue()), instance.weight());

    }
    else {
        mMissingClass += instance.weight();
    }
}

void Evaluation::updateMargins(const double_array predictedDistribution, const int actualClass, const double weight) {

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

void Evaluation::updateNumericScores(double_array predicted, double_array actual, double weight)
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

double_array Evaluation::makeDistribution(double predictedClass)
{
    double_array result = double_array(mNumClasses);
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
        double_array temp(mNumTrainClassVals);
        std::copy(mTrainClassVals.begin(), mTrainClassVals.begin() + mNumTrainClassVals, temp.begin());
        int_array index = Utils::Sort(temp);
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

string Evaluation::toSummaryString(const bool printComplexityStatistics) const
{
    return toSummaryString("=== Summary ===\n", printComplexityStatistics);
}

string Evaluation::toSummaryString(const string &title, bool printComplexityStatistics) const
{
    string text = "";

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

void Evaluation::addNumericTrainClass(const double classValue, const double weight)
{
    if (mTrainClassVals.empty()) {
        mTrainClassVals = double_array(100);
        mTrainClassWeights = double_array(100);
    }
    if (mNumTrainClassVals == mTrainClassVals.size()) {
        double_array temp = double_array(mTrainClassVals.size() * 2);
        std::copy(mTrainClassVals.begin(), mTrainClassVals.end(), temp.begin());
        mTrainClassVals = temp;

        temp = double_array(mTrainClassWeights.size() * 2);
        std::copy(mTrainClassWeights.begin(), mTrainClassWeights.end(), temp.begin());
        mTrainClassWeights = temp;
    }
    mTrainClassVals[mNumTrainClassVals] = classValue;
    mTrainClassWeights[mNumTrainClassVals] = weight;
    mNumTrainClassVals++;
}
const double Evaluation::correct() const
{
    return mCorrect;
}

const double Evaluation::totalCost() const
{
    return mTotalCost;
}

const double Evaluation::avgCost() const
{
    return mTotalCost / mWithClass;
}

const double Evaluation::kappa() const
{
    double_array sumRows = double_array(mConfusionMatrix.size());
    double_array sumColumns = double_array(mConfusionMatrix.size());
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

const double Evaluation::inCorrect() const
{
    return mIncorrect;
}

const double Evaluation::pctCorrect() const
{
    return 100 * mCorrect / mWithClass;
}
const double Evaluation::pctIncorrect() const
{
    return 100 * mIncorrect / mWithClass;
}

const double Evaluation::KBRelativeInformation() const
{
    if (!mClassIsNominal) {
        throw "Can't compute K&B Info score: class numeric!";
    }

    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return 100.0 * KBInformation() / priorEntropy();
}

const double Evaluation::KBInformation() const
{
    if (!mClassIsNominal) {
        throw "Can't compute K&B Info score: class numeric!";
    }

    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumKBInfo;
}
const double Evaluation::KBMeanInformation() const
{
    if (!mClassIsNominal) {
        throw "Can't compute K&B Info score: class numeric!";
    }

    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumKBInfo / (mWithClass - mUnclassified);
}

const double Evaluation::correlationCoefficient() const
{
    if (mClassIsNominal) {
        throw "Can't compute correlation coefficient: class is nominal!";
    }

    double correlation = 0;
    double varActual =
        mSumSqrClass - mSumClass * mSumClass / (mWithClass - mUnclassified);
    double varPredicted =
        mSumSqrPredicted - mSumPredicted * mSumPredicted
        / (mWithClass - mUnclassified);
    double varProd =
        mSumClassPredicted - mSumClass * mSumPredicted
        / (mWithClass - mUnclassified);

    if (varActual * varPredicted <= 0) {
        correlation = 0.0;
    }
    else {
        correlation = varProd / std::sqrt(varActual * varPredicted);
    }

    return correlation;
}

const double Evaluation::SFPriorEntropy() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumPriorEntropy;
}

const double Evaluation::SFMeanPriorEntropy() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumPriorEntropy / mWithClass;
}
const double Evaluation::SFSchemeEntropy() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumSchemeEntropy;
}

const double Evaluation::SFMeanSchemeEntropy() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumSchemeEntropy / (mWithClass - mUnclassified);
}

const double Evaluation::SFEntropyGain() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumPriorEntropy - mSumSchemeEntropy;
}

const double Evaluation::SFMeanEntropyGain() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return (mSumPriorEntropy - mSumSchemeEntropy)
        / (mWithClass - mUnclassified);
}
const double Evaluation::meanAbsoluteError() const
{
    return mSumAbsErr / (mWithClass - mUnclassified);
}
const double Evaluation::rootMeanSquaredError() const
{
    return std::sqrt(mSumSqrErr / (mWithClass - mUnclassified));
}
const double Evaluation::relativeAbsoluteError() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return 100 * meanAbsoluteError() / meanPriorAbsoluteError();
}
const double Evaluation::rootRelativeSquaredError() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return 100.0 * rootMeanSquaredError() / rootMeanPriorSquaredError();
}
const double Evaluation::unclassified() const
{
    return mUnclassified;
}
const double Evaluation::pctUnclassified() const
{
    return 100 * mUnclassified / mWithClass;
}
const double Evaluation::meanPriorAbsoluteError() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return mSumPriorAbsErr / mWithClass;
}
const double Evaluation::priorEntropy() const
{
    if (!mClassIsNominal) {
        throw "Can't compute entropy of class prior: class numeric!";
    }

    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double entropy = 0;
    for (int i = 0; i < mNumClasses; i++) {
        entropy -=
            mClassPriors[i] / mClassPriorsSum
            * Utils::getLog2(mClassPriors[i] / mClassPriorsSum);
    }
    return entropy;
}
const double Evaluation::rootMeanPriorSquaredError() const
{
    if (mNoPriors) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return std::sqrt(mSumPriorSqrErr / mWithClass);
}

string Evaluation::toClassDetailsString(const string &title) const
{
    if (!mClassIsNominal) {
        throw "Evaluation: No confusion matrix possible!";
    }

    string text = title + "\n               TP Rate   FP Rate"
        + "   Precision   Recall" + "  F-Measure   ROC Area  Class\n";
    for (int i = 0; i < mNumClasses; i++) {
        text.append(
            "               " + Utils::doubleToString(truePositiveRate(i), 7, 3))
            .append("   ");
        text.append(Utils::doubleToString(falsePositiveRate(i), 7, 3)).append(
            "    ");
        text.append(Utils::doubleToString(precision(i), 7, 3)).append("   ");
        text.append(Utils::doubleToString(recall(i), 7, 3)).append("   ");
        text.append(Utils::doubleToString(fMeasure(i), 7, 3)).append("    ");

        double rocVal = areaUnderROC(i);
        if (Utils::isMissingValue(rocVal)) {
            text.append("  ?    ").append("    ");
        }
        else {
            text.append(Utils::doubleToString(rocVal, 7, 3)).append("    ");
        }
        text.append(mClassNames[i]).append("\n");
    }

    text.append("Weighted Avg.  "
        + Utils::doubleToString(weightedTruePositiveRate(), 7, 3));
    text
        .append("   " + Utils::doubleToString(weightedFalsePositiveRate(), 7, 3));
    text.append("    " + Utils::doubleToString(weightedPrecision(), 7, 3));
    text.append("   " + Utils::doubleToString(weightedRecall(), 7, 3));
    text.append("   " + Utils::doubleToString(weightedFMeasure(), 7, 3));
    text.append("    " + Utils::doubleToString(weightedAreaUnderROC(), 7, 3));
    text.append("\n");

    return text;
}

string Evaluation::toClassDetailsString() const
{
    return toClassDetailsString("=== Detailed Accuracy By Class ===\n");
}

string Evaluation::toMatrixString() const
{
    return toMatrixString("=== Confusion Matrix ===\n");
}
string Evaluation::toMatrixString(const string &title) const
{
    string text = "";
    char_array IDChars =
    { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    int IDWidth;
    bool fractional = false;

    if (!mClassIsNominal) {
        throw "Evaluation: No confusion matrix possible!";
    }

    // Find the maximum value in the matrix
    // and check for fractional display requirement
    double maxval = 0;
    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            double current = mConfusionMatrix[i][j];
            if (current < 0) {
                current *= -10;
            }
            if (current > maxval) {
                maxval = current;
            }
            double fract = current - std::rint(current);
            if (!fractional && ((std::log(fract) / std::log(10)) >= -2)) {
                fractional = true;
            }
        }
    }

    int val1 = (int)std::log(maxval) / (int)std::log(10) + (fractional ? 3 : 0);
    int val2 = (int)std::log(mNumClasses) / (int)std::log(IDChars.size());
    IDWidth = 1 + val1 > val2 ? val1 : val2;
    text.append(title).append("\n");
    for (int i = 0; i < mNumClasses; i++) {
        if (fractional) {
            text.append(" ").append(num2ShortID(i, IDChars, IDWidth - 3))
                .append("   ");
        }
        else {
            text.append(" ").append(num2ShortID(i, IDChars, IDWidth));
        }
    }
    text.append("   <-- classified as\n");
    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            text.append(" ").append(
                Utils::doubleToString(mConfusionMatrix[i][j], IDWidth,
                    (fractional ? 2 : 0)));
        }
        text.append(" | ").append(num2ShortID(i, IDChars, IDWidth)).append(" = ")
            .append(mClassNames[i]).append("\n");
    }
    return text;
}

double Evaluation::truePositiveRate(const int classIndex) const
{
    double correct = 0, total = 0;
    for (int j = 0; j < mNumClasses; j++) {
        if (j == classIndex) {
            correct += mConfusionMatrix[classIndex][j];
        }
        total += mConfusionMatrix[classIndex][j];
    }
    if (total == 0) {
        return 0;
    }
    return correct / total;
}

double Evaluation::falsePositiveRate(const int classIndex) const
{
    double incorrect = 0, total = 0;
    for (int i = 0; i < mNumClasses; i++) {
        if (i != classIndex) {
            for (int j = 0; j < mNumClasses; j++) {
                if (j == classIndex) {
                    incorrect += mConfusionMatrix[i][j];
                }
                total += mConfusionMatrix[i][j];
            }
        }
    }
    if (total == 0) {
        return 0;
    }
    return incorrect / total;
}
double Evaluation::precision(const int classIndex) const
{
    double correct = 0, total = 0;
    for (int i = 0; i < mNumClasses; i++) {
        if (i == classIndex) {
            correct += mConfusionMatrix[i][classIndex];
        }
        total += mConfusionMatrix[i][classIndex];
    }
    if (total == 0) {
        return 0;
    }
    return correct / total;
}

double Evaluation::recall(const int classIndex) const
{
    return truePositiveRate(classIndex);
}

double Evaluation::fMeasure(const int classIndex) const
{
    double precisionValue = precision(classIndex);
    double recallValue = recall(classIndex);
    if ((precisionValue + recallValue) == 0) {
        return 0;
    }
    return 2 * precisionValue * recallValue / (precisionValue + recallValue);
}

double Evaluation::areaUnderROC(const int classIndex) const
{
    // Check if any predictions have been collected
    if (mPredictions.empty()) {
        return Utils::missingValue();
    }
    else {
        ThresholdCurve tc;
        Instances *result = tc.getCurve(mPredictions, classIndex);
        return ThresholdCurve::getROCArea(*result);
    }
}
double Evaluation::weightedTruePositiveRate() const
{
    double_array classCounts = double_array(mNumClasses);
    double classCountSum = 0;

    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            classCounts[i] += mConfusionMatrix[i][j];
        }
        classCountSum += classCounts[i];
    }

    double truePosTotal = 0;
    for (int i = 0; i < mNumClasses; i++) {
        double temp = truePositiveRate(i);
        truePosTotal += (temp * classCounts[i]);
    }

    return truePosTotal / classCountSum;
}
double Evaluation::weightedFalsePositiveRate() const
{
    double_array classCounts = double_array(mNumClasses);
    double classCountSum = 0;

    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            classCounts[i] += mConfusionMatrix[i][j];
        }
        classCountSum += classCounts[i];
    }

    double falsePosTotal = 0;
    for (int i = 0; i < mNumClasses; i++) {
        double temp = falsePositiveRate(i);
        falsePosTotal += (temp * classCounts[i]);
    }

    return falsePosTotal / classCountSum;
}
double Evaluation::weightedPrecision() const
{
    double_array classCounts = double_array(mNumClasses);
    double classCountSum = 0;

    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            classCounts[i] += mConfusionMatrix[i][j];
        }
        classCountSum += classCounts[i];
    }

    double precisionTotal = 0;
    for (int i = 0; i < mNumClasses; i++) {
        double temp = precision(i);
        precisionTotal += (temp * classCounts[i]);
    }

    return precisionTotal / classCountSum;
}
double Evaluation::weightedRecall() const
{
    return weightedTruePositiveRate();
}

double Evaluation::weightedFMeasure() const
{
    double_array classCounts = double_array(mNumClasses);
    double classCountSum = 0;

    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            classCounts[i] += mConfusionMatrix[i][j];
        }
        classCountSum += classCounts[i];
    }

    double fMeasureTotal = 0;
    for (int i = 0; i < mNumClasses; i++) {
        double temp = fMeasure(i);
        fMeasureTotal += (temp * classCounts[i]);
    }

    return fMeasureTotal / classCountSum;
}
double Evaluation::weightedAreaUnderROC() const
{
    double_array classCounts = double_array(mNumClasses);
    double classCountSum = 0;

    for (int i = 0; i < mNumClasses; i++) {
        for (int j = 0; j < mNumClasses; j++) {
            classCounts[i] += mConfusionMatrix[i][j];
        }
        classCountSum += classCounts[i];
    }

    double aucTotal = 0;
    for (int i = 0; i < mNumClasses; i++) {
        double temp = areaUnderROC(i);
        if (!Utils::isMissingValue(temp)) {
            aucTotal += (temp * classCounts[i]);
        }
    }

    return aucTotal / classCountSum;
}
string Evaluation::num2ShortID(int num, const char_array &IDChars, const int IDWidth) const {

    char_array ID(IDWidth);
    int i;

    for (i = IDWidth - 1; i >= 0; i--) {
        ID[i] = IDChars[num % IDChars.size()];
        num = num / (int)IDChars.size() - 1;
        if (num < 0) {
            break;
        }
    }
    for (i--; i >= 0; i--) {
        ID[i] = ' ';
    }

    return string(ID.begin(), ID.end());
}
