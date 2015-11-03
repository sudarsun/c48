#include "NBTreeSplit.h"
#include "C45Split.h"
#include "NBTreeNoSplit.h"
#include "core/Instances.h"
#include "core/Instance.h"



NBTreeSplit::NBTreeSplit(int attIndex, int minNoObj, double sumOfWeights) : mattIndex(attIndex), msumOfWeights(sumOfWeights) {

	// Get index of attribute to split on.

	// Set the sum of the weights

}

void NBTreeSplit::buildClassifier(Instances *trainInstances) {

	// Initialize the remaining instance variables.
	mnumSubsets = 0;
	merrors = 0;
	if (mglobalNB != nullptr) {
		merrors = mglobalNB->getErrors();
	}

	// Different treatment for enumerated and numeric
	// attributes.
	if (trainInstances->attribute(mattIndex)->isNominal()) {
		mcomplexityIndex = trainInstances->attribute(mattIndex)->numValues();
		handleEnumeratedAttribute(trainInstances);
	}
	else {
		mcomplexityIndex = 2;
		trainInstances->Sort(trainInstances->attribute(mattIndex));
		handleNumericAttribute(trainInstances);
	}
}

int NBTreeSplit::attIndex() {

	return mattIndex;
}

void NBTreeSplit::handleEnumeratedAttribute(Instances *trainInstances) {

	mc45S = new C45Split(mattIndex, 2, msumOfWeights, true);
	mc45S->buildClassifier(trainInstances);
	if (mc45S->numSubsets() == 0) {
		return;
	}
	merrors = 0;
	Instance *instance;

	std::vector<Instances*> trainingSets(mcomplexityIndex);
	for (int i = 0; i < mcomplexityIndex; i++) {
		trainingSets[i] = new Instances(trainInstances, 0);
	}
	/*
	 * mdistribution = new Distribution(mcomplexityIndex,
	 * trainInstances.numClasses());
	 */
	int subset;
	for (int i = 0; i < trainInstances->numInstances(); i++) {
		instance = trainInstances->instance(i);
		subset = mc45S->whichSubset(instance);
		if (subset > -1) {
			trainingSets[subset]->add(static_cast<Instance*>(instance->copy()));
		}
		else {
			std::vector<double> weights = mc45S->weights(instance);
			for (int j = 0; j < mcomplexityIndex; j++) {
				try {
					Instance *temp = static_cast<Instance*>(instance->copy());
					if (weights.size() == mcomplexityIndex) {
						temp->setWeight(temp->weight() * weights[j]);
					}
					else {
						temp->setWeight(temp->weight() / mcomplexityIndex);
					}
					trainingSets[j]->add(temp);
				}
				catch (std::exception &ex) {
					ex.printStackTrace();
					System::err::println(std::string("*** ") + std::to_wstring(mcomplexityIndex));
					System::err::println(weights.size());
					exit(1);
				}
			}
		}
	}

	/*
	 * // compute weights (weights of instances per subset mweights = new
	 * double [mcomplexityIndex]; for (int i = 0; i < mcomplexityIndex; i++) {
	 * mweights[i] = trainingSets[i].sumOfWeights(); }
	 * Utils.normalize(mweights);
	 */

	/*
	 * // Only Instances with known values are relevant. Enumeration enu =
	 * trainInstances.enumerateInstances(); while (enu.hasMoreElements()) {
	 * instance = (Instance) enu.nextElement(); if
	 * (!instance.isMissing(mattIndex)) { //
	 * mdistribution.add((int)instance.value(mattIndex),instance);
	 * trainingSets[(int)instances.value(mattIndex)].add(instance); } else { //
	 * add these to the error count merrors += instance.weight(); } }
	 */

	Random *r = new Random(1);
	int minNumCount = 0;
	for (int i = 0; i < mcomplexityIndex; i++) {
		if (trainingSets[i]->numInstances() >= 5) {
			minNumCount++;
			// Discretize the sets
			Discretize *disc = new Discretize();
			disc->setInputFormat(trainingSets[i]);
			trainingSets[i] = Filter::useFilter(trainingSets[i], disc);

			trainingSets[i]->randomize(r);
			trainingSets[i]->stratify(5);
			NaiveBayesUpdateable *fullModel = new NaiveBayesUpdateable();
			fullModel->buildClassifier(trainingSets[i]);

			// add the errors for this branch of the split
			merrors += NBTreeNoSplit::crossValidate(fullModel, trainingSets[i], r);
		}
		else {
			// if fewer than min obj then just count them as errors
			for (int j = 0; j < trainingSets[i]->numInstances(); j++) {
				merrors += trainingSets[i]->instance(j).weight();
			}
		}
	}

	// Check if there are at least five instances in at least two of the subsets
	// subsets.
	if (minNumCount > 1) {
		mnumSubsets = mcomplexityIndex;
	}
}

