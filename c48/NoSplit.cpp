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

int NoSplit::whichSubset(Instance *instance)
{

	return 0;
}

double_array NoSplit::weights(Instance *instance)
{

	return double_array(0);
}

string NoSplit::leftSide(Instances *instances)
{

	return "";
}

string NoSplit::rightSide(int index, Instances *instances)
{

	return "";
}

string NoSplit::sourceExpression(int index, Instances *data)
{

	return "true"; // or should this be false??
}

