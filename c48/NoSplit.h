#ifndef _NOSPLIT_
#define _NOSPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <stdexcept>

class Distribution;
class Instances;
class Instance;


/// <summary>
/// Class implementing a "no-split"-split.
/// 
/// </summary>
class NoSplit : public ClassifierSplitModel {

	/// <summary>
	/// Creates "no-split"-split for given distribution.
	/// </summary>
public:
	NoSplit(Distribution *distribution);

	/// <summary>
	/// Creates a "no-split"-split for a given set of instances.
	/// </summary>
	/// <exception cref="Exception"> if split can't be built successfully </exception>
	virtual void buildClassifier(Instances instances);

	/// <summary>
	/// Always returns 0 because only there is only one subset.
	/// </summary>
	int whichSubset(Instance *instance);

	/// <summary>
	/// Always returns null because there is only one subset.
	/// </summary>
	std::vector<double> weights(Instance *instance);

	/// <summary>
	/// Does nothing because no condition has to be satisfied.
	/// </summary>
	std::string leftSide(Instances *instances);

	/// <summary>
	/// Does nothing because no condition has to be satisfied.
	/// </summary>
	std::string rightSide(int index, Instances *instances);

	/// <summary>
	/// Returns a string containing java source code equivalent to the test
	/// made at this node. The instance being tested is called "i".
	/// </summary>
	/// <param name="index"> index of the nominal value tested </param>
	/// <param name="data"> the data containing instance structure info </param>
	/// <returns> a value of type 'String' </returns>
	std::string sourceExpression(int index, Instances *data);

};

#endif	//#ifndef _NOSPLIT_
