#ifndef _C48_
#define _C48_

#include "AbstractClassifier.h"
#include "core/Instance.h"
#include "core/Instances.h"
#include <string>
#include <vector>
#include <stdexcept>

class ClassifierTree;
class ModelSelection;
class BinC45ModelSelection;
class C45ModelSelection;

class C48 : public AbstractClassifier
{

    /// <summary>
    /// The decision tree </summary>
protected:
    ClassifierTree *mroot;
    /// <summary>
    /// Unpruned tree? </summary>
    bool munpruned = false;
    /// <summary>
    /// Collapse tree? </summary>
    bool mcollapseTree = true;
    /// <summary>
    /// Confidence level </summary>
    float mCF = 0.25f;
    /// <summary>
    /// Minimum number of instances </summary>
    int mminNumObj = 2;
    /// <summary>
    /// Use MDL correction? </summary>
    bool museMDLcorrection = true;
    /// <summary>
    /// Determines whether probabilities are smoothed using Laplace correction when
    /// predictions are generated
    /// </summary>
    bool museLaplace = false;
    /// <summary>
    /// Use reduced error pruning? </summary>
    bool mreducedErrorPruning = false;
    /// <summary>
    /// Number of folds for reduced error pruning. </summary>
    int mnumFolds = 3;
    /// <summary>
    /// Binary splits on nominal attributes? </summary>
    bool mbinarySplits = false;
    /// <summary>
    /// Subtree raising to be performed? </summary>
    bool msubtreeRaising = true;
    /// <summary>
    /// Cleanup after the tree has been built. </summary>
    bool mnoCleanup = false;
    /// <summary>
    /// Random number seed for reduced-error pruning. </summary>
    int mSeed = 1;
    /// <summary>
    /// Do not relocate split point to actual data value </summary>
    bool mdoNotMakeSplitPointActualValue = false;

public:

    /// <summary>
    /// Generates the classifier.
    /// </summary>
    /// <param name="instances"> the data to train the classifier with </param>
    /// <exception cref="Exception"> if classifier can't be built successfully </exception>
    virtual void buildClassifier( Instances *instances );
    /// <summary>
    /// Classifies an instance.
    /// </summary>
    /// <param name="instance"> the instance to classify </param>
    /// <returns> the classification for the instance </returns>
    /// <exception cref="Exception"> if instance can't be classified successfully </exception>
    virtual double classifyInstance( Instance *instance );
    /// <summary>
    /// Returns class probabilities for an instance.
    /// </summary>
    /// <param name="instance"> the instance to calculate the class probabilities for </param>
    /// <returns> the class probabilities </returns>
    /// <exception cref="Exception"> if distribution can't be computed successfully </exception>
    std::vector<double> distributionForInstance( Instance *instance );
    /// <summary>
    /// Returns graph describing the tree.
    /// </summary>
    /// <returns> the graph describing the tree </returns>
    /// <exception cref="Exception"> if graph can't be computed </exception>
    virtual std::string graph();
    /// <summary>
    /// Returns tree in prefix order.
    /// </summary>
    /// <returns> the tree in prefix order </returns>
    /// <exception cref="Exception"> if something goes wrong </exception>
    virtual std::string prefix();

    /// <summary>
    /// Get the value of Seed.
    /// </summary>
    /// <returns> Value of Seed. </returns>
    virtual int getSeed();

    /// <summary>
    /// Set the value of Seed.
    /// </summary>
    /// <param name="newSeed"> Value to assign to Seed. </param>
    virtual void setSeed( int newSeed );

    /// <summary>
    /// Get the value of useLaplace.
    /// </summary>
    /// <returns> Value of useLaplace. </returns>
    virtual bool getUseLaplace();

    /// <summary>
    /// Set the value of useLaplace.
    /// </summary>
    /// <param name="newuseLaplace"> Value to assign to useLaplace. </param>
    virtual void setUseLaplace( bool newuseLaplace );

    /// <summary>
    /// Get the value of useMDLcorrection.
    /// </summary>
    /// <returns> Value of useMDLcorrection. </returns>
    virtual bool getUseMDLcorrection();

    /// <summary>
    /// Set the value of useMDLcorrection.
    /// </summary>
    /// <param name="newuseMDLcorrection"> Value to assign to useMDLcorrection. </param>
    virtual void setUseMDLcorrection( bool newuseMDLcorrection );

    /// <summary>
    /// Returns a description of the classifier.
    /// </summary>
    /// <returns> a description of the classifier </returns>
    virtual std::string toString();

    /// <summary>
    /// Returns a superconcise version of the model
    /// </summary>
    /// <returns> a summary of the model </returns>
    virtual std::string toSummaryString();

