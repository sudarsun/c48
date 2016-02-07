#include "EntropyBasedSplitCrit.h"
#include "Distribution.h"
//#include "core/ContingencyTables.h"
#include <cmath>

double EntropyBasedSplitCrit::lnFunc(double num) const {

    // Constant hard coded for efficiency reasons
    if (num < 1e-6) {
        return 0;
    }
    else {
        //return ContingencyTables::lnFunc(num);
        return num * log(num) / log(2);
    }
}

double EntropyBasedSplitCrit::oldEnt(Distribution &bags) const {

    double returnValue = 0;
    int j;

    for (j = 0; j < bags.numClasses(); j++) {
        returnValue = returnValue + lnFunc(bags.perClass(j));
    }
    return lnFunc(bags.total()) - returnValue;
}

double EntropyBasedSplitCrit::newEnt(Distribution &bags) const {

    double returnValue = 0;
    int i, j;

    for (i = 0; i < bags.numBags(); i++) {
        for (j = 0; j < bags.numClasses(); j++) {
            returnValue = returnValue + lnFunc(bags.perClassPerBag(i, j));
        }
        returnValue = returnValue - lnFunc(bags.perBag(i));
    }
    return -returnValue;
}

double EntropyBasedSplitCrit::splitEnt(Distribution &bags) const {

    double returnValue = 0;
    int i;

    for (i = 0; i < bags.numBags(); i++) {
        returnValue = returnValue + lnFunc(bags.perBag(i));
    }
    return lnFunc(bags.total()) - returnValue;
}
