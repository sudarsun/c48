#ifndef _CLASSIFIERSPLITMODEL_
#define _CLASSIFIERSPLITMODEL_

#include "core/Instances.h"
#include "core/Instances.h"
#include <string>
#include <vector>
#include <stdexcept>

class Distribution;


/// <summary>
/// Abstract class for classification models that can be used
/// recursively to split the data.
/// </summary>

class ClassifierSplitModel
{
	/// <summary>
	/// Distribution of class values. </summary>
protected:
	Distribution *mDistribution;

	/// <summary>
	/// Number of created subsets. </summary>
	int mNumSubsets = 0;

	/// <summary>
	/// Allows to clone a model (shallow copy).
	/// </summary>
public:
	//virtual void *clone();

	/// <summary>
	/// Builds the classifier split model for the given set of instances.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual void buildClassifier(Instances instances) = 0;

	/// <summary>
	/// Checks if generated model is valid.
	/// </summary>
	bool checkModel();

	/// <summary>
	/// Classifies a given instance.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	double classifyInstance(Instance *instance);

	/// <summary>
	/// Gets class probability for instance.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual double classProb(int classIndex, Instance *instance, int theSubset);

	/// <summary>
	/// Gets class probability for instance.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual double classProbLaplace(int classIndex, Instance *instance, int theSubset);

	/// <summary>
	/// Returns coding costs of model. Returns 0 if not overwritten.
	/// </summary>
	virtual double codingCost();

	/// <summary>
	/// Returns the distribution of class values induced by the model.
	/// </summary>
	Distribution *distribution();

	/// <summary>
	/// Prints left side of condition satisfied by instances.
	/// </summary>
	/// <param name="data"> the data. </param>
	virtual std::string leftSide(Instances *data) = 0;

	/// <summary>
	/// Prints left side of condition satisfied by instances in subset index.
	/// </summary>
	virtual std::string rightSide(int index, Instances *data) = 0;

	/// <summary>
	/// Prints label for subset index of instances (eg class).
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	std::string dumpLabel(int index, Instances *data);

	std::string sourceClass(int index, Instances *data);

	virtual std::string sourceExpression(int index, Instances *data) = 0;

	/// <summary>
	/// Prints the split model.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	std::string dumpModel(Instances *data);

	/// <summary>
	/// Returns the number of created subsets for the split.
	/// </summary>
	int numSubsets();

	/// <summary>
	/// Sets distribution associated with model.
	/// </summary>
	virtual void resetDistribution(Instances *data);

	/// <summary>
	/// Splits the given set of instances into subsets.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	std::vector<Instances*> split(Instances *data);

	/// <summary>
	/// Returns weights if instance is assigned to more than one subset.
	/// Returns null if instance is only assigned to one subset.
	/// </summary>
	virtual std::vector<double> weights(Instance *instance) = 0;

	/// <summary>
	/// Returns index of subset instance is assigned to.
	/// Returns -1 if instance is assigned to more than one subset.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual int whichSubset(Instance *instance) = 0;

	Distribution* getDistribution();
	void setDistribution(Distribution*);
};

#endif	// _CLASSIFIERSPLITMODEL_
