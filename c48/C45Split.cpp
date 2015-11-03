#include "C45Split.h"
#include "core/Instances.h"
#include "InfoGainSplitCrit.h"
#include "GainRatioSplitCrit.h"
#include "core/Instance.h"
#include "Distribution.h"
#include "core/Utils.h"


InfoGainSplitCrit *C45Split::infoGainCrit = new InfoGainSplitCrit();
GainRatioSplitCrit *C45Split::gainRatioCrit = new GainRatioSplitCrit();

C45Split::C45Split(int attIndex, int minNoObj, double sumOfWeights, bool useMDLcorrection) : mattIndex(attIndex), mminNoObj(minNoObj), museMDLcorrection(useMDLcorrection), msumOfWeights(sumOfWeights)
{

	// Get index of attribute to split on.

	// Set minimum number of objects.

	// Set the sum of the weights

	// Whether to use the MDL correction for numeric attributes
}

void C45Split::buildClassifier(Instances *trainInstances)
{

	// Initialize the remaining instance variables.
	mnumSubsets = 0;
	msplitPoint = std::numeric_limits<double>::max();
	minfoGain = 0;
	mgainRatio = 0;

	// Different treatment for enumerated and numeric
	// attributes.
	if (trainInstances->attribute(mattIndex)->isNominal())
	{
		mcomplexityIndex = trainInstances->attribute(mattIndex)->numValues();
		mindex = mcomplexityIndex;
		handleEnumeratedAttribute(trainInstances);
	}
	else
	{
		mcomplexityIndex = 2;
		mindex = 0;
		trainInstances->Sort(trainInstances->attribute(mattIndex));
		handleNumericAttribute(trainInstances);
	}
}

int C45Split::attIndex()
{

	return mattIndex;
}

double C45Split::splitPoint()
{
	return msplitPoint;
}

double C45Split::classProb(int classIndex, Instance *instance, int theSubset)
{

	if (theSubset <= -1)
	{
		std::vector<double> _weights = weights(instance);
		if (_weights.empty())
		{
			return mdistribution->prob(classIndex);
		}
		else
		{
			double prob = 0;
			for (int i = 0; i < _weights.size(); i++)
			{
				prob += _weights[i] * mdistribution->prob(classIndex, i);
			}
			return prob;
		}
	}
	else
	{
		if (Utils::gr(mdistribution->perBag(theSubset), 0))
		{
			return mdistribution->prob(classIndex, theSubset);
		}
		else
		{
			return mdistribution->prob(classIndex);
		}
	}
}

double C45Split::codingCost()
{

	return Utils::getLog2(mindex);
}

double C45Split::gainRatio()
{
	return mgainRatio;
}

