#include "AbstractClassifier.h"
#include "core/Instance.h"
#include "core/Instances.h"

#include "core/Consts.h"
#include "evaluation/BatchPredictor.h"
#include "core/Utils.h"

double AbstractClassifier::classifyInstance(Instance *instance) {

	double_array dist = distributionForInstance(instance);
	if (dist.empty()) {
		throw "Null distribution predicted";
	}
	switch (instance->classAttribute()->type()) {
	case ATTRIBUTE::NOMINAL: {
		double max = 0;
		int maxIndex = 0;

		for (int i = 0; i < (int)dist.size(); i++) {
			if (dist[i] > max) {
				maxIndex = i;
				max = dist[i];
			}
		}
		if (max > 0) {
			return maxIndex;
		}
		else {
			return Utils::missingValue();
		}
	}
	case ATTRIBUTE::NUMERIC:
	case ATTRIBUTE::DATE:
		return dist[0];
	default:
		return Utils::missingValue();
	}
}

double_array AbstractClassifier::distributionForInstance(Instance *instance) {

	double_array dist(instance->numClasses());
	switch (instance->classAttribute()->type()) {
	case ATTRIBUTE::NOMINAL: {
		double classification = classifyInstance(instance);
		if (Utils::isMissingValue(classification)) {
			return dist;
		}
		else {
			dist[static_cast<int>(classification)] = 1.0;
		}
		return dist;
	}
	case ATTRIBUTE::NUMERIC:
	case ATTRIBUTE::DATE:
		dist[0] = classifyInstance(instance);
		return dist;
	default:
		return dist;
	}
}

bool AbstractClassifier::getDebug() {

	return mDebug;
}

void AbstractClassifier::setDebug(bool debug) {

	mDebug = debug;
}

int AbstractClassifier::getNumDecimalPlaces() {
	return mNumDecimalPlaces;
}

void AbstractClassifier::setNumDecimalPlaces(int num) {
	mNumDecimalPlaces = num;
}

void AbstractClassifier::setBatchSize(const string &size) {
	mBatchSize = size;
}

string AbstractClassifier::getBatchSize() {
	return mBatchSize;
}

bool AbstractClassifier::implementsMoreEfficientBatchPrediction() {
	return false;
}

double_2D_array AbstractClassifier::distributionsForInstances(Instances *batch) {
	double_2D_array batchPreds(batch->numInstances());
	for (int i = 0; i < batch->numInstances(); i++) {
		batchPreds[i] = distributionForInstance(batch->instance(i));
	}

	return batchPreds;
}
