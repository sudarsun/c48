#include "InfoGainSplitCrit.h"
#include "Distribution.h"
#include "core/Utils.h"

double InfoGainSplitCrit::splitCritValue(Distribution *bags) const {

    double numerator;

    numerator = oldEnt(bags) - newEnt(bags);

    // Splits with no gain are useless.
    if (Utils::eq(numerator, 0)) {
        return std::numeric_limits<double>::max();
    }

    // We take the reciprocal value because we want to minimize the
    // splitting criterion's value.
    return bags->total() / numerator;
}

double InfoGainSplitCrit::splitCritValue(Distribution *bags, double totalNoInst) const {

    double numerator, noUnknown, unknownRate;
    noUnknown = totalNoInst - bags->total();
    unknownRate = noUnknown / totalNoInst;
    numerator = (oldEnt(bags) - newEnt(bags));
    numerator = (1 - unknownRate) * numerator;

    // Splits with no gain are useless.
    if (Utils::eq(numerator, 0)) {
        return 0;
    }

    return numerator / bags->total();
}

double InfoGainSplitCrit::splitCritValue(Distribution *bags, double totalNoInst, double oldEnt) const {

    double numerator, noUnknown, unknownRate;
    noUnknown = totalNoInst - bags->total();
    unknownRate = noUnknown / totalNoInst;
    numerator = (oldEnt - newEnt(bags));
    numerator = (1 - unknownRate) * numerator;

    // Splits with no gain are useless.
    if (Utils::eq(numerator, 0)) {
        return 0;
    }

    return numerator / bags->total();
}

