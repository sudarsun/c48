#ifndef _CLASSIFIERTREE_
#define _CLASSIFIERTREE_

#include <string>
#include <vector>
#include <list>
#include <stdexcept>
#include "core/Typedefs.h"

// Forward class declarations:
class ModelSelection;
class ClassifierSplitModel;
class Instances;
class Distribution;
class Instance;


/**
 * Class for handling a tree structure used for classification.
 */
class ClassifierTree {

protected:
    /** The model selection method. */
    ModelSelection *mToSelectModel;

    /** Local model at node.*/
    ClassifierSplitModel *mLocalModel;

    /** References to sons. */
    std::vector<ClassifierTree*> mSons;

    /** True if node is leaf. */
    bool mIsLeaf = false;

    /** True if node is empty. */
    bool mIsEmpty = false;

    /** The training instances. */
    Instances *mTrain;

    /** The pruning instances. */
    Distribution *mTest;

    /** The id for the node. */
    int mID = 0;

    /**
     * Gets the next unique node ID.
     *
     * @return the next unique node ID.
     */
    static long long nextID();

    /**
     * Resets the unique node ID counter (e.g. between repeated separate print
     * types)
     */
    static void resetID();

    /**
     * Returns a newly created tree.
     *
     * @param data the training data
     * @return the generated tree
     * @throws Exception if something goes wrong
     */
    virtual ClassifierTree *getNewTree(Instances *data);

    /**
     * Returns a newly created tree.
     *
     * @param train the training data
     * @param test the pruning data.
     * @return the generated tree
     * @throws Exception if something goes wrong
     */
    virtual ClassifierTree *getNewTree(Instances *train, Instances *test);

public:

    /// <summary>
    /// Constructor.
    /// </summary>

    ClassifierTree(ModelSelection *toSelectLocModel);

    /**
     * Method for building a classifier tree.
     *
     * @param data the data to build the tree from
     * @throws Exception if something goes wrong
     */
    virtual void buildClassifier(Instances *data);

    /**
     * Builds the tree structure.
     *
     * @param data the data for which the tree structure is to be generated.
     * @param keepData is training data to be kept?
     * @throws Exception if something goes wrong
     */
    virtual void buildTree(Instances *data, bool keepData);

    /**
     * Builds the tree structure with hold out set
     *
     * @param train the data for which the tree structure is to be generated.
     * @param test the test data for potential pruning
     * @param keepData is training Data to be kept?
     * @throws Exception if something goes wrong
     */
    virtual void buildTree(Instances *train, Instances *test, bool keepData);

    /**
     * Classifies an instance.
     *
     * @param instance the instance to classify
     * @return the classification
     * @throws Exception if something goes wrong
     */
    virtual double classifyInstance(Instance *instance);

    /**
     * Cleanup in order to save memory.
     *
     * @param justHeaderInfo
     */
    void cleanup(Instances *justHeaderInfo);

    /**
     * Returns class probabilities for a weighted instance.
     *
     * @param instance the instance to get the distribution for
     * @param useLaplace whether to use laplace or not
     * @return the distribution
     * @throws Exception if something goes wrong
     */
    double_array distributionForInstance(Instance *instance, bool useLaplace);

    /**
     * Assigns a uniqe id to every node in the tree.
     *
     * @param lastID the last ID that was assign
     * @return the new current ID
     */
    virtual int assignIDs(int lastID);

    /**
     * Returns number of leaves in tree structure.
     *
     * @return the number of leaves
     */
    virtual int numLeaves();

    /**
     * Returns number of nodes in tree structure.
     *
     * @return the number of nodes
     */
    virtual int numNodes();

    /**
     * Prints tree structure.
     *
     * @param isDumpTree Value to display the tree structure.
     * @return the tree structure
     */
    virtual string toString(bool isDumpTree = false);
    /**
     * Computes a list that indicates node membership
     */
    virtual double_array getMembershipValues(Instance *instance);

private:
    /**
     * For getting a unique ID when outputting the tree (hashcode isn't guaranteed
     * unique)
     */
    static long long PRINTED_NODES;

    /**
     * Help method for printing tree structure.
     *
     * @param depth the current depth
     * @param text for outputting the structure
     * @throws Exception if something goes wrong
     */
    void dumpTree(int depth, string &text);

    /**
     * Help method for computing class probabilities of a given instance.
     *
     * @param classIndex the class index
     * @param instance the instance to compute the probabilities for
     * @param weight the weight to use
     * @return the laplace probs
     * @throws Exception if something goes wrong
     */
    double getProbsLaplace(int classIndex, Instance *instance, double weight);

    /**
     * Help method for computing class probabilities of a given instance.
     *
     * @param classIndex the class index
     * @param instance the instance to compute the probabilities for
     * @param weight the weight to use
     * @return the probs
     * @throws Exception if something goes wrong
     */
    double getProbs(int classIndex, Instance *instance, double weight);

    /**
     * Method to the current model.
     */
    ClassifierSplitModel *localModel();

    /**
     * Method to the current son of the index in classifier tree
     */
    ClassifierTree *son(int index);

};

#endif    //#ifndef _CLASSIFIERTREE_
