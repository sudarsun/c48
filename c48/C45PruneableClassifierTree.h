#ifndef _C45PRUNEABLECLASSIFIERTREE_
#define _C45PRUNEABLECLASSIFIERTREE_

#include "ClassifierTree.h"
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

class ModelSelection;
class Instances;
class Distribution;

/// <summary>
/// Class for handling a tree structure that can
/// be pruned using C4.5 procedures.
/// </summary>

class C45PruneableClassifierTree : public ClassifierTree
{
	/// <summary>
	/// True if the tree is to be pruned. </summary>
protected:
	bool mpruneTheTree = false;

	/// <summary>
	/// True if the tree is to be collapsed. </summary>
	bool mcollapseTheTree = false;

	/// <summary>
	/// The confidence factor for pruning. </summary>
	float mCF = 0.25f;

	/// <summary>
	/// Is subtree raising to be performed? </summary>
	bool msubtreeRaising = true;

	/// <summary>
	/// Cleanup after the tree has been built. </summary>
	bool mcleanup = true;

	/// <summary>
	/// Constructor for pruneable tree structure. Stores reference
	/// to associated training data at each node.
	/// </summary>
	/// <param name="toSelectLocModel"> selection method for local splitting model </param>
	/// <param name="pruneTree"> true if the tree is to be pruned </param>
	/// <param name="cf"> the confidence factor for pruning </param>
	/// <param name="raiseTree"> </param>
	/// <param name="cleanup"> </param>
	/// <exception cref="Exception"> if something goes wrong </exception>
public:
	C45PruneableClassifierTree(ModelSelection *toSelectLocModel, bool pruneTree, float cf, bool raiseTree, bool cleanup, bool collapseTree);

	/// <summary>
	/// Method for building a pruneable classifier tree.
	/// </summary>
	/// <param name="data"> the data for building the tree </param>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual void buildClassifier(Instances *data);

	/// <summary>
	/// Collapses a tree to a node if training error doesn't increase.
	/// </summary>
	void collapse();

	/// <summary>
	/// Prunes a tree using C4.5's pruning procedure.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual void prune();

	/// <summary>
	/// Returns a newly created tree.
	/// </summary>
	/// <param name="data"> the data to work with </param>
	/// <returns> the new tree </returns>
	/// <exception cref="Exception"> if something goes wrong </exception>
protected:
	virtual ClassifierTree *getNewTree(Instances *data);

	/// <summary>
	/// Computes estimated errors for tree.
	/// </summary>
	/// <returns> the estimated errors </returns>
private:
	double getEstimatedErrors();

	/// <summary>
	/// Computes estimated errors for one branch.
	/// </summary>
	/// <param name="data"> the data to work with </param>
	/// <returns> the estimated errors </returns>
	/// <exception cref="Exception"> if something goes wrong </exception>
	double getEstimatedErrorsForBranch(Instances *data);

	/// <summary>
	/// Computes estimated errors for leaf.
	/// </summary>
	/// <param name="theDistribution"> the distribution to use </param>
	/// <returns> the estimated errors </returns>
	double getEstimatedErrorsForDistribution(Distribution *theDistribution);

	/// <summary>
	/// Computes errors of tree on training data.
	/// </summary>
	/// <returns> the training errors </returns>
	double getTrainingErrors();

	/// <summary>
	/// Method just exists to make program easier to read.
	/// </summary>
	/// <returns> the local split model </returns>
	ClassifierSplitModel *localModel();

	/// <summary>
	/// Computes new distributions of instances for nodes
	/// in tree.
	/// </summary>
	/// <param name="data"> the data to compute the distributions for </param>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void newDistribution(Instances *data);

	/// <summary>
	/// Method just exists to make program easier to read.
	/// </summary>
	C45PruneableClassifierTree *son(int index);

};


#endif	// _C45PRUNEABLECLASSIFIERTREE_
