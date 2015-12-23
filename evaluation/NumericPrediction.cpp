#include "NumericPrediction.h"

NumericPrediction::NumericPrediction(double actual, double predicted) : NumericPrediction(actual, predicted, 1) {
}

NumericPrediction::NumericPrediction(double actual, double predicted, double weight) {
	double_2D_array value(0, double_array());
	NumericPrediction(actual, predicted, weight, value);
}

NumericPrediction::NumericPrediction(double actual, double predicted, double weight, double_2D_array &predInt) {

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

void NumericPrediction::setPredictionIntervals(double_2D_array &predInt) {
	mPredictionIntervals = predInt;
}

double_2D_array NumericPrediction::predictionIntervals() {
	return mPredictionIntervals;
}

string NumericPrediction::toString() {
	string desc = "";
	desc.append("NUM: ").append(std::to_string(actual())).append(" ").append(std::to_string(predicted()));
	desc.append(" ").append(std::to_string(weight()));

	return desc;
}
