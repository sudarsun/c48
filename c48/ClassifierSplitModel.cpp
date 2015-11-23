#include "ClassifierSplitModel.h"
#include "Distribution.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "core/Utils.h"


/*void *ClassifierSplitModel::clone()
{

return new ClassifierSplitModel();
}*/

bool ClassifierSplitModel::checkModel()
{

	if (mNumSubsets > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double ClassifierSplitModel::classifyInstance(Instance *instance)
{

	int theSubset;

	theSubset = whichSubset(instance);
	if (theSubset > -1)
	{
		return static_cast<double>(mDistribution->maxClass(theSubset));
	}
	else
	{
		return static_cast<double>(mDistribution->maxClass());
	}
}

double ClassifierSplitModel::classProb(int classIndex, Instance *instance, int theSubset)
{

	if (theSubset > -1)
	{
		return mDistribution->prob(classIndex, theSubset);
	}
	else
	{
		std::vector<double> _weights = weights(instance);
		if (_weights.empty())
		{
			return mDistribution->prob(classIndex);
		}
		else
		{
			double prob = 0;
			for (int i = 0; i < (int)_weights.size(); i++)
			{
				prob += _weights[i] * mDistribution->prob(classIndex, i);
			}
			return prob;
		}
	}
}

double ClassifierSplitModel::classProbLaplace(int classIndex, Instance *instance, int theSubset)
{

	if (theSubset > -1)
	{
		return mDistribution->laplaceProb(classIndex, theSubset);
	}
	else
	{
		std::vector<double> _weights = weights(instance);
		if (_weights.empty())
		{
			return mDistribution->laplaceProb(classIndex);
		}
		else
		{
			double prob = 0;
			for (int i = 0; i < (int)_weights.size(); i++)
			{
				prob += _weights[i] * mDistribution->laplaceProb(classIndex, i);
			}
			return prob;
		}
	}
}

double ClassifierSplitModel::codingCost()
{

	return 0;
}

Distribution *ClassifierSplitModel::distribution()
{

	return mDistribution;
}

std::string ClassifierSplitModel::dumpLabel(int index, Instances *data)
{

	std::string text;

	text = "";
	text.append((static_cast<Instances*>(data))->classAttribute()->value(mDistribution->maxClass(index)));
	text.append(std::string(" (") + std::to_string(Utils::roundDouble(mDistribution->perBag(index), 2)));
	if (Utils::gr(mDistribution->numIncorrect(index), 0))
	{
		text.append(std::string("/") + std::to_string(Utils::roundDouble(mDistribution->numIncorrect(index), 2)));
	}
	text.append(")");

	return text;
}

std::string ClassifierSplitModel::sourceClass(int index, Instances *data)
{
	std::cout << "sourceClass";
	return std::to_string(mDistribution->maxClass(index));
}

std::string ClassifierSplitModel::dumpModel(Instances *data)
{

	std::string text;
	int i;

	text = "";
	for (i = 0; i < mNumSubsets; i++)
	{
		text.append(leftSide(data) + rightSide(i, data) + std::string(": "));
		text.append(dumpLabel(i, data) + std::string("\n"));
	}
	return text;
}

int ClassifierSplitModel::numSubsets()
{

	return mNumSubsets;
}

void ClassifierSplitModel::resetDistribution(Instances *data)
{

	mDistribution = new Distribution(data, this);
}

std::vector<Instances*> ClassifierSplitModel::split(Instances *data)
{

	// Create subsets
	std::vector<Instances*> instances(mNumSubsets);
	for (int j = 0; j < mNumSubsets; j++)
	{
		instances[j] = new Instances(data, 0);
	}
	auto totalInst = data->numInstances();
	for (auto i = 0; i < totalInst; i++)
	{
		Instance *instance = data->instance(i);
		int subset = whichSubset(instance);
		if (subset > -1)
		{
			instances[subset]->add(instance);
		}
		else
		{
			std::vector<double> _weights = weights(instance);
			for (int j = 0; j < mNumSubsets; j++)
			{
				if (Utils::gr(_weights[j], 0))
				{
					instances[j]->add(instance);
					(instances[j]->lastInstance())->setWeight(_weights[j] * instance->weight());
				}
			}
		}
	}
	return instances;
}

Distribution* ClassifierSplitModel::getDistribution()
{
	return mDistribution;
}

void ClassifierSplitModel::setDistribution(Distribution* dist)
{
	mDistribution = dist;
}
