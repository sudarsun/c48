#include "GainRatioSplitCrit.h"
#include "Distribution.h"
//#include "core/ContingencyTables.h"
#include "core/Utils.h"

double GainRatioSplitCrit::splitCritValue(Distribution &bags) const {

    double numerator;
    double denumerator;

    numerator = oldEnt(bags) - newEnt(bags);

    // Splits with no gain are useless.
    if (Utils::eq(numerator, 0)) {
        return std::numeric_limits<double>::max();
    }
    // !Need to check here! - ok now
    denumerator = EntropyBasedSplitCrit::splitEnt(bags);

    // Test if split is trivial.
    if (Utils::eq(denumerator, 0)) {
        return std::numeric_limits<double>::max();
    }

    // We take the reciprocal value because we want to minimize the
    // splitting criterion's value.
    return denumerator / numerator;
}

double GainRatioSplitCrit::splitCritValue(Distribution &bags, double totalnoInst, double numerator) const {

    double denumerator;
    // Compute split info.
    denumerator = splitEnt(bags, totalnoInst);

    // Test if split is trivial.
    if (Utils::eq(denumerator, 0)) {
        return 0;
    }
    denumerator = denumerator / totalnoInst;

    return numerator / denumerator;
}

double GainRatioSplitCrit::splitEnt(Distribution &bags, double totalnoInst) const {

    double returnValue = 0;
    double noUnknown;
    int i;

    noUnknown = totalnoInst - bags.total();
    if (Utils::gr(bags.total(), 0)) {
        for (i = 0; i < bags.numBags(); i++) {
            returnValue = returnValue - lnFunc(bags.perBag(i));
        }
        returnValue = returnValue - lnFunc(noUnknown);
        returnValue = returnValue + lnFunc(totalnoInst);
    }
    return returnValue;
}
