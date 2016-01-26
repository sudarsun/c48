#include "Instances.h"
#include "Instance.h"
#include "Consts.h"
#include "Utils.h"

#include <iostream>
#include <unordered_set>
#include <exception>
#include <stdexcept>

Instances::Instances(const string &name, std::vector<Attribute*> &attInfo, const int capacity)
{
    // check whether the attribute names are unique
    std::unordered_set<string> attr_names;
    string nonUniqueNames = "";
    for (auto att : attInfo)
    {
        if (std::find(attr_names.begin(), attr_names.end(), att->name()) != attr_names.end())
        {
            nonUniqueNames.append(string("'") + att->name() + string("' "));
        }
        attr_names.insert(att->name());
    }
    if (attr_names.size() != attInfo.size())
    {
        throw std::invalid_argument(string("Attribute names are not unique!") + string(" Causes: ") + nonUniqueNames);
    }
    attr_names.clear();

    mRelationName = name;
    mClassIndex = -1;
    mAttributes = attInfo;
    mNamesToAttributeIndices = std::unordered_map<string, int>(static_cast<int>(numAttributes() / 0.75));
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

Instances::Instances(Instances *dataset, const int capacity)
{
    initialize(dataset, capacity);
}

Attribute *Instances::attribute(const int index) const
{
    return mAttributes[index];
}

Attribute *Instances::attribute(const string &name)
{

    int index = mNamesToAttributeIndices[name];
    if (index != -1)
    {
        return attribute(index);
    }

    return nullptr;
}

int Instances::numAttributes() const
{
    return (int)mAttributes.size();
}

int Instances::classIndex() const
{
    return mClassIndex;
}

void Instances::setClassIndex(const int classIndex)
{
    if (classIndex >= numAttributes())
    {
        throw  string("Invalid class index: ") + std::to_string(classIndex);
    }
    mClassIndex = classIndex;
}

bool Instances::add(Instance *instance)
{
    Instance *newInstance = static_cast<Instance*>(instance);
    newInstance->setDataset(const_cast<Instances*>(this));
    mInstances.push_back(instance);
    return true;
}

void Instances::add(const int index, Instance *instance)
{
    Instance *newInstance = static_cast<Instance*>(instance);
    newInstance->setDataset(this);
    mInstances[index] = instance;
}

int Instances::numInstances() const
{
    return (int)mInstances.size();
}

void Instances::copyInstances(const int from, Instances *dest, const int num)
{
    for (int i = 0; i < num; i++)
    {
        Instance *newInstance = new Instance(dest->instance(i)->weight(), dest->instance(i)->toDoubleArray());
        newInstance->setDataset(const_cast<Instances*>(this));
        mInstances.push_back(newInstance);
    }
}

Instance *Instances::instance(const int index) const
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

Attribute *Instances::classAttribute() const
{
    if (mClassIndex < 0)
    {
        throw "Class index is negative (not set)!";
    }
    return attribute(mClassIndex);
}
int Instances::numClasses() const
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

Instance *Instances::lastInstance() const
{
    return mInstances[mInstances.size() - 1];
}

void Instances::deleteWithMissing(const int attIndex)
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

double Instances::sumOfWeights() const
{
    double sum = 0;

    for (int i = 0; i < numInstances(); i++)
    {
        sum += instance(i)->weight();
    }
    return sum;
}

Instances *Instances::trainCV(const int numFolds, const int numFold)
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
    train = new Instances(this, numInstances() - numInstForFold);
    first = numFold * (numInstances() / numFolds) + offset;
    copyInstances(0, train, first);
    copyInstances(first + numInstForFold, train, numInstances() - first - numInstForFold);

    return train;
}

Instances *Instances::testCV(const int numFolds, const int numFold)
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
    test = new Instances(this, numInstForFold);
    first = numFold * (numInstances() / numFolds) + offset;
    copyInstances(first, test, numInstForFold);
    return test;
}

void Instances::Sort(const int attIndex)
{
    if (!attribute(attIndex)->isNominal())
    {
        // Use quicksort from Utils class for sorting
        double_array vals(numInstances());
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

        int_array sortOrder = Utils::sortWithNoMissingValues(vals);
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

void Instances::sortBasedOnNominalAttribute(const int attIndex)
{
    // Figure out number of instances for each attribute value
    // and store original list of instances away
    int_array counts((attribute(attIndex))->numValues());
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
    int_array indices(counts.size());
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

string Instances::getRelationName() const
{
    return mRelationName;
}

void Instances::setRelationName(const string name)
{
    mRelationName = name;
}

double_array Instances::attributeToDoubleArray(const int index) const
{
    int totalInst = numInstances();
    double_array result;
    for (int i = 0; i < totalInst; i++) {
        result.push_back(instance(i)->value(index));
    }
    return result;
}
