#include "KernelEstimator.h"
#include "core/Utils.h"
#include "c48/Stats.h"


double KernelEstimator::MAX_ERROR = 0.01;

int KernelEstimator::findNearestValue(double key) {

    int low = 0;
    int high = mNumValues;
    int middle = 0;
    while (low < high) {
        middle = (low + high) / 2;
        double current = mValues[middle];
        if (current == key) {
            return middle;
        }
        if (current > key) {
            high = middle;
        }
        else if (current < key) {
            low = middle + 1;
        }
    }
    return low;
}

double KernelEstimator::round(double data) {

    return std::rint(data / mPrecision) * mPrecision;
}

KernelEstimator::KernelEstimator(double precision) {

    mValues = double_array(50);
    mWeights = double_array(50);
    mNumValues = 0;
    mSumOfWeights = 0;
    mAllWeightsOne = true;
    mPrecision = precision;
    // precision cannot be zero
    if (mPrecision < Utils::SMALL) {
        mPrecision = Utils::SMALL;
    }
    //    mStandardDev = 1e10 * mPrecision; // Set the standard deviation initially very wide
    mStandardDev = mPrecision / (2 * 3);
}

void KernelEstimator::addValue(double data, double weight) {

    if (weight == 0) {
        return;
    }
    data = round(data);
    int insertIndex = findNearestValue(data);
    if ((mNumValues <= insertIndex) || (mValues[insertIndex] != data)) {
        if (mNumValues < mValues.size()) {
            int left = mNumValues - insertIndex;
            std::cout << "Please cross-check here tfor array copy";
            std::copy(mValues.begin() + insertIndex, mValues.begin() + left, mValues.begin() + insertIndex + 1);
            std::copy(mWeights.begin() + insertIndex, mWeights.begin() + left, mWeights.begin() + insertIndex + 1);

            // std::copy( mValues, insertIndex, mValues, insertIndex + 1, left );
            // std::copy( mWeights, insertIndex, mWeights, insertIndex + 1, left );

            mValues[insertIndex] = data;
            mWeights[insertIndex] = weight;
            mNumValues++;
        }
        else {
            double_array newValues(mValues.size() * 2);
            double_array newWeights(mValues.size() * 2);
            int left = mNumValues - insertIndex;
            std::copy(mValues.begin(), mValues.begin() + insertIndex, newValues.begin());
            std::copy(mWeights.begin(), mWeights.begin() + insertIndex, newWeights.begin());
            //std::copy( mValues, 0, newValues, 0, insertIndex );
            //std::copy( mWeights, 0, newWeights, 0, insertIndex );
            newValues[insertIndex] = data;
            newWeights[insertIndex] = weight;
            std::copy(mValues.begin() + insertIndex, mValues.begin() + left, newValues.begin() + insertIndex + 1);
            std::copy(mWeights.begin() + insertIndex, mWeights.begin() + left, newWeights.begin() + insertIndex + 1);
            //std::copy( mValues, insertIndex, newValues, insertIndex + 1, left );
            //std::copy( mWeights, insertIndex, newWeights, insertIndex + 1, left );
            mNumValues++;
            mValues = newValues;
            mWeights = newWeights;
        }
        if (weight != 1) {
            mAllWeightsOne = false;
        }
    }
    else {
        mWeights[insertIndex] += weight;
        mAllWeightsOne = false;
    }
    mSumOfWeights += weight;
    double range = mValues[mNumValues - 1] - mValues[0];
    if (range > 0) {
        mStandardDev = std::fmax(range / sqrt(mSumOfWeights), mPrecision / (2 * 3));
        // allow at most 3 sds within one interval
    }
}

double KernelEstimator::getProbability(double data) {

    double delta = 0, sum = 0, currentProb = 0;
    double zLower = 0, zUpper = 0;
    if (mNumValues == 0) {
        zLower = (data - (mPrecision / 2)) / mStandardDev;
        zUpper = (data + (mPrecision / 2)) / mStandardDev;
        return (Stats::normalProbability(zUpper) - Stats::normalProbability(zLower));
    }
    double weightSum = 0;
    int start = findNearestValue(data);
    for (int i = start; i < mNumValues; i++) {
        delta = mValues[i] - data;
        zLower = (delta - (mPrecision / 2)) / mStandardDev;
        zUpper = (delta + (mPrecision / 2)) / mStandardDev;
        currentProb = (Stats::normalProbability(zUpper) - Stats::normalProbability(zLower));
        sum += currentProb * mWeights[i];
        /*
        System.out.print("zL" + (i + 1) + ": " + zLower + " ");
        System.out.print("zU" + (i + 1) + ": " + zUpper + " ");
        System.out.print("P" + (i + 1) + ": " + currentProb + " ");
        System.out.println("total: " + (currentProb * mWeights[i]) + " ");
        */
        weightSum += mWeights[i];
        if (currentProb * (mSumOfWeights - weightSum) < sum * MAX_ERROR) {
            break;
        }
    }
    for (int i = start - 1; i >= 0; i--) {
        delta = mValues[i] - data;
        zLower = (delta - (mPrecision / 2)) / mStandardDev;
        zUpper = (delta + (mPrecision / 2)) / mStandardDev;
        currentProb = (Stats::normalProbability(zUpper) - Stats::normalProbability(zLower));
        sum += currentProb * mWeights[i];
        weightSum += mWeights[i];
        if (currentProb * (mSumOfWeights - weightSum) < sum * MAX_ERROR) {
            break;
        }
    }
    return sum / mSumOfWeights;
}

string KernelEstimator::toString() {

    string result = std::to_string(mNumValues) + string(" Normal Kernels. \nStandardDev = ") + Utils::doubleToString(mStandardDev, 6, 4) + string(" Precision = ") + std::to_string(mPrecision);
    if (mNumValues == 0) {
        result += string("  \nMean = 0");
    }
    else {
        result += string("  \nMeans =");
        for (int i = 0; i < mNumValues; i++) {
            result += string(" ") + std::to_string(mValues[i]);
        }
        if (!mAllWeightsOne) {
            result += string("\nWeights = ");
            for (int i = 0; i < mNumValues; i++) {
                result += string(" ") + std::to_string(mWeights[i]);
            }
        }
    }
    return result + string("\n");
}

int KernelEstimator::getNumKernels() {
    return mNumValues;
}

double_array KernelEstimator::getMeans() {
    return mValues;
}

double_array KernelEstimator::getWeights() {
    return mWeights;
}

double KernelEstimator::getPrecision() {
    return mPrecision;
}

double KernelEstimator::getStdDev() {
    return mStandardDev;
}
