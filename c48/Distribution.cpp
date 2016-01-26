#include "Distribution.h"
#include "core/Instances.h"
#include "ClassifierSplitModel.h"
#include "core/Instance.h"
#include "core/Utils.h"
#include <iterator>

Distribution::Distribution(int numBags, int numClasses) :
    mperBag(double_array(numBags)),
    mperClass(double_array(numClasses))
{
    for (int i = 0; i < numBags; i++)
    {
        this->mperClassPerBag.push_back( double_array(numClasses));
    }
    totaL = 0;
}

Distribution::Distribution(double_2D_array &table) :
    mperClassPerBag(table),
    mperBag(double_array(table.size())),
    mperClass(double_array(table[0].size()))
{

    int i, j;

    for (i = 0; i < (int)table.size(); i++)
    {
        for (j = 0; j < (int)table[i].size(); j++)
        {
            mperBag[i] += table[i][j];
            mperClass[j] += table[i][j];
            totaL += table[i][j];
        }
    }
}

Distribution::Distribution(Instances *source) :
    mperBag(double_array(1)),
    mperClass(double_array(source->numClasses()))
{
    // Clear 
    mperClassPerBag.clear();

    totaL = 0;
    mperClassPerBag.push_back(double_array(source->numClasses()));
    int totalnum = source->numInstances();
    for (int i = 0; i < totalnum; i++)
    {
        add(0, source->instance(i));
    }
}

Distribution::Distribution(Instances *source, ClassifierSplitModel *modelToUse) :
    mperBag(double_array(modelToUse->numSubsets())),
    mperClass(double_array(source->numClasses())) {

    int index;

    double_array weights;

    // Clear 
    mperClassPerBag.clear();
    totaL = 0;
    for (int i = 0; i < modelToUse->numSubsets(); i++) {
        mperClassPerBag.push_back(double_array(source->numClasses()));
    }
    int totalnum = source->numInstances();
    for (int i = 0; i < totalnum; i++)
    {
        index = modelToUse->whichSubset(source->instance(i));
        if (index != -1) {
            add(index, source->instance(i));
        }
        else {
            weights = modelToUse->weights(source->instance(i));
            addWeights(source->instance(i), weights);
        }
    }
}

Distribution::Distribution(Distribution *toMerge) 
{
    mperClassPerBag.clear();
    mperBag.clear();
    mperClass.clear();
    totaL = toMerge->totaL;
    std::copy(toMerge->mperClass.begin(), toMerge->mperClass.end(), std::back_inserter(mperClass));
    mperClassPerBag.push_back(double_array(toMerge->numClasses()));
    std::copy(toMerge->mperClass.begin(), toMerge->mperClass.end(), std::back_inserter(mperClassPerBag[0]));
    mperBag.push_back(totaL);
}

Distribution::Distribution(Distribution *toMerge, int index) :
    mperClassPerBag(double_2D_array(1)),
    mperBag(double_array(2)),
    mperClass(double_array(toMerge->numClasses()))
{

    int i;

    totaL = toMerge->totaL;
    std::copy(toMerge->mperClass.begin(), toMerge->mperClass.end(), std::back_inserter(mperClass));
    mperClassPerBag[0] = double_array(toMerge->numClasses());
    std::copy(toMerge->mperClassPerBag[index].begin(), toMerge->mperClassPerBag[index].end(), std::back_inserter(mperClassPerBag[0]));
    mperClassPerBag[1] = double_array(toMerge->numClasses());
    for (i = 0; i < toMerge->numClasses(); i++)
    {
        mperClassPerBag[1][i] = toMerge->mperClass[i] - mperClassPerBag[0][i];
    }
    mperBag[0] = toMerge->mperBag[index];
    mperBag[1] = totaL - mperBag[0];
}

int Distribution::actualNumBags() const
{

    int returnValue = 0;
    int i;

    for (i = 0; i < (int)mperBag.size(); i++)
    {
        if (Utils::gr(mperBag[i], 0))
        {
            returnValue++;
        }
    }

    return returnValue;
}

