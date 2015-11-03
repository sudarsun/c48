#include "statistics.h"
#include "Utils.h"


void Statistics::reset() {
	count = 0;
	sum = 0;
	sumSq = 0;
	stdDev = std::numeric_limits<double>::quiet_NaN();
	mean = std::numeric_limits<double>::quiet_NaN();
	min = std::numeric_limits<double>::quiet_NaN();
	max = std::numeric_limits<double>::quiet_NaN();
	stdDevFactor = 0;
}

void Statistics::negativeCount() {
	sum = std::numeric_limits<double>::quiet_NaN();
	sumSq = std::numeric_limits<double>::quiet_NaN();
	stdDev = std::numeric_limits<double>::quiet_NaN();
	mean = std::numeric_limits<double>::quiet_NaN();
	min = std::numeric_limits<double>::quiet_NaN();
	max = std::numeric_limits<double>::quiet_NaN();
}

void Statistics::goInvalid() {
	count = std::numeric_limits<double>::quiet_NaN();
	negativeCount();
}

bool Statistics::isInvalid() {
	return std::isnan(count);
}

void Statistics::add(double value) {

	add(value, 1);
}

void Statistics::add(double value, double weight) {

	// treat as subtract
	if (weight < 0) {
		subtract(value, -weight);
		return;
	}

	// don't leave invalid state
	if (isInvalid()) {
		return;
	}

	// go invalid
	if (std::isinf(weight) || std::isnan(weight) || std::isinf(value) || std::isnan(value)) {
		goInvalid();
		return;
	}

	// ignore
	if (weight == 0) {
		return;
	}

	double newCount = count + weight;
	if (count < 0 && (newCount > 0 || Utils::eq(newCount, 0))) {
		reset();
		return;
	}

	count = newCount;

	if (count < 0) {
		return;
	}

	double weightedValue = value*weight;
	sum += weightedValue;
	sumSq += value * weightedValue;
	if (std::isnan(mean)) {
		// For the first value the mean can suffer from loss of precision
		// so we treat it separately and make sure the calculation stays accurate
		mean = value;
		stdDevFactor = 0;
	}
	else {
		double delta = weight*(value - mean);
		mean += delta / count;
		stdDevFactor += delta*(value - mean);
	}

	if (std::isnan(min)) {
		min = max = value;
	}
	else if (value < min) {
		min = value;
	}
	else if (value > max) {
		max = value;
	}
}

void Statistics::subtract(double value) {
	subtract(value, 1);
}

void Statistics::subtract(double value, double weight) {

	// treat as add
	if (weight < 0) {
		add(value, -weight);
		return;
	}

	// don't leave invalid state
	if (isInvalid()) {
		return;
	}

	// go invalid
	if (std::isinf(weight) || std::isnan(weight) || std::isinf(value) || std::isnan(value)) {
		goInvalid();
		return;
	}

	// ignore
	if (weight == 0) {
		return;
	}

	count -= weight;

	if (Utils::eq(count, 0)) {
		reset();
		return;
	}
	else if (count < 0) {
		negativeCount();
		return;
	}

	double weightedValue = value*weight;
	sum -= weightedValue;
	sumSq -= value * weightedValue;
	double delta = weight*(value - mean);
	mean -= delta / count;
	stdDevFactor -= delta*(value - mean);
}

void Statistics::calculateDerived() {

	if (count <= 1) {
		stdDev = std::numeric_limits<double>::quiet_NaN();
		return;
	}
	stdDev = stdDevFactor / (count - 1);
	if (stdDev < 0) {
		stdDev = 0;
		return;
	}
	stdDev = sqrt(stdDev);

}

std::string Statistics::toString() {
	std::string tostring = "";

	tostring.append("Count   ");
	tostring.append(count, 8);
	tostring.append("\n");
	tostring.append("Min     ");
	tostring.append(min, 8);
	tostring.append("\n");
	tostring.append("Max     ");
	tostring.append(max, 8);
	tostring.append("\n");
	tostring.append("Sum     ");
	tostring.append(sum, 8);
	tostring.append("\n");
	tostring.append("SumSq   ");
	tostring.append(sumSq, 8);
	tostring.append("\n");
	tostring.append("Mean    ");
	tostring.append(mean, 8);
	tostring.append("\n");
	tostring.append("StdDev  ");
	tostring.append(stdDev, 8);
	tostring.append("\n");
	return tostring;
}
