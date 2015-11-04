#include "DenseInstance.h"
#include "Instance.h"
#include "Attribute.h"
#include "Utils.h"

DenseInstance::DenseInstance(double weight, std::vector<double> &attValues)
{
	mAttValues = attValues;
	mWeight = weight;
	//mDataset = nullptr;
}

Attribute *DenseInstance::attribute(int index)
{
	//return m_Dataset->attribute( index );
	return nullptr;
}

int DenseInstance::numAttributes()
{
	return mAttValues.size();
}
int DenseInstance::classIndex()
{
	// @pure@
	return 0;
}

void DenseInstance::setValue(int attIndex, double value)
{
	freshAttributeVector();
	mAttValues[attIndex] = value;
}

void DenseInstance::freshAttributeVector()
{
	mAttValues = toDoubleArray();
}
std::vector<double> DenseInstance::toDoubleArray()
{
	std::vector<double> newValues(mAttValues.size());
	std::copy(std::begin(mAttValues), std::end(mAttValues), std::begin(newValues));
	return newValues;
}

void DenseInstance::setClassMissing()
{

	int classIndexValue = classIndex();
	if (classIndexValue < 0)
	{
		throw  "Class is not set!";
	}
	setMissing(classIndexValue);
}

void DenseInstance::setMissing(int attIndex)
{

	setValue(attIndex, Utils::missingValue());
}

void DenseInstance::setValue(int attIndex, const std::string &value)
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

double DenseInstance::value(int attIndex)
{
	// @pure@

	return mAttValues[attIndex];
}

double DenseInstance::classValue()
{
	int classIndexValue = classIndex();
	if (classIndexValue < 0)
	{
		throw "Class is not set!";
	}
	return value(classIndexValue);
}

double DenseInstance::weight()
{
	return mWeight;
}

bool DenseInstance::isMissing(int attIndex)
{

	if (Utils::isMissingValue(value(attIndex)))
	{
		return true;
	}
	return false;
}

Attribute *DenseInstance::classAttribute()
{
	int classIndexValue = classIndex();
	if (classIndexValue < 0)
	{
		throw "Class index is negative (not set)!";
	}
	return attribute(classIndexValue);
}
int DenseInstance::numClasses()
{
	int classIndexValue = classIndex();
	if (classIndexValue < 0)
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
void DenseInstance::setWeight(double weight)
{

	mWeight = weight;
}

double DenseInstance::value(int attIndex)
{
	return mAttValues[attIndex];
}