int Distribution::actualNumClasses() const
{

    int returnValue = 0;
    int i;

    for (i = 0; i < (int)mperClass.size(); i++)
    {
        if (Utils::gr(mperClass[i], 0))
        {
            returnValue++;
        }
    }

    return returnValue;
}

int Distribution::actualNumClasses(int bagIndex) const
{

    int returnValue = 0;
    int i;

    for (i = 0; i < (int)mperClass.size(); i++)
    {
        if (Utils::gr(mperClassPerBag[bagIndex][i], 0))
        {
            returnValue++;
        }
    }

    return returnValue;
}

void Distribution::add(int bagIndex, Instance *instance)
{

    int classIndex;
    double weight;

    classIndex = static_cast<int>(instance->classValue());
    weight = instance->weight();
    mperClassPerBag[bagIndex][classIndex] = mperClassPerBag[bagIndex][classIndex] + weight;
    mperBag[bagIndex] = mperBag[bagIndex] + weight;
    mperClass[classIndex] = mperClass[classIndex] + weight;
    totaL = totaL + weight;
}

void Distribution::sub(int bagIndex, Instance *instance)
{

    int classIndex;
    double weight;

    classIndex = static_cast<int>(instance->classValue());
    weight = instance->weight();
    mperClassPerBag[bagIndex][classIndex] = mperClassPerBag[bagIndex][classIndex] - weight;
    mperBag[bagIndex] = mperBag[bagIndex] - weight;
    mperClass[classIndex] = mperClass[classIndex] - weight;
    totaL = totaL - weight;
}

void Distribution::add(int bagIndex, double_array &counts)
{

    double sum = Utils::sum(counts);

    for (int i = 0; i < (int)counts.size(); i++)
    {
        mperClassPerBag[bagIndex][i] += counts[i];
    }
    mperBag[bagIndex] = mperBag[bagIndex] + sum;
    for (int i = 0; i < (int)counts.size(); i++)
    {
        mperClass[i] = mperClass[i] + counts[i];
    }
    totaL = totaL + sum;
}

void Distribution::addInstWithUnknown(Instances *source, int attIndex)
{

    double_array probs;
    double weight, newWeight;
    int classIndex;
    int j;

    probs = double_array(mperBag.size());
    for (j = 0; j < (int)mperBag.size(); j++)
    {
        if (Utils::eq(totaL, 0))
        {
            probs[j] = 1.0 / probs.size();
        }
        else
        {
            probs[j] = mperBag[j] / totaL;
        }
    }
    int totalnum = source->numInstances();
    for (int i = 0; i < totalnum; i++)
    {

        if (source->instance(i)->isMissing(attIndex))
        {
            classIndex = static_cast<int>(source->instance(i)->classValue());
            weight = source->instance(i)->weight();
            mperClass[classIndex] = mperClass[classIndex] + weight;
            totaL = totaL + weight;
            for (j = 0; j < (int)mperBag.size(); j++)
            {
                newWeight = probs[j] * weight;
                mperClassPerBag[j][classIndex] = mperClassPerBag[j][classIndex] + newWeight;
                mperBag[j] = mperBag[j] + newWeight;
            }
        }
    }

}

void Distribution::addRange(int bagIndex, Instances *source, int startIndex, int lastPlusOne)
{

    double sumOfWeights = 0;
    int classIndex;
    Instance *instance;
    int i;

    for (i = startIndex; i < lastPlusOne; i++)
    {
        instance = source->instance(i);
        classIndex = static_cast<int>(instance->classValue());
        sumOfWeights = sumOfWeights + instance->weight();
        mperClassPerBag[bagIndex][classIndex] += instance->weight();
        mperClass[classIndex] += instance->weight();
    }
    mperBag[bagIndex] += sumOfWeights;
    totaL += sumOfWeights;
}

