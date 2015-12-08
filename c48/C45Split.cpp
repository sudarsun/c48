#include "C45Split.h"
#include "core/Instances.h"
#include "InfoGainSplitCrit.h"
#include "GainRatioSplitCrit.h"
#include "core/Instance.h"
#include "Distribution.h"
#include "core/Utils.h"


InfoGainSplitCrit *C45Split::infoGainCrit = new InfoGainSplitCrit();
GainRatioSplitCrit *C45Split::gainRatioCrit = new GainRatioSplitCrit();

C45Split::C45Split(int attIndex, int minNoObj, double sumOfWeights, bool useMDLcorrection) :
	mAttIndex(attIndex), mMinNoObj(minNoObj), mUseMDLcorrection(useMDLcorrection), mSumOfWeights(sumOfWeights)
{

	// Get index of attribute to split on.

	// Set minimum number of objects.

	// Set the sum of the weights

	// Whether to use the MDL correction for numeric attributes
}

void C45Split::buildClassifier(Instances trainInstances)
{

	// Initialize the remaining instance variables.
	mNumSubsets = 0;
	mSplitPoint = std::numeric_limits<double>::max();
	mInfoGain = 0;
	mGainRatio = 0;

	// Different treatment for enumerated and numeric
	// attributes.
	if (trainInstances.attribute(mAttIndex)->isNominal())
	{
		mComplexityIndex = trainInstances.attribute(mAttIndex)->numValues();
		mIndex = mComplexityIndex;
		handleEnumeratedAttribute(&trainInstances);
	}
	else
	{
		mComplexityIndex = 2;
		mIndex = 0;
		trainInstances.Sort(trainInstances.attribute(mAttIndex));
		handleNumericAttribute(&trainInstances);
	}
}

int C45Split::attIndex()
{

	return mAttIndex;
}

double C45Split::splitPoint()
{
	return mSplitPoint;
}

double C45Split::classProb(int classIndex, Instance *instance, int theSubset)
{

	if (theSubset <= -1)
	{
		std::vector<double> _weights = weights(instance);
		if (_weights.empty())
		{
			return mDistribution->prob(classIndex);
		}
		else
		{
			double prob = 0;
			for (int i = 0; i < _weights.size(); i++)
			{
				prob += _weights[i] * mDistribution->prob(classIndex, i);
			}
			return prob;
		}
	}
	else
	{
		if (Utils::gr(mDistribution->perBag(theSubset), 0))
		{
			return mDistribution->prob(classIndex, theSubset);
		}
		else
		{
			return mDistribution->prob(classIndex);
		}
	}
}

double C45Split::codingCost()
{

	return Utils::getLog2(mIndex);
}

double C45Split::gainRatio()
{
	return mGainRatio;
}

void C45Split::handleEnumeratedAttribute(Instances *trainInstances)
{

	Instance *instance;

	mDistribution = new Distribution(mComplexityIndex, trainInstances->numClasses());

	// Only Instances with known values are relevant.
	int totalInst = trainInstances->numInstances();
	for (int i = 0; i < totalInst; i++)
	{
		instance = trainInstances->instance(i);
		if (!instance->isMissing(mAttIndex))
		{
			mDistribution->add(static_cast<int>(instance->value(mAttIndex)), instance);
		}
	}

	// Check if minimum number of Instances in at least two
	// subsets.
	if (mDistribution->check(mMinNoObj))
	{
		mNumSubsets = mComplexityIndex;
		mInfoGain = infoGainCrit->splitCritValue(mDistribution, mSumOfWeights);
		mGainRatio = gainRatioCrit->splitCritValue(mDistribution, mSumOfWeights, mInfoGain);
	}
}

