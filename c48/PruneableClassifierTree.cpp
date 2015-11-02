#include "PruneableClassifierTree.h"
#include "NoSplit.h"
#include "core/Instances.h"
#include "ClassifierSplitModel.h"


PruneableClassifierTree::PruneableClassifierTree( ModelSelection *toSelectLocModel, bool pruneTree, int num, bool cleanup, int seed )
    : ClassifierTree( toSelectLocModel )
{
    pruneTheTree = pruneTree;
    numSets = num;
    mcleanup = cleanup;
    mseed = seed;
}


void PruneableClassifierTree::buildClassifier( Instances *data )
{

    // remove instances with missing class
    data = new Instances( data );
    data->deleteWithMissingClass();

    Random *random = new Random( mseed );
    data->stratify( numSets );
    buildTree( data->trainCV( numSets, numSets - 1, random ), data->testCV( numSets, numSets - 1 ), !mcleanup );
    if( pruneTheTree )
    {
        prune();
    }
    if( mcleanup )
    {
        cleanup( new Instances( data, 0 ) );
    }
}

void PruneableClassifierTree::prune()
{

    if( !misLeaf )
    {

        // Prune all subtrees.
        for( int i = 0; i < msons::length; i++ )
        {
            son( i )->prune();
        }

        // Decide if leaf is best choice.
        if( Utils::smOrEq( errorsForLeaf(),errorsForTree() ) )
        {

            // Free son Trees
            msons = nullptr;
            misLeaf = true;

            // Get NoSplit Model for node.
            mlocalModel = new NoSplit( localModel()->distribution() );
        }
    }
}

ClassifierTree *PruneableClassifierTree::getNewTree( Instances *train, Instances *test )
{

    PruneableClassifierTree *newTree = new PruneableClassifierTree( mtoSelectModel, pruneTheTree, numSets, mcleanup, mseed );
    newTree->buildTree( train, test, !mcleanup );
    return newTree;
}

double PruneableClassifierTree::errorsForTree()
{

    double errors = 0;

    if( misLeaf )
    {
        return errorsForLeaf();
    }
    else
    {
        for( int i = 0; i < msons::length; i++ )
        {
            if( Utils::eq( localModel()->distribution().perBag(i), 0 ) )
            {
                errors += mtest::perBag( i ) - mtest::perClassPerBag( i,localModel()->distribution().maxClass() );
            }
            else
            {
                errors += son( i )->errorsForTree();
            }
        }

        return errors;
    }
}

double PruneableClassifierTree::errorsForLeaf()
{

    return mtest::total() - mtest::perClass(localModel()->distribution().maxClass());
}

ClassifierSplitModel *PruneableClassifierTree::localModel()
{

    return static_cast<ClassifierSplitModel*>( mlocalModel );
}

PruneableClassifierTree *PruneableClassifierTree::son( int index )
{

    return static_cast<PruneableClassifierTree*>( msons[index] );
}
