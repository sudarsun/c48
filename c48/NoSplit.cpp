#include "NoSplit.h"
#include "Distribution.h"
#include "core/Instances.h"
#include "core/Instance.h"


NoSplit::NoSplit(Distribution *distribution)
{
    mDistribution = new Distribution(distribution);
    mNumSubsets = 1;
}

void NoSplit::buildClassifier(Instances instances)
{
    mDistribution = new Distribution(&instances);
    mNumSubsets = 1;
}

const int NoSplit::whichSubset(Instance *instance) const
{
    return 0;
}

double_array NoSplit::weights(Instance *instance) const
{
    return double_array(0);
}

string NoSplit::leftSide(Instances *instances) const
{
    return "";
}

string NoSplit::rightSide(const int index, Instances *instances) const
{
    return "";
}

string NoSplit::sourceExpression(const int index, Instances *data)
{
    return "true"; // or should this be false??
}

