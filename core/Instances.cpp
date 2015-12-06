#include "Instances.h"
#include "DenseInstance.h"
#include "Consts.h"
#include "Utils.h"

#include <iostream>
#include <unordered_set>
#include <exception>
#include <stdexcept>


Instances::Instances(const std::string &name, std::vector<Attribute*> &attInfo, int capacity)
{
	// check whether the attribute names are unique
	std::unordered_set<std::string> attr_names;
	std::string nonUniqueNames = "";
	for (auto att : attInfo)
	{
		if (std::find(attr_names.begin(), attr_names.end(), att->name()) != attr_names.end())
		{
			nonUniqueNames.append(std::string("'") + att->name() + std::string("' "));
		}
		attr_names.insert(att->name());
	}
	if (attr_names.size() != attInfo.size())
	{
		throw std::invalid_argument(std::string("Attribute names are not unique!") + std::string(" Causes: ") + nonUniqueNames);
	}
	attr_names.clear();

	mRelationName = name;
	mClassIndex = -1;
	mAttributes = attInfo;
	mNamesToAttributeIndices = std::unordered_map<std::string, int>(static_cast<int>(numAttributes() / 0.75));
	for (int i = 0; i < numAttributes(); i++)
	{
		(attribute(i))->setIndex(i);
		mNamesToAttributeIndices[(attribute(i))->name()] = i;
	}
	mInstances = std::vector<Instance*>(capacity);
}

Instances::Instances(Instances *dataset) :Instances(dataset, 0)
{
	this->copyInstances(0, dataset, dataset->numInstances());
}

Instances::Instances(Instances *dataset, int capacity)
{
	initialize(dataset, capacity);
}

Attribute *Instances::attribute(int index)
{
	// @pure@

	return mAttributes[index];
}

Attribute *Instances::attribute(const std::string &name)
{
	// @pure@

	int index = mNamesToAttributeIndices[name];
	if (index != -1)
	{
		return attribute(index);
	}

	return nullptr;
}

int Instances::numAttributes()
{
	return (int)mAttributes.size();
}

int Instances::classIndex()
{
	return mClassIndex;
}

void Instances::setClassIndex(int classIndex)
{
	if (classIndex >= numAttributes())
	{
		throw  std::string("Invalid class index: ") + std::to_string(classIndex);
	}
	mClassIndex = classIndex;
}

bool Instances::add(Instance *instance)
{
	Instance *newInstance = static_cast<Instance*>( instance );
	newInstance->setDataset(const_cast<Instances*>(this));
	mInstances.push_back(instance);

	return true;
}

void Instances::add(int index, Instance *instance)
{
	// @non_null@

	Instance *newInstance = static_cast<Instance*>( instance );

	newInstance->setDataset( this );
	mInstances[index] = instance ;
}

int Instances::numInstances()
{
	return (int)mInstances.size();
}

void Instances::copyInstances(int from, Instances *dest, int num)
{
	// @non_null@

	for (int i = 0; i < num; i++)
	{
		Instance *newInstance = static_cast<Instance*>(new DenseInstance(static_cast<DenseInstance*>(dest->instance(i))->weight(), static_cast<DenseInstance*>(dest->instance(i))->toDoubleArray()));
		newInstance->setDataset(const_cast<Instances*>(this));
		mInstances.push_back(newInstance);
	}
}

Instance *Instances::instance(int index)
{
	return mInstances[index];
}

void Instances::initialize(Instances *dataset, int capacity)
{
	if (capacity < 0)
	{
		capacity = 0;
	}

	// Strings only have to be "shallow" copied because
	// they can't be modified.
	mClassIndex = dataset->mClassIndex;
	mRelationName = dataset->mRelationName;
	mAttributes = dataset->mAttributes;
	mNamesToAttributeIndices = dataset->mNamesToAttributeIndices;
	mInstances = std::vector<Instance*>(capacity);
}

Attribute *Instances::classAttribute()
{
	if (mClassIndex < 0)
	{
		throw "Class index is negative (not set)!";
	}
	return attribute(mClassIndex);
}
int Instances::numClasses()
{
	if (mClassIndex < 0)
	{
		throw "Class index is negative (not set)!";
	}
	if (!classAttribute()->isNominal())
	{
		return 1;
	}
	else
	{
		return classAttribute()->numValues();
	}
}
double Instances::classValue()
{

	return 0.0;
}

double Instances::weight()
{
	return 0.0;
}
bool Instances::isMissing(int attIndex)
{
	return false;
}

Instance *Instances::lastInstance()
{
	return mInstances[mInstances.size() - 1];
}

void Instances::setWeight(double weight)
{

	mWeight = weight;
}

void Instances::deleteWithMissing(int attIndex)
{

	std::vector<Instance*> newInstances;
	int totalInst = numInstances();
	for (int i = 0; i < totalInst; i++)
	{
		if (!instance(i)->isMissing(attIndex))
		{
			newInstances.push_back(instance(i));
		}
	}
	mInstances = newInstances;
}