    /// <summary>
    /// Returns the size of the tree
    /// </summary>
    /// <returns> the size of the tree </returns>
    virtual double measureTreeSize();

    /// <summary>
    /// Returns the number of leaves
    /// </summary>
    /// <returns> the number of leaves </returns>
    virtual double measureNumLeaves();

    /// <summary>
    /// Returns the number of rules (same as number of leaves)
    /// </summary>
    /// <returns> the number of rules </returns>
    virtual double measureNumRules();

    /// <summary>
    /// Get the value of unpruned.
    /// </summary>
    /// <returns> Value of unpruned. </returns>
    virtual bool getUnpruned();

    /// <summary>
    /// Set the value of unpruned. Turns reduced-error pruning off if set.
    /// </summary>
    /// <param name="v"> Value to assign to unpruned. </param>
    virtual void setUnpruned( bool v );

    /// <summary>
    /// Get the value of collapseTree.
    /// </summary>
    /// <returns> Value of collapseTree. </returns>
    virtual bool getCollapseTree();

    /// <summary>
    /// Set the value of collapseTree.
    /// </summary>
    /// <param name="v"> Value to assign to collapseTree. </param>
    virtual void setCollapseTree( bool v );

    /// <summary>
    /// Get the value of CF.
    /// </summary>
    /// <returns> Value of CF. </returns>
    virtual float getConfidenceFactor();

    /// <summary>
    /// Set the value of CF.
    /// </summary>
    /// <param name="v"> Value to assign to CF. </param>
    virtual void setConfidenceFactor( float v );

    /// <summary>
    /// Get the value of minNumObj.
    /// </summary>
    /// <returns> Value of minNumObj. </returns>
    virtual int getMinNumObj();

    /// <summary>
    /// Set the value of minNumObj.
    /// </summary>
    /// <param name="v"> Value to assign to minNumObj. </param>
    virtual void setMinNumObj( int v );

    /// <summary>
    /// Get the value of reducedErrorPruning.
    /// </summary>
    /// <returns> Value of reducedErrorPruning. </returns>
    virtual bool getReducedErrorPruning();

    /// <summary>
    /// Set the value of reducedErrorPruning. Turns unpruned trees off if set.
    /// </summary>
    /// <param name="v"> Value to assign to reducedErrorPruning. </param>
    virtual void setReducedErrorPruning( bool v );

    /// <summary>
    /// Get the value of numFolds.
    /// </summary>
    /// <returns> Value of numFolds. </returns>
    virtual int getNumFolds();

    /// <summary>
    /// Set the value of numFolds.
    /// </summary>
    /// <param name="v"> Value to assign to numFolds. </param>
    virtual void setNumFolds( int v );

    /// <summary>
    /// Get the value of binarySplits.
    /// </summary>
    /// <returns> Value of binarySplits. </returns>
    virtual bool getBinarySplits();

    /// <summary>
    /// Set the value of binarySplits.
    /// </summary>
    /// <param name="v"> Value to assign to binarySplits. </param>
    virtual void setBinarySplits( bool v );

    /// <summary>
    /// Get the value of subtreeRaising.
    /// </summary>
    /// <returns> Value of subtreeRaising. </returns>
    virtual bool getSubtreeRaising();

    /// <summary>
    /// Set the value of subtreeRaising.
    /// </summary>
    /// <param name="v"> Value to assign to subtreeRaising. </param>
    virtual void setSubtreeRaising( bool v );

    /// <summary>
    /// Check whether instance data is to be saved.
    /// </summary>
    /// <returns> true if instance data is saved </returns>
    virtual bool getSaveInstanceData();

    /// <summary>
    /// Set whether instance data is to be saved.
    /// </summary>
    /// <param name="v"> true if instance data is to be saved </param>
    virtual void setSaveInstanceData( bool v );

    /// <summary>
    /// Gets the value of doNotMakeSplitPointActualValue.
    /// </summary>
    /// <returns> the value </returns>
    virtual bool getDoNotMakeSplitPointActualValue();

    /// <summary>
    /// Sets the value of doNotMakeSplitPointActualValue.
    /// </summary>
    /// <param name="mdoNotMakeSplitPointActualValue"> the value to set </param>
    virtual void setDoNotMakeSplitPointActualValue( bool mdoNotMakeSplitPointActualValue );

    /// <summary>
    /// Builds the classifier to generate a partition.
    /// </summary>
    virtual void generatePartition( Instances *data );

    /// <summary>
    /// Computes an array that indicates node membership.
    /// </summary>
    virtual std::vector<double> getMembershipValues( Instance *inst );

    /// <summary>
    /// Returns the number of elements in the partition.
    /// </summary>
    virtual int numElements();
};

#endif	//#ifndef _J48_
