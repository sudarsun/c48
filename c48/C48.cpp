#include "C48.h"
#include "ModelSelection.h"
#include "C45PruneableClassifierTree.h"
#include "C45ModelSelection.h"

C48::C48()
{
    mRoot = nullptr;
    mUnpruned = false;
    mCollapseTree = true;
    mCF = 0.25f;
    mMinNumObj = 2;
    mUseMDLcorrection = true;
    mUseLaplace = false;
    mReducedErrorPruning = false;
    mNumFolds = 3;
    mSubtreeRaising = true;
    mNoCleanup= false;
    mDoNotMakeSplitPointActualValue= false;
}

C48::~C48()
{
    if(mRoot != nullptr)
        delete mRoot;
}

void C48::buildClassifier(Instances *instances)
{

    ModelSelection *modSelection = nullptr;
    
    modSelection = new C45ModelSelection(mMinNumObj, instances, mUseMDLcorrection, mDoNotMakeSplitPointActualValue);

    if (!mReducedErrorPruning)
    {
        mRoot = new C45PruneableClassifierTree(modSelection, !mUnpruned, mCF, mSubtreeRaising, !mNoCleanup, mCollapseTree);
    }

    mRoot->buildClassifier(instances);
}

double C48::classifyInstance(Instance *instance) const
{
    return mRoot->classifyInstance(instance);
}

double_array C48::distributionForInstance(Instance *instance) const
{
    return mRoot->distributionForInstance(instance, mUseLaplace);
}

bool C48::getUseLaplace() const
{
    return mUseLaplace;
}

void C48::setUseLaplace(bool newuseLaplace)
{
    mUseLaplace = newuseLaplace;
}

bool C48::getUseMDLcorrection() const
{
    return mUseMDLcorrection;
}

void C48::setUseMDLcorrection(bool newuseMDLcorrection)
{
    mUseMDLcorrection = newuseMDLcorrection;
}

string C48::toString(bool isDumpTree) const
{
    if (mRoot == nullptr)
    {
        return "No classifier built";
    }
    if (mUnpruned)
    {
        return string("C48 unpruned tree\n------------------\n") + mRoot->toString(isDumpTree);
    }
    else
    {
        return string("C48 pruned tree\n------------------\n") + mRoot->toString(isDumpTree);
    }
}

string C48::toSummaryString() const
{
    return string("Number of leaves: ") + std::to_string(mRoot->numLeaves()) + string("\n") + string("Size of the tree: ") + std::to_string(mRoot->numNodes()) + string("\n");
}

double C48::measureTreeSize() const
{
    return mRoot->numNodes();
}

double C48::measureNumLeaves() const
{
    return mRoot->numLeaves();
}

double C48::measureNumRules() const
{
    return mRoot->numLeaves();
}

bool C48::getUnpruned() const
{
    return mUnpruned;
}

void C48::setUnpruned(bool v)
{
    if (v)
    {
        mReducedErrorPruning = false;
    }
    mUnpruned = v;
}

bool C48::getCollapseTree() const
{
    return mCollapseTree;
}

void C48::setCollapseTree(bool v)
{
    mCollapseTree = v;
}

float C48::getConfidenceFactor() const
{
    return mCF;
}

void C48::setConfidenceFactor(float v)
{
    mCF = v;
}

int C48::getMinNumObj() const
{
    return mMinNumObj;
}

void C48::setMinNumObj(int v)
{
    mMinNumObj = v;
}

bool C48::getReducedErrorPruning() const
{
    return mReducedErrorPruning;
}

void C48::setReducedErrorPruning(bool v)
{
    if (v)
    {
        mUnpruned = false;
    }
    mReducedErrorPruning = v;
}

int C48::getNumFolds() const
{
    return mNumFolds;
}

void C48::setNumFolds(int v)
{
    mNumFolds = v;
}

bool C48::getSubtreeRaising() const
{
    return mSubtreeRaising;
}

void C48::setSubtreeRaising(bool v)
{
    mSubtreeRaising = v;
}

bool C48::getSaveInstanceData() const
{
    return mNoCleanup;
}

void C48::setSaveInstanceData(bool v)
{
    mNoCleanup = v;
}

bool C48::getDoNotMakeSplitPointActualValue() const
{
    return mDoNotMakeSplitPointActualValue;
}

void C48::setDoNotMakeSplitPointActualValue(bool mDoNotMakeSplitPointActualValue)
{
    this->mDoNotMakeSplitPointActualValue = mDoNotMakeSplitPointActualValue;
}

void C48::generatePartition(Instances *data)
{
    buildClassifier(data);
}

double_array C48::getMembershipValues(Instance *inst) const
{
    return mRoot->getMembershipValues(inst);
}

int C48::numElements() const
{
    return mRoot->numNodes();
}
