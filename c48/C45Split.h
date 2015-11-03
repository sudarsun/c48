#ifndef _C45SPLIT_
#define _C45SPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

class Instances;
class InfoGainSplitCrit;
class GainRatioSplitCrit;
class Instance;
class Distribution;


/// <summary>
/// Class implementing a C4.5-type split on an attribute.
/// 
/// @author Eibe Frank (eibe@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class C45Split : public ClassifierSplitModel {

	/// <summary>
	/// Desired number of branches. </summary>
protected:
	int mcomplexityIndex = 0;

	/// <summary>
	/// Attribute to split on. </summary>
	const int mattIndex;

	/// <summary>
	/// Minimum number of objects in a split. </summary>
	const int mminNoObj;

	/// <summary>
	/// Use MDL correction? </summary>
	const bool museMDLcorrection;

	/// <summary>
	/// Value of split point. </summary>
	double msplitPoint = 0;

	/// <summary>
	/// InfoGain of split. </summary>
	double minfoGain = 0;

	/// <summary>
	/// GainRatio of split. </summary>
	double mgainRatio = 0;

	/// <summary>
	/// The sum of the weights of the instances. </summary>
	const double msumOfWeights;

	/// <summary>
	/// Number of split points. </summary>
	int mindex = 0;

	/// <summary>
	/// Static reference to splitting criterion. </summary>
	static InfoGainSplitCrit *infoGainCrit;

	/// <summary>
	/// Static reference to splitting criterion. </summary>
	static GainRatioSplitCrit *gainRatioCrit;

	/// <summary>
	/// Initializes the split model.
	/// </summary>
public:
	C45Split(int attIndex, int minNoObj, double sumOfWeights, bool useMDLcorrection);

	/// <summary>
	/// Creates a C4.5-type split on the given data. Assumes that none of the class
	/// values is missing.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual void buildClassifier(Instances *trainInstances);

	/// <summary>
	/// Returns index of attribute for which split was generated.
	/// </summary>
	int attIndex();

	/// <summary>
	/// Returns the split point (numeric attribute only).
	/// </summary>
	/// <returns> the split point used for a test on a numeric attribute </returns>
	virtual double splitPoint();

	/// <summary>
	/// Gets class probability for instance.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	double classProb(int classIndex, Instance *instance, int theSubset);

	/// <summary>
	/// Returns coding cost for split (used in rule learner).
	/// </summary>
	double codingCost();

	/// <summary>
	/// Returns (C4.5-type) gain ratio for the generated split.
	/// </summary>
	double gainRatio();

	/// <summary>
	/// Creates split on enumerated attribute.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
private:
	void handleEnumeratedAttribute(Instances *trainInstances);

	/// <summary>
	/// Creates split on numeric attribute.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void handleNumericAttribute(Instances *trainInstances);

	/// <summary>
	/// Returns (C4.5-type) information gain for the generated split.
	/// </summary>
public:
	double infoGain();

	/// <summary>
	/// Prints left side of condition..
	/// </summary>
	/// <param name="data"> training set. </param>
	std::string leftSide(Instances *data);

	/// <summary>
	/// Prints the condition satisfied by instances in a subset.
	/// </summary>
	/// <param name="index"> of subset </param>
	/// <param name="data"> training set. </param>
	std::string rightSide(int index, Instances *data);

	/// <summary>
	/// Returns a string containing java source code equivalent to the test made at
	/// this node. The instance being tested is called "i".
	/// </summary>
	/// <param name="index"> index of the nominal value tested </param>
	/// <param name="data"> the data containing instance structure info </param>
	/// <returns> a value of type 'String' </returns>
	std::string sourceExpression(int index, Instances *data);

	/// <summary>
	/// Sets split point to greatest value in given data smaller or equal to old
	/// split point. (C4.5 does this for some strange reason).
	/// </summary>
	void setSplitPoint(Instances *allInstances);

	/// <summary>
	/// Returns the minsAndMaxs of the index.th subset.
	/// </summary>
	std::vector<std::vector<double>> minsAndMaxs(Instances *data, std::vector<std::vector<double>> &minsAndMaxs, int index);

	/// <summary>
	/// Sets distribution associated with model.
	/// </summary>
	virtual void resetDistribution(Instances *data);

	/// <summary>
	/// Returns weights if instance is assigned to more than one subset. Returns
	/// null if instance is only assigned to one subset.
	/// </summary>
	std::vector<double> weights(Instance *instance);

	/// <summary>
	/// Returns index of subset instance is assigned to. Returns -1 if instance is
	/// assigned to more than one subset.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	int whichSubset(Instance *instance);

};


#endif	//#ifndef _C45SPLIT_
