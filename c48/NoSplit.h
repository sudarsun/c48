#ifndef _NOSPLIT_
#define _NOSPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <stdexcept>

class Distribution;
class Instances;
class Instance;

/**
 * Class implementing a "no-split"-split.
 *
 */
class NoSplit : public ClassifierSplitModel {

public:
    /**
     * Creates "no-split"-split for given distribution.
     */
    NoSplit(Distribution *distribution);

    /**
     * Creates a "no-split"-split for a given set of instances.
     *
     * @exception Exception if split can't be built successfully
     */
    virtual void buildClassifier(Instances instances);

    /**
     * Always returns 0 because only there is only one subset.
     */
    const int whichSubset(Instance *instance) const override;

    /**
     * Always returns null because there is only one subset.
     */
    double_array weights(Instance *instance) const override;

    /**
     * Does nothing because no condition has to be satisfied.
     */
    string leftSide(Instances *instances) const override;

    /**
     * Does nothing because no condition has to be satisfied.
     */
    string rightSide(const int index, Instances *instances) const override;

    /**
     * Returns a string containing java source code equivalent to the test
     * made at this node. The instance being tested is called "i".
     *
     * @param index index of the nominal value tested
     * @param data the data containing instance structure info
     * @return a value of type 'String'
     */
    string sourceExpression(const int index, Instances *data);

};

#endif    //#ifndef _NOSPLIT_
