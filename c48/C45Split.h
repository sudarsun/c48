#ifndef _C45SPLIT_
#define _C45SPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

// Forward class declarations:
class Instances;
class InfoGainSplitCrit;
class GainRatioSplitCrit;
class Instance;
class Distribution;


/**
 * Class implementing a C4.5-type split on an attribute.
 */
class C45Split : public ClassifierSplitModel {

protected:
    /** Desired number of branches. */

    int mComplexityIndex = 0;

    /** Attribute to split on. */
    const int mAttIndex;

    /** Minimum number of objects in a split. */
    const int mMinNoObj;

    /** Use MDL correction? */
    const bool mUseMDLcorrection;

    /** Value of split point. */
    double mSplitPoint = 0;

    /** InfoGain of split. */
    double mInfoGain = 0;

    /** GainRatio of split. */
    double mGainRatio = 0;

    /** The sum of the weights of the instances. */
    const double mSumOfWeights;

    /** Number of split points. */
    int mIndex = 0;

    /** Static reference to splitting criterion. */
    static InfoGainSplitCrit *infoGainCrit;

    /** Static reference to splitting criterion. */
    static GainRatioSplitCrit *gainRatioCrit;

public:

    /** Constructor that initializes the split model. */

    C45Split(int attIndex, int minNoObj, double sumOfWeights, bool useMDLcorrection);

    /**
     * Creates a C4.5-type split on the given data. Assumes that none of the class
     * values is missing.
     *
     * @exception Exception if something goes wrong
     */
    virtual void buildClassifier(Instances trainInstances);

    /**
     * Returns index of attribute for which split was generated.
     */
    int attIndex() const;

    /**
     * Returns the split point (numeric attribute only).
     *
     * @return the split point used for a test on a numeric attribute
     */
    double splitPoint() const;

    /**
     * Gets class probability for instance.
     *
     * @exception Exception if something goes wrong
     */
    double classProb(const int classIndex, Instance *instance, const int theSubset) const;

    /**
     * Returns coding cost for split (used in rule learner).
     */
    double codingCost() const;

    /**
     * Returns (C4.5-type) gain ratio for the generated split.
     */
    double gainRatio() const;

    /**
     * Returns (C4.5-type) information gain for the generated split.
     */
    double infoGain() const;

    /**
     * Prints left side of condition..
     *
     * @param data training set.
     */
    string leftSide(Instances *data) const override;

    /**
     * Prints the condition satisfied by instances in a subset.
     *
     * @param index of subset
     * @param data training set.
     */
    string rightSide(const int index, Instances *data) const override;

    /**
     * Sets split point to greatest value in given data smaller or equal to old
     * split point. (C4.5 does this for some strange reason).
     *
     * @param data training set.
     */
    void setSplitPoint(Instances *allInstances);

    /**
     * Returns the minsAndMaxs of the index.th subset.
     *
     * @param data training set.
     * @param minsAndMaxs min and max in 2D array.
     * @param index index.
     */
    double_2D_array minsAndMaxs(Instances *data, const double_2D_array &minsAndMaxs, int index) const;

    /**
     * Sets distribution associated with model.
     */
    virtual void resetDistribution(Instances *data);

    /**
     * Returns weights if instance is assigned to more than one subset. Returns
     * null if instance is only assigned to one subset.
     */
    double_array weights(Instance *instance) const;

    /**
     * Returns index of subset instance is assigned to. Returns -1 if instance is
     * assigned to more than one subset.
     *
     * @exception Exception if something goes wrong
     */
    const int whichSubset(Instance *instance) const;

private:
    /**
     * Creates split on enumerated attribute.
     *
     * @exception Exception if something goes wrong
     */
    void handleEnumeratedAttribute(Instances *trainInstances);

    /**
     * Creates split on numeric attribute.
     *
     * @exception Exception if something goes wrong
     */
    void handleNumericAttribute(Instances *trainInstances);

};


#endif    //#ifndef _C45SPLIT_
