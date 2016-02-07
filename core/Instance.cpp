#include "Instance.h"
#include "Instances.h"
#include "Utils.h"

Instance::Instance(Instance *instance)
{
    mAttValues = instance->mAttValues;
    mWeight = instance->mWeight;
    mDataset = instance->mDataset;
}
Instance::Instance(const double weight,const double_array &attValues)
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
Attribute &Instance::attribute(const int index) const
{
    return mDataset->attribute(index);
}

int Instance::numAttributes() const
{
    return (int)mAttValues.size();
}
int Instance::classIndex() const
{
    return mDataset->classIndex();
}

void Instance::setValue(const int attIndex, const double value)
{
    freshAttributeVector();
    mAttValues[attIndex] = value;
}

void Instance::freshAttributeVector()
{
    mAttValues = toDoubleArray();
}
double_array Instance::toDoubleArray() const
{
    double_array newValues(mAttValues.size());
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

void Instance::setMissing(const int attIndex)
{
    setValue(attIndex, Utils::missingValue());
}

void Instance::setValue(const int attIndex, const string &value)
{
    int valIndex;

    if (!attribute(attIndex).isNominal() && !attribute(attIndex).isString())
    {
        throw "Attribute neither nominal nor string!";
    }
    valIndex = attribute(attIndex).indexOfValue(value);
    if (valIndex == -1)
    {
        if (attribute(attIndex).isNominal())
        {
            throw "Value not defined for given nominal attribute!";
        }
        else
        {
            attribute(attIndex).forceAddValue(value);
            valIndex = attribute(attIndex).indexOfValue(value);
        }
    }
    setValue(attIndex, valIndex);
}

double Instance::value(const int attIndex) const
{
    return mAttValues[attIndex];
}

double Instance::classValue() const
{
    int classIndexValue = classIndex();
    if (classIndexValue < 0)
    {
        throw "Class is not set!";
    }
    return value(classIndexValue);
}

double Instance::weight() const
{
    return mWeight;
}

bool Instance::isMissing(const int attIndex) const
{
    if (Utils::isMissingValue(value(attIndex)))
    {
        return true;
    }
    return false;
}

Attribute &Instance::classAttribute() const
{
    return mDataset->classAttribute();
}

int Instance::numClasses() const
{
    return mDataset->numClasses();
}

void Instance::setWeight(const double weight)
{
    mWeight = weight;
}

void Instance::setDataset(Instances *instances)
{
    mDataset = instances;
}

Instances* Instance::getDataset() const
{
    return mDataset;
}

double Instance::missingValue() const
{
    return std::numeric_limits<double>::quiet_NaN();;
}

bool Instance::classIsMissing() const
{
    int classIndexValue = classIndex();
    if (classIndexValue < 0)
    {
        throw "Class is not set!";
    }
    return isMissing(classIndexValue);
}