void Instances::deleteWithMissing(Attribute *att)
{
	deleteWithMissing(att->index());
}

void Instances::deleteWithMissingClass()
{

	if (mClassIndex < 0)
	{
		throw "Class index is negative (not set)!";
	}
	deleteWithMissing(mClassIndex);
}

double Instances::sumOfWeights()
{
	// @pure@

	double sum = 0;

	for (int i = 0; i < numInstances(); i++)
	{
		sum += instance(i)->weight();
	}
	return sum;
}

double Instances::value(int attIndex)
{
	std::cout << "should call DenseInstance value()";
	return 0;
}
std::string Instances::toString()
{
	return "";
}

Instances *Instances::trainCV(int numFolds, int numFold)
{

	int numInstForFold, first, offset;
	Instances *train;

	if (numFolds < 2)
	{
		throw std::invalid_argument("Number of folds must be at least 2!");
	}
	if (numFolds > numInstances())
	{
		throw "Can't have more folds than instances!";
	}
	numInstForFold = numInstances() / numFolds;
	if (numFold < numInstances() % numFolds)
	{
		numInstForFold++;
		offset = numFold;
	}
	else
	{
		offset = numInstances() % numFolds;
	}
	train = new Instances( this, numInstances() - numInstForFold );
	first = numFold * (numInstances() / numFolds) + offset;
	copyInstances(0, train, first);
	copyInstances(first + numInstForFold, train, numInstances() - first - numInstForFold);

	return train;
}

Instances *Instances::testCV(int numFolds, int numFold)
{

	int numInstForFold, first, offset;
	Instances *test;

	if (numFolds < 2)
	{
		throw std::invalid_argument("Number of folds must be at least 2!");
	}
	if (numFolds > numInstances())
	{
		throw "Can't have more folds than instances!";
	}
	numInstForFold = numInstances() / numFolds;
	if (numFold < numInstances() % numFolds)
	{
		numInstForFold++;
		offset = numFold;
	}
	else
	{
		offset = numInstances() % numFolds;
	}
	test = new Instances( this, numInstForFold );
	first = numFold * (numInstances() / numFolds) + offset;
	copyInstances(first, test, numInstForFold);
	return test;
}

void Instances::Sort(int attIndex)
{

	if (!attribute(attIndex)->isNominal())
	{

		// Use quicksort from Utils class for sorting
		std::vector<double> vals(numInstances());
		std::vector<Instance*> backup(vals.size());
		for (int i = 0; i < vals.size(); i++)
		{
			Instance *inst = instance(i);
			backup[i] = inst;
			double val = inst->value(attIndex);
			if (Utils::isMissingValue(val))
			{
				vals[i] = std::numeric_limits<double>::max();
			}
			else
			{
				vals[i] = val;
			}
		}

		std::vector<int> sortOrder = Utils::sortWithNoMissingValues(vals);
		for (int i = 0; i < vals.size(); i++)
		{
			mInstances[i] = backup[sortOrder[i]];
		}
	}
	else
	{
		sortBasedOnNominalAttribute(attIndex);
	}
}

void Instances::Sort(Attribute *att)
{

	Sort(att->index());
}


void Instances::sortBasedOnNominalAttribute(int attIndex)
{

	// Figure out number of instances for each attribute value
	// and store original list of instances away
	std::vector<int> counts((attribute(attIndex))->numValues());
	std::vector<Instance*> backup(numInstances());
	int j = 0;
	for (auto inst : mInstances)
	{
		backup[j++] = inst;
		if (!inst->isMissing(attIndex))
		{
			counts[static_cast<int>(inst->value(attIndex))]++;
		}
	}

	// Indices to figure out where to add instances
	std::vector<int> indices(counts.size());
	int start = 0;
	for (int i = 0; i < counts.size(); i++)
	{
		indices[i] = start;
		start += counts[i];
	}
	for (auto inst : backup)
	{
		// Use backup here
		if (!inst->isMissing(attIndex))
		{
			mInstances[indices[static_cast<int>(inst->value(attIndex))]++] = inst;
		}
		else
		{
			mInstances[start++] = inst;
		}
	}
}

Instances* Instances::getDataset()
{
	return this;
}

std::string Instances::getRelationName()
{
	return mRelationName;
}

void Instances::setRelationName(const std::string name)
{
	mRelationName = name;
}

void Instances::setDataset(Instances *instances) {

}
bool Instances::classIsMissing()
{
	return false;
}

double Instances:: missingValue()
{
	return 0.0;
}

std::vector<double> Instances::attributeToDoubleArray(int index)
{
	int totalInst = numInstances();
	std::vector<double> result;
	for (int i = 0; i < totalInst; i++) {
		result.push_back(instance(i)->value(index));
	}
	return result;
}

void Instances::setClassMissing()
{
	throw "Error!..Class is not set!";
}
