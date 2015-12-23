#ifndef _CLASSIFIERSPLITMODEL_
#define _CLASSIFIERSPLITMODEL_

#include "core/Instances.h"
#include "core/Instances.h"
#include <string>
#include <vector>
#include <stdexcept>

class Distribution;

/**
 * Abstract class for classification models that can be used recursively to split the data.
 */
class ClassifierSplitModel
{

protected:

	/** Distribution of class values. */
	Distribution *mDistribution;

	/** Number of created subsets. */
	int mNumSubsets = 0;

public:

	/**
	 * Builds the classifier split model for the given set of instances.
	 *
	 * @exception Exception if something goes wrong
	 */
	virtual void buildClassifier(Instances instances) = 0;

	/**
	 * Checks if generated model is valid.
	 */
	bool checkModel();

	/**
	 * Classifies a given instance.
	 *
	 * @exception Exception if something goes wrong
	 */
	double classifyInstance(Instance *instance);

	/**
	 * Gets class probability for instance.
	 *
	 * @exception Exception if something goes wrong
	 */
	virtual double classProb(int classIndex, Instance *instance, int theSubset);

	/**
	 * Gets class probability for instance.
	 *
	 * @exception Exception if something goes wrong
	 */
	virtual double classProbLaplace(int classIndex, Instance *instance, int theSubset);

	/**
	 * Returns coding costs of model. Returns 0 if not overwritten.
	 */
	virtual double codingCost();

	/**
	 * Returns the distribution of class values induced by the model.
	 */
	Distribution *distribution();

	/**
	 * Prints left side of condition satisfied by instances.
	 *
	 * @param data the data.
	 */
	virtual string leftSide(Instances *data) = 0;

	/**
	 * Prints left side of condition satisfied by instances in subset index.
	 */
	virtual string rightSide(int index, Instances *data) = 0;

	/**
	 * Prints label for subset index of instances (eg class).
	 *
	 * @exception Exception if something goes wrong
	 */
	string dumpLabel(int index, Instances *data);

	/**
	 * Prints the split model.
	 *
	 * @exception Exception if something goes wrong
	 */
	string dumpModel(Instances *data);

	/// <summary>
	/// Returns the number of created subsets for the split.
	/// </summary>
	int numSubsets();

	/**
	 * Returns the number of created subsets for the split.
	 */
	virtual void resetDistribution(Instances *data);

	/**
	 * Splits the given set of instances into subsets.
	 *
	 * @exception Exception if something goes wrong
	 */
	std::vector<Instances*> split(Instances *data);

	/**
	 * Returns weights if instance is assigned to more than one subset.
	 * Returns null if instance is only assigned to one subset.
	 */
	virtual double_array weights(Instance *instance) = 0;

	/**
	 * Returns index of subset instance is assigned to.
	 * Returns -1 if instance is assigned to more than one subset.
	 *
	 * @exception Exception if something goes wrong
	 */
	virtual int whichSubset(Instance *instance) = 0;

	/**
	 * Get the distribution opbject
	 */
	Distribution* getDistribution();

	/**
	 * Set the distribution object
	 */
	void setDistribution(Distribution*);
};

#endif	// _CLASSIFIERSPLITMODEL_