void C45Split::handleEnumeratedAttribute(Instances *trainInstances)
{

	Instance *instance;

	mdistribution = new Distribution(mcomplexityIndex, trainInstances->numClasses());

	// Only Instances with known values are relevant.
	int totalInst = trainInstances->numInstances();
	for (int i = 0; i < totalInst; i++)
	{
		instance = trainInstances->instance(i);
		if (!instance->isMissing(mattIndex))
		{
			mdistribution->add(static_cast<int>(instance->value(mattIndex)), instance);
		}
	}

	// Check if minimum number of Instances in at least two
	// subsets.
	if (mdistribution->check(mminNoObj))
	{
		mnumSubsets = mcomplexityIndex;
		minfoGain = infoGainCrit->splitCritValue(mdistribution, msumOfWeights);
		mgainRatio = gainRatioCrit->splitCritValue(mdistribution, msumOfWeights, minfoGain);
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
	mdistribution = new Distribution(2, trainInstances->numClasses());

	// Only Instances with known values are relevant.
	int totalInst = trainInstances->numInstances();
	i = 0;
	for (int j = 0; j < totalInst; j++)
	{
		instance = trainInstances->instance(j);
		if (instance->isMissing(mattIndex))
		{
			break;
		}
		mdistribution->add(1, instance);
		i++;
	}
	firstMiss = i;

	// Compute minimum number of Instances required in each
	// subset.
	minSplit = 0.1 * (mdistribution->total()) / (trainInstances->numClasses());
	if (Utils::smOrEq(minSplit, mminNoObj))
	{
		minSplit = mminNoObj;
	}
	else if (Utils::gr(minSplit, 25))
	{
		minSplit = 25;
	}

	// Enough Instances with known values?
	if (Utils::sm(firstMiss, 2 * minSplit))
	{
		return;
	}

	// Compute values of criteria for all possible split
	// indices.
	defaultEnt = infoGainCrit->oldEnt(mdistribution);
	while (next < firstMiss)
	{

		if (trainInstances->instance(next - 1)->value(mattIndex) + 1e-5 < trainInstances->instance(next)->value(mattIndex))
		{

			// Move class values for all Instances up to next
			// possible split point.
			mdistribution->shiftRange(1, 0, trainInstances, last, next);

			// Check if enough Instances in each subset and compute
			// values for criteria.
			if (Utils::grOrEq(mdistribution->perBag(0), minSplit) && Utils::grOrEq(mdistribution->perBag(1), minSplit))
			{
				currentInfoGain = infoGainCrit->splitCritValue(mdistribution, msumOfWeights, defaultEnt);
				if (Utils::gr(currentInfoGain, minfoGain))
				{
					minfoGain = currentInfoGain;
					splitIndex = next - 1;
				}
				mindex++;
			}
			last = next;
		}
		next++;
	}

	// Was there any useful split?
	if (mindex == 0)
	{
		return;
	}

	// Compute modified information gain for best split.
	if (museMDLcorrection)
	{
		minfoGain = minfoGain - (Utils::log2(mindex) / msumOfWeights);
	}
	if (Utils::smOrEq(minfoGain, 0))
	{
		return;
	}

	// Set instance variables' values to values for
	// best split.
	mnumSubsets = 2;
	msplitPoint = (trainInstances->instance(splitIndex + 1)->value(mattIndex) + trainInstances->instance(splitIndex)->value(mattIndex)) / 2;

	// In case we have a numerical precision problem we need to choose the
	// smaller value
	if (msplitPoint == trainInstances->instance(splitIndex + 1)->value(mattIndex))
	{
		msplitPoint = trainInstances->instance(splitIndex)->value(mattIndex);
	}

	// Restore distributioN for best split.
	mdistribution = new Distribution(2, trainInstances->numClasses());
	mdistribution->addRange(0, trainInstances, 0, splitIndex + 1);
	mdistribution->addRange(1, trainInstances, splitIndex + 1, firstMiss);

	// Compute modified gain ratio for best split.
	mgainRatio = gainRatioCrit->splitCritValue(mdistribution, msumOfWeights, minfoGain);
}

double C45Split::infoGain()
{

	return minfoGain;
}

std::string C45Split::leftSide(Instances *data)
{

	return data->attribute(mattIndex)->name();
}

std::string C45Split::rightSide(int index, Instances *data)
{

	std::string text;

	text = "";
	if (data->attribute(mattIndex)->isNominal())
	{
		text.append(std::string(" = ") + data->attribute(mattIndex)->value(index));
	}
	else if (index == 0)
	{
		text.append(std::string(" <= ") + Utils::doubleToString(msplitPoint, 6));
	}
	else
	{
		text.append(std::string(" > ") + Utils::doubleToString(msplitPoint, 6));
	}
	//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
	return text;
}

std::string C45Split::sourceExpression(int index, Instances *data)
{

	std::string expr;
	if (index < 0)
	{
		return std::string("i[") + std::to_string(mattIndex) + std::string("] == null");
	}
	if (data->attribute(mattIndex).isNominal())
	{
		expr = new StringBuffer("i[");
		expr.append(mattIndex).append("]");
		expr.append(".equals(\"").append(data->attribute(mattIndex)->value(index)).append("\")");
	}
	else
	{
		expr = "((Double) i[";
		expr.append(mattIndex).append("])");
		if (index == 0)
		{
			expr.append(".doubleValue() <= ").append(msplitPoint);
		}
		else
		{
			expr.append(".doubleValue() > ").append(msplitPoint);
		}
	}

	return expr;
}

void C45Split::setSplitPoint(Instances *allInstances)
{

	double newSplitPoint = -std::numeric_limits<double>::max();
	double tempValue;
	Instance *instance;

	if ((allInstances->attribute(mattIndex)->isNumeric()) && (mnumSubsets > 1))
	{
		int totalInst = allInstances->numInstances();
		for (int i = 0; i < totalInst; i++)
		{
			instance = allInstances->instance(i);
			if (!instance->isMissing(mattIndex))
			{
				tempValue = instance->value(mattIndex);
				if (Utils::gr(tempValue, newSplitPoint) && Utils::smOrEq(tempValue, msplitPoint))
				{
					newSplitPoint = tempValue;
				}
			}
		}
		msplitPoint = newSplitPoint;
	}
}

std::vector<std::vector<double>> C45Split::minsAndMaxs(Instances *data, std::vector<std::vector<double>> &minsAndMaxs, int index)
{

	//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
	//ORIGINAL LINE: double[][] newMinsAndMaxs = new double[data.numAttributes()][2];
	std::vector<std::vector<double>> newMinsAndMaxs = RectangularVectors::ReturnRectangularDoubleVector(data->numAttributes(), 2);

	for (int i = 0; i < data->numAttributes(); i++)
	{
		newMinsAndMaxs[i][0] = minsAndMaxs[i][0];
		newMinsAndMaxs[i][1] = minsAndMaxs[i][1];
		if (i == mattIndex)
		{
			if (data->attribute(mattIndex)->isNominal())
			{
				newMinsAndMaxs[mattIndex][1] = 1;
			}
			else
			{
				newMinsAndMaxs[mattIndex][1 - index] = msplitPoint;
			}
		}
	}

	return newMinsAndMaxs;
}

void C45Split::resetDistribution(Instances *data)
{

	Instances *insts = new Instances(data, data->numInstances());
	for (int i = 0; i < data->numInstances(); i++)
	{
		if (whichSubset(data->instance(i)) > -1)
		{
			insts->add(data->instance(i));
		}
	}
	Distribution *newD = new Distribution(insts, this);
	newD->addInstWithUnknown(data, mattIndex);
	mdistribution = newD;
}

std::vector<double> C45Split::weights(Instance *instance)
{

	std::vector<double> weights;
	int i;

	if (instance->isMissing(mattIndex))
	{
		weights = std::vector<double>(mnumSubsets);
		for (i = 0; i < mnumSubsets; i++)
		{
			weights[i] = mdistribution->perBag(i) / mdistribution->total();
		}
		return weights;
	}
	else
	{
		return (0);
	}
}

int C45Split::whichSubset(Instance *instance)
{

	if (instance->isMissing(mattIndex))
	{
		return -1;
	}
	else
	{
		if (instance->attribute(mattIndex)->isNominal())
		{
			return static_cast<int>(instance->value(mattIndex));
		}
		else if (Utils::smOrEq(instance->value(mattIndex), msplitPoint))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
