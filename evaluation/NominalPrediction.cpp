#include "NominalPrediction.h"

NominalPrediction::NominalPrediction(double actual, double_array &distribution) : NominalPrediction(actual, distribution, 1) {

}

NominalPrediction::NominalPrediction(double actual, double_array &distribution, double weight) {

    if (distribution.empty()) {
        throw "Null distribution in NominalPrediction.";
    }
    mActual = actual;
    mDistribution = distribution;
    mWeight = weight;
    updatePredicted();
}

double_array NominalPrediction::distribution() {

    return mDistribution;
}

double NominalPrediction::actual() {

    return mActual;
}

double NominalPrediction::predicted() {

    return mPredicted;
}

double NominalPrediction::weight() {

    return mWeight;
}

double NominalPrediction::margin() {

    if ((mActual == MISSING_VALUE) || (mPredicted == MISSING_VALUE)) {
        return MISSING_VALUE;
    }
    double probActual = mDistribution[static_cast<int>(mActual)];
    double probNext = 0;
    for (int i = 0; i < (int)mDistribution.size(); i++) {
        if ((i != mActual) && (mDistribution[i] > probNext)) {
            probNext = mDistribution[i];
        }
    }

    return probActual - probNext;
}

double_array NominalPrediction::makeDistribution(double predictedClass, int numClasses) {

    double_array dist(numClasses);
    if (predictedClass == MISSING_VALUE) {
        return dist;
    }
    dist[static_cast<int>(predictedClass)] = 1.0;
    return dist;
}

double_array NominalPrediction::makeUniformDistribution(int numClasses) {

    double_array dist(numClasses);
    for (int i = 0; i < numClasses; i++) {
        dist[i] = 1.0 / numClasses;
    }
    return dist;
}

void NominalPrediction::updatePredicted() {

    int predictedClass = -1;
    double bestProb = 0.0;
    for (int i = 0; i < (int)mDistribution.size(); i++) {
        if (mDistribution[i] > bestProb) {
            predictedClass = i;
            bestProb = mDistribution[i];
        }
    }

    if (predictedClass != -1) {
        mPredicted = predictedClass;
    }
    else {
        mPredicted = MISSING_VALUE;
    }
}

string NominalPrediction::toString() {

    string desc = "";
    desc.append("NOM: ").append(std::to_string(actual())).append(" ").append(std::to_string(predicted()));
    desc.append(" ").append(std::to_string(weight()));
    double_array dist = distribution();
    for (int i = 0; i < (int)dist.size(); i++) {
        desc.append(" ").append(std::to_string(dist[i]));
    }

    return desc;
}
