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

	if (mnumSubsets > 0)
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
		return static_cast<double>(mdistribution->maxClass(theSubset));
	}
	else
	{
		return static_cast<double>(mdistribution->maxClass());
	}
}

double ClassifierSplitModel::classProb(int classIndex, Instance *instance, int theSubset)
{

	if (theSubset > -1)
	{
		return mdistribution->prob(classIndex, theSubset);
	}
	else
	{
		std::vector<double> _weights = weights(instance);
		if (_weights.empty())
		{
			return mdistribution->prob(classIndex);
		}
		else
		{
			double prob = 0;
			for (int i = 0; i < (int)_weights.size(); i++)
			{
				prob += _weights[i] * mdistribution->prob(classIndex, i);
			}
			return prob;
		}
	}
}

double ClassifierSplitModel::classProbLaplace(int classIndex, Instance *instance, int theSubset)
{

	if (theSubset > -1)
	{
		return mdistribution->laplaceProb(classIndex, theSubset);
	}
	else
	{
		std::vector<double> _weights = weights(instance);
		if (_weights.empty())
		{
			return mdistribution->laplaceProb(classIndex);
		}
		else
		{
			double prob = 0;
			for (int i = 0; i < (int)_weights.size(); i++)
			{
				prob += _weights[i] * mdistribution->laplaceProb(classIndex, i);
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

	return mdistribution;
}

std::string ClassifierSplitModel::dumpLabel(int index, Instances *data)
{

	std::string text;

	text = "";
	text.append((static_cast<Instances*>(data))->classAttribute()->value(mdistribution->maxClass(index)));
	text.append(std::string(" (") + std::to_string(Utils::roundDouble(mdistribution->perBag(index), 2)));
	if (Utils::gr(mdistribution->numIncorrect(index), 0))
	{
		text.append(std::string("/") + std::to_string(Utils::roundDouble(mdistribution->numIncorrect(index), 2)));
	}
	text.append(")");

	return text;
}

std::string ClassifierSplitModel::sourceClass(int index, Instances *data)
{
	std::cout << "sourceClass";
	return std::to_string(mdistribution->maxClass(index));
}

std::string ClassifierSplitModel::dumpModel(Instances *data)
{

	std::string text;
	int i;

	text = "";
	for (i = 0; i < mnumSubsets; i++)
	{
		text.append(leftSide(data) + rightSide(i, data) + std::string(": "));
		text.append(dumpLabel(i, data) + std::string("\n"));
	}
	return text;
}

int ClassifierSplitModel::numSubsets()
{

	return mnumSubsets;
}

void ClassifierSplitModel::resetDistribution(Instances *data)
{

	mdistribution = new Distribution(data, this);
}

std::vector<Instances*> ClassifierSplitModel::split(Instances *data)
{

	// Find size and constitution of subsets
	std::vector<int> subsetSize(mnumSubsets);
	for (int i = 0; i < data->numInstances(); i++)
	{
		Instance * instance = data->instance(i);
		int subset = whichSubset(instance);
		if (subset > -1)
		{
			subsetSize[subset]++;
		}
		else
		{
			std::vector<double> _weights = weights(instance);
			for (int j = 0; j < mnumSubsets; j++)
			{
				if (Utils::gr(_weights[j], 0))
				{
					subsetSize[j]++;
				}
			}
		}
	}

	// Create subsets
	std::vector<Instances*> instances(mnumSubsets);
	for (int j = 0; j < mnumSubsets; j++)
	{
		instances[j] = new Instances(data, subsetSize[j]);
	}
	for (auto i = 0; i < data->numInstances(); i++)
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
			for (int j = 0; j < mnumSubsets; j++)
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
	return mdistribution;
}

void ClassifierSplitModel::setDistribution(Distribution* dist)
{
	mdistribution = dist;
}
