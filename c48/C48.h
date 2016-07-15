#ifndef _C48_
#define _C48_

#include "AbstractClassifier.h"
#include "C45PruneableClassifierTree.h"
#include "ClassifierTree.h"
#include "core/Instance.h"
#include "core/Instances.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

class ClassifierTree;
class ModelSelection;
class C45ModelSelection;

/**
 * Class for generating a pruned or unpruned C4.5 decision tree. For more information, see
 *
 * Ross Quinlan (1993). C4.5: Programs for Machine Learning. Morgan Kaufmann Publishers, San Mateo, CA.
 *
 */
class C48 : public AbstractClassifier
{

protected:

    /** The decision tree */
	//std::unique_ptr<ClassifierTree> mRoot;
	ClassifierTree *mRoot;

    /** Unpruned tree? */
    bool mUnpruned;

    /** Collapse tree? */
    bool mCollapseTree;

    /** Confidence level */
    float mCF;

    /** Minimum number of instances */
    int mMinNumObj;

    /** Use MDL correction? */
    bool mUseMDLcorrection;

    /** Determines whether probabilities are smoothed using Laplace correction when predictions are generated */
    bool mUseLaplace;

    /** Use reduced error pruning? */
    bool mReducedErrorPruning;

    /** Number of folds for reduced error pruning. */
    int mNumFolds;

    /** Subtree raising to be performed? */
    bool mSubtreeRaising;

    /** Cleanup after the tree has been built. */
    bool mNoCleanup;

    /** Do not relocate split point to actual data value */
    bool mDoNotMakeSplitPointActualValue;

public:

    /**
     * Constructor
     */
    C48();

    /**
    * Destructor
    */
    ~C48();

    /**
     * Generates the classifier.
     *
     * @param instances the data to train the classifier with
     * @throws Exception if classifier can't be built successfully
     */
    virtual void buildClassifier(Instances &instances);

    /**
     * Classifies an instance.
     *
     * @param instance the instance to classify
     * @return the classification for the instance
     * @throws Exception if instance can't be classified successfully
     */
    virtual double classifyInstance(Instance &instance) const;

    /**
     * Returns class probabilities for an instance.
     *
     * @param instance the instance to calculate the class probabilities for
     * @return the class probabilities
     * @throws Exception if distribution can't be computed successfully
     */
    double_array distributionForInstance(Instance &instance) const;

    /**
     * Get the value of useLaplace.
     *
     * @return Value of useLaplace.
     */
    virtual bool getUseLaplace() const;

    /**
     * Set the value of useLaplace.
     *
     * @param newuseLaplace Value to assign to useLaplace.
     */
    virtual void setUseLaplace(bool newuseLaplace);

    /**
     * Get the value of useMDLcorrection.
     *
     * @return Value of useMDLcorrection.
     */
    virtual bool getUseMDLcorrection() const;

    /**
     * Set the value of useMDLcorrection.
     *
     * @param newuseMDLcorrection Value to assign to useMDLcorrection.
     */
    virtual void setUseMDLcorrection(bool newuseMDLcorrection);

    /**
     * Returns a description of the classifier.
     *
     * @param isDumpTree Value to display the tree structure.
     * @return a description of the classifier
     */
    virtual string toString(bool isDumpTree = false) const;

    /**
     * Returns a superconcise version of the model
     *
     * @return a summary of the model
     */
    virtual string toSummaryString() const;

    /**
     * Returns the size of the tree
     *
     * @return the size of the tree
     */
    virtual double measureTreeSize() const;

    /**
     * Returns the number of leaves
     *
     * @return the number of leaves
     */
    virtual double measureNumLeaves() const;

    /**
     * Returns the number of rules (same as number of leaves)
     *
     * @return the number of rules
     */
    virtual double measureNumRules() const;

    /**
     * Get the value of unpruned.
     *
     * @return Value of unpruned.
     */
    virtual bool getUnpruned() const;

    /**
     * Set the value of unpruned. Turns reduced-error pruning off if set.
     *
     * @param v Value to assign to unpruned.
     */
    virtual void setUnpruned(bool v);

    /**
      * Get the value of collapseTree.
      *
      * @return Value of collapseTree.
      */
    virtual bool getCollapseTree() const;

    /**
     * Set the value of collapseTree.
     *
     * @param v Value to assign to collapseTree.
     */
    virtual void setCollapseTree(bool v);

    /**
     * Get the value of CF.
     *
     * @return Value of CF.
     */
    virtual float getConfidenceFactor() const;

    /**
     * Set the value of CF.
     *
     * @param v Value to assign to CF.
     */
    virtual void setConfidenceFactor(float v);

    /**
     * Get the value of minNumObj.
     *
     * @return Value of minNumObj.
     */
    virtual int getMinNumObj() const;

    /**
     * Set the value of minNumObj.
     *
     * @param v Value to assign to minNumObj.
     */
    virtual void setMinNumObj(int v);

    /**
     * Get the value of reducedErrorPruning.
     *
     * @return Value of reducedErrorPruning.
     */
    virtual bool getReducedErrorPruning() const;

    /**
     * Set the value of reducedErrorPruning. Turns unpruned trees off if set.
     *
     * @param v Value to assign to reducedErrorPruning.
     */
    virtual void setReducedErrorPruning(bool v);

    /**
     * Get the value of numFolds.
     *
     * @return Value of numFolds.
     */
    virtual int getNumFolds() const;

    /**
     * Set the value of numFolds.
     *
     * @param v Value to assign to numFolds.
     */
    virtual void setNumFolds(int v);

    /**
     * Get the value of subtreeRaising.
     *
     * @return Value of subtreeRaising.
     */
    virtual bool getSubtreeRaising() const;

    /**
     * Set the value of subtreeRaising.
     *
     * @param v Value to assign to subtreeRaising.
     */
    virtual void setSubtreeRaising(bool v);

    /**
     * Check whether instance data is to be saved.
     *
     * @return true if instance data is saved
     */
    virtual bool getSaveInstanceData() const;

    /**
     * Set whether instance data is to be saved.
     *
     * @param v true if instance data is to be saved
     */
    virtual void setSaveInstanceData(bool v);

    /**
     * Gets the value of doNotMakeSplitPointActualValue.
     *
     * @return the value
     */
    virtual bool getDoNotMakeSplitPointActualValue() const;

    /**
     * Sets the value of doNotMakeSplitPointActualValue.
     *
     * @param m_doNotMakeSplitPointActualValue the value to set
     */
    virtual void setDoNotMakeSplitPointActualValue(bool mdoNotMakeSplitPointActualValue);

    /**
     * Builds the classifier to generate a partition.
     */
    virtual void generatePartition(Instances &data);

    /**
     * Computes an array that indicates node membership.
     */
    virtual double_array getMembershipValues(Instance &inst) const;

    /**
     * Returns the number of elements in the partition.
     */
    virtual int numElements() const;

    bool setParameters(const int, char **inParameters);
	friend std::ostream & operator << (std::ostream &os, const C48 &c48)
	{
		c48.mRoot->serialize(os);
		return os << '\0' << c48.mUnpruned << '\0' << c48.mCollapseTree << '\0' << c48.mCF << '\0' << c48.mMinNumObj << '\0'
			<< c48.mUseMDLcorrection << '\0' << c48.mUseLaplace << '\0' << c48.mReducedErrorPruning << '\0'
			<< c48.mNumFolds << '\0' << c48.mSubtreeRaising << '\0' << c48.mNoCleanup << '\0' << c48.mDoNotMakeSplitPointActualValue << '\0';
	}
};

#endif    //#ifndef _J48_
