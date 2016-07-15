#ifndef _C45MODELSELECTION_
#define _C45MODELSELECTION_

#include "ModelSelection.h"
#include <string>
#include <vector>
#include <stdexcept>

// Forward class declarations:
class C45Split;
class Instances;
class ClassifierSplitModel;
class NoSplit;
class Distribution;
class Attribute;

/**
 * Class for selecting a C4.5-type split for a given dataset.
 */
class C45ModelSelection : public ModelSelection {

protected:
    /** Minimum number of objects in interval. */
    const int mMinNoObj;

    /** Use MDL correction? */
    const bool mUseMDLcorrection;

    /** All the training data */
    Instances *mAllData;

    /** Do not relocate split point to actual data value */
    const bool mDoNotMakeSplitPointActualValue;

public:
    /**
     * Initializes the split selection method with the given parameters.
     *
     * @param minNoObj minimum number of instances that have to occur in at least
     *          two subsets induced by split
     * @param allData FULL training dataset (necessary for selection of split
     *          points).
     * @param useMDLcorrection whether to use MDL adjustement when finding splits
     *          on numeric attributes
     * @param doNotMakeSplitPointActualValue if true, split point is not relocated
     *          by scanning the entire dataset for the closest data value
     */
    C45ModelSelection(int minNoObj, Instances &allData, bool useMDLcorrection, bool doNotMakeSplitPointActualValue);

    /**
     * Sets reference to training data to null.
     */
    virtual void cleanup() const;

    /**
     * Selects C4.5-type split for the given dataset.
     */
    ClassifierSplitModel *selectModel(Instances &data) const;

    /**
     * Selects C4.5-type split for the given dataset.
     */
    virtual ClassifierSplitModel *selectModel(Instances &train, Instances &test) const override;

	virtual void serialize(std::ostream &os);
};

#endif    //#ifndef _C45MODELSELECTION_
