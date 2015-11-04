#include "C45PruneableClassifierTree.h"
#include "ModelSelection.h"
#include "core/Instances.h"
#include "Distribution.h"
#include "core/Utils.h"
#include "NoSplit.h"
#include "Stats.h"

C45PruneableClassifierTree::C45PruneableClassifierTree(ModelSelection *toSelectLocModel, bool pruneTree, float cf, bool raiseTree, bool cleanup, bool collapseTree) : ClassifierTree(toSelectLocModel)
{
	mpruneTheTree = pruneTree;
	mCF = cf;
	msubtreeRaising = raiseTree;
	mcleanup = cleanup;
	mcollapseTheTree = collapseTree;
}


void C45PruneableClassifierTree::buildClassifier(Instances *data)
{

	// remove instances with missing class
	data = new Instances(data);
	data->deleteWithMissingClass();

	buildTree(data, msubtreeRaising || !mcleanup);
	if (mcollapseTheTree)
	{
		collapse();
	}
	if (mpruneTheTree)
	{
		prune();
	}
	if (mcleanup)
	{
		cleanup(new Instances(data, 0));
	}
}

void C45PruneableClassifierTree::collapse()
{

	double errorsOfSubtree;
	double errorsOfTree;
	int i;

	if (!misLeaf)
	{
		errorsOfSubtree = getTrainingErrors();
		errorsOfTree = localModel()->distribution()->numIncorrect();
		if (errorsOfSubtree >= errorsOfTree - 1E-3)
		{

			// Free adjacent trees
			//msons = ;
			misLeaf = true;

			// Get NoSplit Model for tree.
			mlocalModel = new NoSplit(localModel()->distribution());
		}
		else
		{
			for (i = 0; i < (int)msons.size(); i++)
			{
				son(i)->collapse();
			}
		}
	}
}

void C45PruneableClassifierTree::prune()
{

	double errorsLargestBranch;
	double errorsLeaf;
	double errorsTree;
	int indexOfLargestBranch;
	C45PruneableClassifierTree *largestBranch;
	int i;

	if (!misLeaf)
	{

		// Prune all subtrees.
		for (i = 0; i < (int)msons.size(); i++)
		{
			son(i)->prune();
		}

		// Compute error for largest branch
		indexOfLargestBranch = localModel()->distribution()->maxBag();
		if (msubtreeRaising)
		{
			errorsLargestBranch = son(indexOfLargestBranch)->getEstimatedErrorsForBranch(static_cast<Instances*>(mtrain));
		}
		else
		{
			errorsLargestBranch = std::numeric_limits<double>::max();
		}

		// Compute error if this Tree would be leaf
		errorsLeaf = getEstimatedErrorsForDistribution(localModel()->distribution());

		// Compute error for the whole subtree
		errorsTree = getEstimatedErrors();

		// Decide if leaf is best choice.
		if (Utils::smOrEq(errorsLeaf, errorsTree + 0.1) && Utils::smOrEq(errorsLeaf, errorsLargestBranch + 0.1))
		{

			// Free son Trees
			msons.clear();
			misLeaf = true;

			// Get NoSplit Model for node.
			mlocalModel = new NoSplit(localModel()->distribution());
			return;
		}

		// Decide if largest branch is better choice
		// than whole subtree.
		if (Utils::smOrEq(errorsLargestBranch, errorsTree + 0.1))
		{
			largestBranch = son(indexOfLargestBranch);
			msons = largestBranch->msons;
			mlocalModel = largestBranch->localModel();
			misLeaf = largestBranch->misLeaf;
			newDistribution(mtrain);
			prune();
		}
	}
}

ClassifierTree *C45PruneableClassifierTree::getNewTree(Instances *data)
{

	C45PruneableClassifierTree *newTree = new C45PruneableClassifierTree(mtoSelectModel, mpruneTheTree, mCF, msubtreeRaising, mcleanup, mcollapseTheTree);
	newTree->buildTree(static_cast<Instances*>(data), msubtreeRaising || !mcleanup);

	return newTree;
}

double C45PruneableClassifierTree::getEstimatedErrors()
{

	double errors = 0;
	int i;

	if (misLeaf)
	{
		return getEstimatedErrorsForDistribution(localModel()->distribution());
	}
	else
	{
		for (i = 0; i < (int)msons.size(); i++)
		{
			errors = errors + son(i)->getEstimatedErrors();
		}
		return errors;
	}
}

double C45PruneableClassifierTree::getEstimatedErrorsForBranch(Instances *data)
{

	std::vector<Instances*> localInstances;
	double errors = 0;
	int i;

	if (misLeaf)
	{
		return getEstimatedErrorsForDistribution(new Distribution(data));
	}
	else
	{
		Distribution *savedDist = localModel()->getDistribution();
		localModel()->resetDistribution(data);
		//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
		localInstances = static_cast<std::vector<Instances*>>(localModel()->split(data));
		localModel()->setDistribution(savedDist);
		for (i = 0; i < (int)msons.size(); i++)
		{
			errors = errors + son(i)->getEstimatedErrorsForBranch(localInstances[i]);
		}
		return errors;
	}
}

double C45PruneableClassifierTree::getEstimatedErrorsForDistribution(Distribution *theDistribution)
{

	if (Utils::eq(theDistribution->total(), 0))
	{
		return 0;
	}
	else
	{
		return theDistribution->numIncorrect() + Stats::addErrs(theDistribution->total(), theDistribution->numIncorrect(), mCF);
	}
}

double C45PruneableClassifierTree::getTrainingErrors()
{

	double errors = 0;
	int i;

	if (misLeaf)
	{
		return localModel()->distribution()->numIncorrect();
	}
	else
	{
		for (i = 0; i < (int)msons.size(); i++)
		{
			errors = errors + son(i)->getTrainingErrors();
		}
		return errors;
	}
}

ClassifierSplitModel *C45PruneableClassifierTree::localModel()
{

	return static_cast<ClassifierSplitModel*>(mlocalModel);
}

void C45PruneableClassifierTree::newDistribution(Instances *data)
{

	std::vector<Instances*> localInstances;

	localModel()->resetDistribution(data);
	mtrain = data;
	if (!misLeaf)
	{
		localInstances = static_cast<std::vector<Instances*>>(localModel()->split(data));
		for (int i = 0; i < (int)msons.size(); i++)
		{
			son(i)->newDistribution(localInstances[i]);
		}
	}
	else
	{

		// Check whether there are some instances at the leaf now!
		if (!Utils::eq(data->sumOfWeights(), 0))
		{
			misEmpty = false;
		}
	}
}

C45PruneableClassifierTree *C45PruneableClassifierTree::son(int index)
{

	return static_cast<C45PruneableClassifierTree*>(msons[index]);
}