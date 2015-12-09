#include "Instance.h"
#include "Instances.h"
#include "Utils.h"

Instance::Instance(Instance *instance)
{
	mAttValues = instance->mAttValues;
	mWeight = instance->mWeight;
	mDataset = instance->mDataset;
}
Instance::Instance(double weight, std::vector<double> attValues)
{
	mAttValues = attValues;
	mWeight = weight;
	mDataset = nullptr;
}

Instance* Instance::copy()
{
	Instance *result = new Instance(this);
	result->mDataset = mDataset;
	return result;
}
Attribute *Instance::attribute(int index)
{
	return mDataset->attribute(index);
}

int Instance::numAttributes()
{
	return (int)mAttValues.size();
}
int Instance::classIndex()
{
	return mDataset->classIndex();
}

void Instance::setValue(int attIndex, double value)
{
	freshAttributeVector();
	mAttValues[attIndex] = value;
}

void Instance::freshAttributeVector()
{
	mAttValues = toDoubleArray();
}
std::vector<double> Instance::toDoubleArray()
{
	std::vector<double> newValues(mAttValues.size());
	std::copy(std::begin(mAttValues), std::end(mAttValues), std::begin(newValues));
	return newValues;
}

void Instance::setClassMissing()
{

	int classIndexValue = classIndex();
	if (classIndexValue < 0)
	{
		throw  "Class is not set!";
	}
	setMissing(classIndexValue);
}

void Instance::setMissing(int attIndex)
{
	setValue(attIndex, Utils::missingValue());
}

void Instance::setValue(int attIndex, const std::string &value)
{
	int valIndex;

	if (!attribute(attIndex)->isNominal() && !attribute(attIndex)->isString())
	{
		throw "Attribute neither nominal nor string!";
	}
	valIndex = attribute(attIndex)->indexOfValue(value);
	if (valIndex == -1)
	{
		if (attribute(attIndex)->isNominal())
		{
			throw "Value not defined for given nominal attribute!";
		}
		else
		{
			attribute(attIndex)->forceAddValue(value);
			valIndex = attribute(attIndex)->indexOfValue(value);
		}
	}
	setValue(attIndex, valIndex);
}

double Instance::value(int attIndex)
{
	return mAttValues[attIndex];
}

double Instance::classValue()
{
	int classIndexValue = classIndex();
	if (classIndexValue < 0)
	{
		throw "Class is not set!";
	}
	return value(classIndexValue);
}

double Instance::weight()
{
	return mWeight;
}

bool Instance::isMissing(int attIndex)
{

	if (Utils::isMissingValue(value(attIndex)))
	{
		return true;
	}
	return false;
}

Attribute *Instance::classAttribute()
{

	return mDataset->classAttribute();
}

int Instance::numClasses()
{
	return mDataset->numClasses();
}

void Instance::setWeight(double weight)
{
	mWeight = weight;
}

void Instance::setDataset(Instances *instances)
{
	mDataset = instances;
}

Instances* Instance::getDataset()
{
	return mDataset;
}

double Instance::missingValue()
{
	return std::numeric_limits<double>::quiet_NaN();;
}

bool Instance::classIsMissing()
{
	int classIndexValue = classIndex();
	if (classIndexValue < 0)
	{
		throw "Class is not set!";
	}
	return isMissing(classIndexValue);
}