void C45Split::handleNumericAttribute(Instances *trainInstances)
{

	int firstMiss;
	int next = 1;
	int last = 0;
	int splitIndex = -1;
	double currentInfoGain;
	double defaultEnt;
	double minSplit;
	Instance *instance;
	int i;

	// Current attribute is a numeric attribute.
	mDistribution = new Distribution(2, trainInstances->numClasses());

	// Only Instances with known values are relevant.
	int totalInst = trainInstances->numInstances();
	i = 0;
	for (int j = 0; j < totalInst; j++)
	{
		instance = trainInstances->instance(j);
		if (instance->isMissing(mAttIndex))
		{
			break;
		}
		mDistribution->add(1, instance);
		i++;
	}
	firstMiss = i;

	// Compute minimum number of Instances required in each
	// subset.
	minSplit = 0.1 * (mDistribution->total()) / (trainInstances->numClasses());
	if (Utils::smOrEq(minSplit, mMinNoObj))
	{
		minSplit = mMinNoObj;
	}
	else
	{
		if (Utils::gr(minSplit, 25))	
			minSplit = 25;
	}

	// Enough Instances with known values?
	if (Utils::sm(firstMiss, 2 * minSplit))
	{
		return;
	}

	// Compute values of criteria for all possible split
	// indices.
	defaultEnt = infoGainCrit->oldEnt(mDistribution);
	while (next < firstMiss)
	{
		if ( trainInstances->instance(next - 1)->value(mAttIndex)+ 1e-5 < trainInstances->instance(next)->value(mAttIndex))
		{

			// Move class values for all Instances up to next
			// possible split point.
			mDistribution->shiftRange(1, 0, trainInstances, last, next);

			// Check if enough Instances in each subset and compute
			// values for criteria.
			if (Utils::grOrEq(mDistribution->perBag(0), minSplit) && Utils::grOrEq(mDistribution->perBag(1), minSplit))
			{
				currentInfoGain = infoGainCrit->splitCritValue(mDistribution, mSumOfWeights, defaultEnt);
				if (Utils::gr(currentInfoGain, mInfoGain))
				{
					mInfoGain = currentInfoGain;
					splitIndex = next - 1;
				}
				mIndex++;
			}
			last = next;
		}
		next++;
	}

	// Was there any useful split?
	if (mIndex == 0)
	{
		return;
	}

	// Compute modified information gain for best split.
	if (mUseMDLcorrection)
	{
		mInfoGain = mInfoGain - (Utils::getLog2(mIndex) / mSumOfWeights);
	}
	if (Utils::smOrEq(mInfoGain, 0))
	{
		return;
	}

	// Set instance variables' values to values for
	// best split.
	mNumSubsets = 2;
	mSplitPoint = (trainInstances->instance(splitIndex + 1)->value(mAttIndex) + trainInstances->instance(splitIndex)->value(mAttIndex)) / 2;

	// In case we have a numerical precision problem we need to choose the
	// smaller value
	if (mSplitPoint == trainInstances->instance(splitIndex + 1)->value(mAttIndex))
	{
		mSplitPoint = trainInstances->instance(splitIndex)->value(mAttIndex);
	}

	// Restore distributioN for best split.
	mDistribution = new Distribution(2, trainInstances->numClasses());
	mDistribution->addRange(0, trainInstances, 0, splitIndex + 1);
	mDistribution->addRange(1, trainInstances, splitIndex + 1, firstMiss);

	// Compute modified gain ratio for best split.
	mGainRatio = gainRatioCrit->splitCritValue(mDistribution, mSumOfWeights, mInfoGain);
}

double C45Split::infoGain()
{

	return mInfoGain;
}

std::string C45Split::leftSide(Instances *data)
{

	return data->attribute(mAttIndex)->name();
}

std::string C45Split::rightSide(int index, Instances *data)
{

	std::string text;

	text = "";
	if (data->attribute(mAttIndex)->isNominal())
	{
		text.append(std::string(" = ") + data->attribute(mAttIndex)->value(index));
	}
	else if (index == 0)
	{
		text.append(std::string(" <= ") + Utils::doubleToString(mSplitPoint, 6));
	}
	else
	{
		text.append(std::string(" > ") + Utils::doubleToString(mSplitPoint, 6));
	}

	return text;
}