void Distribution::addWeights(Instance *instance, double_array &weights)
{

    int classIndex;
    int i;

    classIndex = static_cast<int>(instance->classValue());
    for (i = 0; i < (int)mperBag.size(); i++)
    {
        double weight = instance->weight() * weights[i];
        mperClassPerBag[i][classIndex] = mperClassPerBag[i][classIndex] + weight;
        mperBag[i] = mperBag[i] + weight;
        mperClass[classIndex] = mperClass[classIndex] + weight;
        totaL = totaL + weight;
    }
}

bool Distribution::check(double minNoObj) const
{

    int counter = 0;
    int i;

    for (i = 0; i < (int)mperBag.size(); i++)
    {
        if (Utils::grOrEq(mperBag[i], minNoObj))
        {
            counter++;
        }
    }
    if (counter > 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*void *Distribution::clone()
{

    int i, j;

    Distribution *newDistribution = new Distribution((int)mperBag.size(), (int)mperClass.size());
    for (i = 0; i < (int)mperBag.size(); i++)
    {
        newDistribution->mperBag[i] = mperBag[i];
        for (j = 0; j < (int)mperClass.size(); j++)
        {
            newDistribution->mperClassPerBag[i][j] = mperClassPerBag[i][j];
        }
    }
    for (j = 0; j < (int)mperClass.size(); j++)
    {
        newDistribution->mperClass[j] = mperClass[j];
    }
    newDistribution->totaL = totaL;

    return newDistribution;
}*/

void Distribution::del(int bagIndex, Instance *instance)
{

    int classIndex;
    double weight;

    classIndex = static_cast<int>(instance->classValue());
    weight = instance->weight();
    mperClassPerBag[bagIndex][classIndex] = mperClassPerBag[bagIndex][classIndex] - weight;
    mperBag[bagIndex] = mperBag[bagIndex] - weight;
    mperClass[classIndex] = mperClass[classIndex] - weight;
    totaL = totaL - weight;
}

void Distribution::delRange(int bagIndex, Instances *source, int startIndex, int lastPlusOne)
{

    double sumOfWeights = 0;
    int classIndex;
    Instance *instance;
    int i;

    for (i = startIndex; i < lastPlusOne; i++)
    {
        instance = source->instance(i);
        classIndex = static_cast<int>(instance->classValue());
        sumOfWeights = sumOfWeights + instance->weight();
        mperClassPerBag[bagIndex][classIndex] -= instance->weight();
        mperClass[classIndex] -= instance->weight();
    }
    mperBag[bagIndex] -= sumOfWeights;
    totaL -= sumOfWeights;
}

string Distribution::dumpDistribution() const
{

    string text;
    int i, j;

    text = "";
    for (i = 0; i < (int)mperBag.size(); i++)
    {
        text.append(string("Bag num ") + std::to_string(i) + string("\n"));
        for (j = 0; j < (int)mperClass.size(); j++)
        {
            text.append(string("Class num ") + std::to_string(j) + string(" ") + std::to_string(mperClassPerBag[i][j]) + string("\n"));
        }
    }
    return text;
}

void Distribution::initialize()
{

    for (int i = 0; i < (int)mperClass.size(); i++)
    {
        mperClass[i] = 0;
    }
    for (int i = 0; i < (int)mperBag.size(); i++)
    {
        mperBag[i] = 0;
    }
    for (int i = 0; i < (int)mperBag.size(); i++)
    {
        for (int j = 0; j < (int)mperClass.size(); j++)
        {
            mperClassPerBag[i][j] = 0;
        }
    }
    totaL = 0;
}

double_2D_array Distribution::matrix()
{

    return mperClassPerBag;
}

int Distribution::maxBag()  const
{

    double max;
    int maxIndex;
    int i;

    max = 0;
    maxIndex = -1;
    for (i = 0; i < (int)mperBag.size(); i++)
    {
        if (Utils::grOrEq(mperBag[i], max))
        {
            max = mperBag[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

int Distribution::maxClass() const
{

    double maxCount = 0;
    int maxIndex = 0;
    int i;

    for (i = 0; i < (int)mperClass.size(); i++)
    {
        if (Utils::gr(mperClass[i], maxCount))
        {
            maxCount = mperClass[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

int Distribution::maxClass(int index) const
{

    double maxCount = 0;
    int maxIndex = 0;
    int i;

    if (Utils::gr(mperBag[index], 0))
    {
        for (i = 0; i < (int)mperClass.size(); i++)
        {
            if (Utils::gr(mperClassPerBag[index][i], maxCount))
            {
                maxCount = mperClassPerBag[index][i];
                maxIndex = i;
            }
        }
        return maxIndex;
    }
    else
    {
        return maxClass();
    }
}

int Distribution::numBags() const
{

    return (int)mperBag.size();
}

int Distribution::numClasses() const
{

    return (int)mperClass.size();
}

double Distribution::numCorrect() const
{

    return mperClass[maxClass()];
}

double Distribution::numCorrect(int index) const
{

    return mperClassPerBag[index][maxClass(index)];
}

double Distribution::numIncorrect() const
{

    return totaL - numCorrect();
}

double Distribution::numIncorrect(int index) const
{

    return mperBag[index] - numCorrect(index);
}

double Distribution::perClassPerBag(int bagIndex, int classIndex) const
{

    return mperClassPerBag[bagIndex][classIndex];
}

double Distribution::perBag(int bagIndex) const
{

    return mperBag[bagIndex];
}

double Distribution::perClass(int classIndex) const
{

    return mperClass[classIndex];
}

double Distribution::laplaceProb(int classIndex) const
{

    return (mperClass[classIndex] + 1) / (totaL + mperClass.size());
}

double Distribution::laplaceProb(int classIndex, int intIndex) const
{

    if (Utils::gr(mperBag[intIndex], 0))
    {
        return (mperClassPerBag[intIndex][classIndex] + 1.0) / (mperBag[intIndex] + mperClass.size());
    }
    else
    {
        return laplaceProb(classIndex);
    }

}

double Distribution::prob(int classIndex) const
{

    if (!Utils::eq(totaL, 0))
    {
        return mperClass[classIndex] / totaL;
    }
    else
    {
        return 0;
    }
}

double Distribution::prob(int classIndex, int intIndex) const
{

    if (Utils::gr(mperBag[intIndex], 0))
    {
        return mperClassPerBag[intIndex][classIndex] / mperBag[intIndex];
    }
    else
    {
        return prob(classIndex);
    }
}

Distribution *Distribution::subtract(Distribution *toSubstract)  const
{

    Distribution *newDist = new Distribution(1, (int)mperClass.size());

    newDist->mperBag[0] = totaL - toSubstract->totaL;
    newDist->totaL = newDist->mperBag[0];
    for (int i = 0; i < (int)mperClass.size(); i++)
    {
        newDist->mperClassPerBag[0][i] = mperClass[i] - toSubstract->mperClass[i];
        newDist->mperClass[i] = newDist->mperClassPerBag[0][i];
    }
    return newDist;
}

double Distribution::total() const
{
    return totaL;
}

void Distribution::shift(int from, int to, Instance *instance)
{

    int classIndex;
    double weight;

    classIndex = static_cast<int>(instance->classValue());
    weight = instance->weight();
    mperClassPerBag[from][classIndex] -= weight;
    mperClassPerBag[to][classIndex] += weight;
    mperBag[from] -= weight;
    mperBag[to] += weight;
}

void Distribution::shiftRange(int from, int to, Instances *source, int startIndex, int lastPlusOne)
{

    int classIndex;
    double weight;
    Instance *instance;
    int i;

    for (i = startIndex; i < lastPlusOne; i++)
    {
        instance = source->instance(i);
        classIndex = static_cast<int>(instance->classValue());
        weight = instance->weight();
        mperClassPerBag[from][classIndex] -= weight;
        mperClassPerBag[to][classIndex] += weight;
        mperBag[from] -= weight;
        mperBag[to] += weight;
    }
}