void NBTreeSplit::handleNumericAttribute(Instances *trainInstances) {

	mc45S = new C45Split(mattIndex, 2, msumOfWeights, true);
	mc45S->buildClassifier(trainInstances);
	if (mc45S->numSubsets() == 0) {
		return;
	}
	merrors = 0;

	std::vector<Instances*> trainingSets(mcomplexityIndex);
	trainingSets[0] = new Instances(trainInstances, 0);
	trainingSets[1] = new Instances(trainInstances, 0);
	int subset = -1;

	// populate the subsets
	for (int i = 0; i < trainInstances->numInstances(); i++) {
		Instance *instance = trainInstances->instance(i);
		subset = mc45S->whichSubset(instance);
		if (subset != -1) {
			trainingSets[subset]->add(static_cast<Instance*>(instance->copy()));
		}
		else {
			std::vector<double> weights = mc45S->weights(instance);
			for (int j = 0; j < mcomplexityIndex; j++) {
				Instance *temp = static_cast<Instance*>(instance->copy());
				if (weights.size() == mcomplexityIndex) {
					temp->setWeight(temp->weight() * weights[j]);
				}
				else {
					temp->setWeight(temp->weight() / mcomplexityIndex);
				}
				trainingSets[j]->add(temp);
			}
		}
	}

	/*
	 * // compute weights (weights of instances per subset mweights = new
	 * double [mcomplexityIndex]; for (int i = 0; i < mcomplexityIndex; i++) {
	 * mweights[i] = trainingSets[i].sumOfWeights(); }
	 * Utils.normalize(mweights);
	 */

	Random *r = new Random(1);
	int minNumCount = 0;
	for (int i = 0; i < mcomplexityIndex; i++) {
		if (trainingSets[i]->numInstances() > 5) {
			minNumCount++;
			// Discretize the sets
			Discretize *disc = new Discretize();
			disc->setInputFormat(trainingSets[i]);
			trainingSets[i] = Filter::useFilter(trainingSets[i], disc);

			trainingSets[i]->randomize(r);
			trainingSets[i]->stratify(5);
			NaiveBayesUpdateable *fullModel = new NaiveBayesUpdateable();
			fullModel->buildClassifier(trainingSets[i]);

			// add the errors for this branch of the split
			merrors += NBTreeNoSplit::crossValidate(fullModel, trainingSets[i], r);
		}
		else {
			for (int j = 0; j < trainingSets[i]->numInstances(); j++) {
				merrors += trainingSets[i]->instance(j).weight();
			}
		}
	}

	// Check if minimum number of Instances in at least two
	// subsets.
	if (minNumCount > 1) {
		mnumSubsets = mcomplexityIndex;
	}
}

int NBTreeSplit::whichSubset(Instance *instance) {

	return mc45S->whichSubset(instance);
}

std::vector<double> NBTreeSplit::weights(Instance *instance) {
	return mc45S->weights(instance);
	// return mweights;
}

std::string NBTreeSplit::sourceExpression(int index, Instances *data) {
	return mc45S->sourceExpression(index, data);
}

std::string NBTreeSplit::rightSide(int index, Instances *data) {
	return mc45S->rightSide(index, data);
}

std::string NBTreeSplit::leftSide(Instances *data) {

	return mc45S->leftSide(data);
}

double NBTreeSplit::classProb(int classIndex, Instance *instance, int theSubset) {

	// use the global naive bayes model
	if (theSubset > -1) {
		return mglobalNB->classProb(classIndex, instance, theSubset);
	}
	else {
		throw "This shouldn't happen!!!";
	}
}

NBTreeNoSplit *NBTreeSplit::getGlobalModel() {
	return mglobalNB;
}

void NBTreeSplit::setGlobalModel(NBTreeNoSplit *global) {
	mglobalNB = global;
}

double NBTreeSplit::getErrors() {
	return merrors;
}

