#ifndef _C45PRUNEABLECLASSIFIERTREE_
#define _C45PRUNEABLECLASSIFIERTREE_

#include "ClassifierTree.h"
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

// Forward class declarations:
class ModelSelection;
class Instances;
class Distribution;

/**
 * Class for handling a tree structure that can
 * be pruned using C4.5 procedures.
 */
class C45PruneableClassifierTree : public ClassifierTree
{
protected:
    /** True if the tree is to be pruned. */
    bool mPruneTheTree;

    /** True if the tree is to be collapsed. */
    bool mCollapseTheTree;

    /** The confidence factor for pruning. */
    float mCF = 0.25f;

    /** Is subtree raising to be performed? */
    bool mSubtreeRaising = true;

    /** Cleanup after the tree has been built. */
    bool mCleanup = true;

    /**
       * Returns a newly created tree.
       *
       * @param data the data to work with
       * @return the new tree
       * @throws Exception if something goes wrong
       */
    virtual ClassifierTree *getNewTree(Instances *data) const override;

public:
    /**
     * Constructor for pruneable tree structure. Stores reference
     * to associated training data at each node.
     *
     * @param toSelectLocModel selection method for local splitting model
     * @param pruneTree true if the tree is to be pruned
     * @param cf the confidence factor for pruning
     * @param raiseTree
     * @param cleanup
     * @throws Exception if something goes wrong
     */
    C45PruneableClassifierTree(ModelSelection *toSelectLocModel, bool pruneTree, float cf, bool raiseTree, bool cleanup, bool collapseTree);

    /**
     * Method for building a pruneable classifier tree.
     *
     * @param data the data for building the tree
     * @throws Exception if something goes wrong
     */
    virtual void buildClassifier(Instances *data) override;

    /**
     * Collapses a tree to a node if training error doesn't increase.
     */
    void collapse();

    /**
     * Prunes a tree using C4.5's pruning procedure.
     *
     * @throws Exception if something goes wrong
     */
    virtual void prune();

private:
    /**
     * Computes estimated errors for tree.
     *
     * @return the estimated errors
     */
    double getEstimatedErrors() const;

    /**
     * Computes estimated errors for one branch.
     *
     * @param data the data to work with
     * @return the estimated errors
     * @throws Exception if something goes wrong
     */
    double getEstimatedErrorsForBranch(Instances *data) const;

    /**
     * Computes estimated errors for leaf.
     *
     * @param theDistribution the distribution to use
     * @return the estimated errors
     */
    double getEstimatedErrorsForDistribution(Distribution *theDistribution) const;

    /**
     * Computes errors of tree on training data.
     *
     * @return the training errors
     */
    double getTrainingErrors() const;

    /**
     * Method just exists to make program easier to read.
     *
     * @return the local split model
     */
    ClassifierSplitModel *localModel() const;

    /**
     * Computes new distributions of instances for nodes
     * in tree.
     *
     * @param data the data to compute the distributions for
     * @throws Exception if something goes wrong
     */
    void newDistribution(Instances *data);

    /**
     * Method just exists to make program easier to read.
     */
    C45PruneableClassifierTree *son(int index) const;

};

#endif    // _C45PRUNEABLECLASSIFIERTREE_
