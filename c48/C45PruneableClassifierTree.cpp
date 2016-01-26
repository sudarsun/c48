#include "C45PruneableClassifierTree.h"
#include "ModelSelection.h"
#include "core/Instances.h"
#include "Distribution.h"
#include "core/Utils.h"
#include "NoSplit.h"
#include "Stats.h"
    
C45PruneableClassifierTree::C45PruneableClassifierTree(ModelSelection *toSelectLocModel, bool pruneTree, float cf, bool raiseTree, bool cleanup, bool collapseTree) : ClassifierTree(toSelectLocModel)
{
    mPruneTheTree = pruneTree;
    mCF = cf;
    mSubtreeRaising = raiseTree;
    mCleanup = cleanup;
    mCollapseTheTree = collapseTree;
}


void C45PruneableClassifierTree::buildClassifier(Instances *data)
{

    // remove instances with missing class
    Instances dataMissed(data);
    dataMissed.deleteWithMissingClass();

    buildTree(&dataMissed, mSubtreeRaising || !mCleanup);
    if (mCollapseTheTree)
    {
        collapse();
    }
    if (mPruneTheTree)
    {
        prune();
    }
    if (mCleanup)
    {
        ;// cleanup(new Instances(dataMissed, 0));
    }
}

void C45PruneableClassifierTree::collapse()
{

    double errorsOfSubtree;
    double errorsOfTree;
    int i;

    if (!mIsLeaf)
    {
        errorsOfSubtree = getTrainingErrors();
        errorsOfTree = localModel()->getDistribution()->numIncorrect();
        if (errorsOfSubtree >= errorsOfTree - 1E-3)
        {

            // Free adjacent trees
            //mSons = ;
            mIsLeaf = true;

            // Get NoSplit Model for tree.
            mLocalModel = new NoSplit(localModel()->getDistribution());
        }
        else
        {
            for (i = 0; i < (int)mSons.size(); i++)
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

    if (!mIsLeaf)
    {

        // Prune all subtrees.
        for (i = 0; i < (int)mSons.size(); i++)
        {
            son(i)->prune();
        }

        // Compute error for largest branch
        indexOfLargestBranch = localModel()->getDistribution()->maxBag();
        if (mSubtreeRaising)
        {
            errorsLargestBranch = son(indexOfLargestBranch)->getEstimatedErrorsForBranch(static_cast<Instances*>(mTrain));
        }
        else
        {
            errorsLargestBranch = std::numeric_limits<double>::max();
        }

        // Compute error if this Tree would be leaf
        errorsLeaf = getEstimatedErrorsForDistribution(localModel()->getDistribution());

        // Compute error for the whole subtree
        errorsTree = getEstimatedErrors();

        // Decide if leaf is best choice.
        if (Utils::smOrEq(errorsLeaf, errorsTree + 0.1) && Utils::smOrEq(errorsLeaf, errorsLargestBranch + 0.1))
        {

            // Free son Trees
            mSons.clear();
            mIsLeaf = true;

            // Get NoSplit Model for node.
            mLocalModel = new NoSplit(localModel()->getDistribution());
            return;
        }

        // Decide if largest branch is better choice
        // than whole subtree.
        if (Utils::smOrEq(errorsLargestBranch, errorsTree + 0.1))
        {
            largestBranch = son(indexOfLargestBranch);
            mSons = largestBranch->mSons;
            mLocalModel = largestBranch->localModel();
            mIsLeaf = largestBranch->mIsLeaf;
            newDistribution(mTrain);
            prune();
        }
    }
}

ClassifierTree *C45PruneableClassifierTree::getNewTree(Instances *data) const
{

    C45PruneableClassifierTree *newTree = new C45PruneableClassifierTree(mToSelectModel, mPruneTheTree, mCF, mSubtreeRaising, mCleanup, mCollapseTheTree);
    newTree->buildTree(static_cast<Instances*>(data), mSubtreeRaising || !mCleanup);

    return newTree;
}

double C45PruneableClassifierTree::getEstimatedErrors() const
{

    double errors = 0;
    int i;

    if (mIsLeaf)
    {
        return getEstimatedErrorsForDistribution(localModel()->getDistribution());
    }
    else
    {
        for (i = 0; i < (int)mSons.size(); i++)
        {
            errors = errors + son(i)->getEstimatedErrors();
        }
        return errors;
    }
}

double C45PruneableClassifierTree::getEstimatedErrorsForBranch(Instances *data) const
{

    std::vector<Instances*> localInstances;
    double errors = 0;
    int i;

    if (mIsLeaf)
    {
        return getEstimatedErrorsForDistribution(new Distribution(data));
    }
    else
    {
        Distribution *savedDist = localModel()->getDistribution();
        localModel()->resetDistribution(data);

        localInstances = static_cast<std::vector<Instances*>>(localModel()->split(data));
        localModel()->setDistribution(savedDist);
        for (i = 0; i < (int)mSons.size(); i++)
        {
            errors = errors + son(i)->getEstimatedErrorsForBranch(localInstances[i]);
        }
        return errors;
    }
}

double C45PruneableClassifierTree::getEstimatedErrorsForDistribution(Distribution *theDistribution) const
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

double C45PruneableClassifierTree::getTrainingErrors() const
{

    double errors = 0;
    int i;

    if (mIsLeaf)
    {
        return localModel()->getDistribution()->numIncorrect();
    }
    else
    {
        for (i = 0; i < (int)mSons.size(); i++)
        {
            errors = errors + son(i)->getTrainingErrors();
        }
        return errors;
    }
}

ClassifierSplitModel *C45PruneableClassifierTree::localModel() const
{

    return static_cast<ClassifierSplitModel*>(mLocalModel);
}

void C45PruneableClassifierTree::newDistribution(Instances *data)
{

    std::vector<Instances*> localInstances;

    localModel()->resetDistribution(data);
    mTrain = data;
    if (!mIsLeaf)
    {
        localInstances = static_cast<std::vector<Instances*>>(localModel()->split(data));
        for (int i = 0; i < (int)mSons.size(); i++)
        {
            son(i)->newDistribution(localInstances[i]);
        }
    }
    else
    {

        // Check whether there are some instances at the leaf now!
        if (!Utils::eq(data->sumOfWeights(), 0))
        {
            mIsEmpty = false;
        }
    }
}

C45PruneableClassifierTree *C45PruneableClassifierTree::son(int index) const
{

    return static_cast<C45PruneableClassifierTree*>(mSons[index]);
}