std::string C45Split::sourceExpression(int index, Instances *data)
{

	std::string expr;
	if (index < 0)
	{
		return std::string("i[") + std::to_string(mAttIndex) + std::string("] == null");
	}
	if (data->attribute(mAttIndex)->isNominal())
	{
		expr = "i[";
		expr.append(std::to_string(mAttIndex)).append("]");
		expr.append(".equals(\"").append(data->attribute(mAttIndex)->value(index)).append("\")");
	}
	else
	{
		expr = "((Double) i[";
		expr.append(std::to_string(mAttIndex)).append("])");
		if (index == 0)
		{
			expr.append(".doubleValue() <= ").append(std::to_string(mSplitPoint));
		}
		else
		{
			expr.append(".doubleValue() > ").append(std::to_string(mSplitPoint));
		}
	}

	return expr;
}

void C45Split::setSplitPoint(Instances *allInstances)
{

	double newSplitPoint = -std::numeric_limits<double>::max();
	double tempValue;
	Instance *instance;

	if ((allInstances->attribute(mAttIndex)->isNumeric()) && (mNumSubsets > 1))
	{
		int totalInst = allInstances->numInstances();
		for (int i = 0; i < totalInst; i++)
		{
			instance = allInstances->instance(i);
			if (!instance->isMissing(mAttIndex))
			{
				tempValue = instance->value(mAttIndex);
				if (Utils::gr(tempValue, newSplitPoint) && Utils::smOrEq(tempValue, mSplitPoint))
				{
					newSplitPoint = tempValue;
				}
			}
		}
		mSplitPoint = newSplitPoint;
	}
}

std::vector<std::vector<double>> C45Split::minsAndMaxs(Instances *data, std::vector<std::vector<double>> &minsAndMaxs, int index)
{

	std::vector<std::vector<double>> newMinsAndMaxs(data->numAttributes(), std::vector<double>(2, 0));

	for (int i = 0; i < data->numAttributes(); i++)
	{
		newMinsAndMaxs[i][0] = minsAndMaxs[i][0];
		newMinsAndMaxs[i][1] = minsAndMaxs[i][1];
		if (i == mAttIndex)
		{
			if (data->attribute(mAttIndex)->isNominal())
			{
				newMinsAndMaxs[mAttIndex][1] = 1;
			}
			else
			{
				newMinsAndMaxs[mAttIndex][1 - index] = mSplitPoint;
			}
		}
	}

	return newMinsAndMaxs;
}

void C45Split::resetDistribution(Instances *data)
{

	Instances *insts = new Instances(data, 0);
	for (int i = 0; i < data->numInstances(); i++)
	{
		if (whichSubset(data->instance(i)) > -1)
		{
			insts->add(data->instance(i));
		}
	}
	Distribution *newD = new Distribution(insts, this);
	newD->addInstWithUnknown(data, mAttIndex);
	mDistribution = newD;
}

std::vector<double> C45Split::weights(Instance *instance)
{

	std::vector<double> weights;
	int i;

	if (instance->isMissing(mAttIndex))
	{
		weights = std::vector<double>(mNumSubsets);
		for (i = 0; i < mNumSubsets; i++)
		{
			weights[i] = mDistribution->perBag(i) / mDistribution->total();
		}
		return weights;
	}
	else
	{
		return{ 0 } ;
	}
}

int C45Split::whichSubset(Instance *instance)
{

	if (instance->isMissing(mAttIndex))
	{
		return -1;
	}
	else
	{
		if (instance->attribute(mAttIndex)->isNominal())
		{
			return static_cast<int>(instance->value(mAttIndex));
		}
		else if (Utils::smOrEq(instance->value(mAttIndex), mSplitPoint))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
