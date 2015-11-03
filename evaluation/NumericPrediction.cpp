#include "NumericPrediction.h"

NumericPrediction::NumericPrediction(double actual, double predicted) : NumericPrediction(actual, predicted, 1) {
}

NumericPrediction::NumericPrediction(double actual, double predicted, double weight) {
	std::vector<std::vector<double>> value(0, std::vector<double>());
	NumericPrediction(actual, predicted, weight, value);
}

NumericPrediction::NumericPrediction(double actual, double predicted, double weight, std::vector<std::vector<double>> &predInt) {

	mActual = actual;
	mPredicted = predicted;
	mWeight = weight;
	setPredictionIntervals(predInt);
}

double NumericPrediction::actual() {
	return mActual;
}

double NumericPrediction::predicted() {
	return mPredicted;
}

double NumericPrediction::weight() {
	return mWeight;
}

double NumericPrediction::error() {
	if ((mActual == MISSING_VALUE) || (mPredicted == MISSING_VALUE)) {
		return MISSING_VALUE;
	}
	return mPredicted - mActual;
}

void NumericPrediction::setPredictionIntervals(std::vector<std::vector<double>> &predInt) {
	mPredictionIntervals = predInt;
}

std::vector<std::vector<double>> NumericPrediction::predictionIntervals() {
	return mPredictionIntervals;
}

std::string NumericPrediction::toString() {
	std::string desc = "";
	desc.append("NUM: ").append(std::to_string(actual())).append(" ").append(std::to_string(predicted()));
	desc.append(" ").append(std::to_string(weight()));

	return desc;
}
