#include "NoSplit.h"
#include "Distribution.h"
#include "core/Instances.h"
#include "core/Instance.h"


NoSplit::NoSplit( Distribution *distribution )
{

    mdistribution = new Distribution( distribution );
    mnumSubsets = 1;
}

void NoSplit::buildClassifier( Instances *instances )
{

    mdistribution = new Distribution( instances );
    mnumSubsets = 1;
}

int NoSplit::whichSubset( Instance *instance )
{

    return 0;
}

std::vector<double> NoSplit::weights( Instance *instance )
{

    return std::vector<double>(0);
}

std::string NoSplit::leftSide( Instances *instances )
{

    return "";
}

std::string NoSplit::rightSide( int index, Instances *instances )
{

    return "";
}

std::string NoSplit::sourceExpression( int index, Instances *data )
{

    return "true"; // or should this be false??
}

