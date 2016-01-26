#include "NumericPrediction.h"

NumericPrediction::NumericPrediction(double actual, double predicted) : NumericPrediction(actual, predicted, 1) {
}

NumericPrediction::NumericPrediction(double actual, double predicted, double weight) {
    mActual = actual;
    mPredicted = predicted;
    mWeight = weight;
}

double NumericPrediction::actual() const {
    return mActual;
}

double NumericPrediction::predicted() const {
    return mPredicted;
}

double NumericPrediction::weight() const {
    return mWeight;
}

double NumericPrediction::error() const {
    if ((mActual == MISSING_VALUE) || (mPredicted == MISSING_VALUE)) {
        return MISSING_VALUE;
    }
    return mPredicted - mActual;
}

string NumericPrediction::toString() const {
    string desc = "";
    desc.append("NUM: ").append(std::to_string(actual())).append(" ").append(std::to_string(predicted()));
    desc.append(" ").append(std::to_string(weight()));

    return desc;
}
