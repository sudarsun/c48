#include "C48.h"
#include "ModelSelection.h"
//#include "BinC45ModelSelection.h"
#include "C45PruneableClassifierTree.h"
#include "PruneableClassifierTree.h"
#include "C45ModelSelection.h"

void C48::buildClassifier(Instances *instances)
{

	ModelSelection *modSelection = nullptr;

	if (mbinarySplits)
	{
		;//modSelection = new BinC45ModelSelection( mminNumObj, instances, museMDLcorrection, mdoNotMakeSplitPointActualValue );
	}
	else
	{
		modSelection = new C45ModelSelection(mminNumObj, instances, museMDLcorrection, mdoNotMakeSplitPointActualValue);
	}

	if (!mreducedErrorPruning)
	{
		mroot = new C45PruneableClassifierTree(modSelection, !munpruned, mCF, msubtreeRaising, !mnoCleanup, mcollapseTree);
	}
	else
	{
		;//mroot = new PruneableClassifierTree( modSelection, !munpruned, mnumFolds, !mnoCleanup, mSeed );
	}
	mroot->buildClassifier(instances);
	if (mbinarySplits)
	{
		;//( static_cast<BinC45ModelSelection*>( modSelection ) )->cleanup();
	}
	else
	{
		;// (static_cast<C45ModelSelection*>(modSelection))->cleanup();
	}
}

double C48::classifyInstance(Instance *instance)
{
	return mroot->classifyInstance(instance);
}

std::vector<double> C48::distributionForInstance(Instance *instance)
{
	return mroot->distributionForInstance(instance, museLaplace);
}

std::string C48::graph()
{
	return mroot->graph();
}

std::string C48::prefix()
{
	return mroot->prefix();
}

int C48::getSeed()
{
	return mSeed;
}

void C48::setSeed(int newSeed)
{
	mSeed = newSeed;
}

bool C48::getUseLaplace()
{
	return museLaplace;
}

void C48::setUseLaplace(bool newuseLaplace)
{
	museLaplace = newuseLaplace;
}

bool C48::getUseMDLcorrection()
{
	return museMDLcorrection;
}

void C48::setUseMDLcorrection(bool newuseMDLcorrection)
{
	museMDLcorrection = newuseMDLcorrection;
}

std::string C48::toString()
{
	if (mroot == nullptr)
	{
		return "No classifier built";
	}
	if (munpruned)
	{
		return std::string("C48 unpruned tree\n------------------\n") + mroot->toString();
	}
	else
	{
		return std::string("C48 pruned tree\n------------------\n") + mroot->toString();
	}
}

std::string C48::toSummaryString()
{
	return std::string("Number of leaves: ") + std::to_string(mroot->numLeaves()) + std::string("\n") + std::string("Size of the tree: ") + std::to_string(mroot->numNodes()) + std::string("\n");
}

double C48::measureTreeSize()
{
	return mroot->numNodes();
}

double C48::measureNumLeaves()
{
	return mroot->numLeaves();
}

double C48::measureNumRules()
{
	return mroot->numLeaves();
}

bool C48::getUnpruned()
{
	return munpruned;
}

void C48::setUnpruned(bool v)
{
	if (v)
	{
		mreducedErrorPruning = false;
	}
	munpruned = v;
}

bool C48::getCollapseTree()
{
	return mcollapseTree;
}

void C48::setCollapseTree(bool v)
{
	mcollapseTree = v;
}

float C48::getConfidenceFactor()
{
	return mCF;
}

void C48::setConfidenceFactor(float v)
{
	mCF = v;
}

int C48::getMinNumObj()
{
	return mminNumObj;
}

void C48::setMinNumObj(int v)
{
	mminNumObj = v;
}

bool C48::getReducedErrorPruning()
{
	return mreducedErrorPruning;
}

void C48::setReducedErrorPruning(bool v)
{
	if (v)
	{
		munpruned = false;
	}
	mreducedErrorPruning = v;
}

int C48::getNumFolds()
{
	return mnumFolds;
}

void C48::setNumFolds(int v)
{
	mnumFolds = v;
}

bool C48::getBinarySplits()
{
	return mbinarySplits;
}

void C48::setBinarySplits(bool v)
{
	mbinarySplits = v;
}

bool C48::getSubtreeRaising()
{
	return msubtreeRaising;
}

void C48::setSubtreeRaising(bool v)
{
	msubtreeRaising = v;
}

bool C48::getSaveInstanceData()
{
	return mnoCleanup;
}

void C48::setSaveInstanceData(bool v)
{
	mnoCleanup = v;
}

bool C48::getDoNotMakeSplitPointActualValue()
{
	return mdoNotMakeSplitPointActualValue;
}

void C48::setDoNotMakeSplitPointActualValue(bool mdoNotMakeSplitPointActualValue)
{
	this->mdoNotMakeSplitPointActualValue = mdoNotMakeSplitPointActualValue;
}

void C48::generatePartition(Instances *data)
{
	buildClassifier(data);
}

std::vector<double> C48::getMembershipValues(Instance *inst)
{
	return mroot->getMembershipValues(inst);
}

int C48::numElements()
{
	return mroot->numNodes();
}
