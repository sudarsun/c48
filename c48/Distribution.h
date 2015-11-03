#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_

#include <string>
#include <vector>
#include <stdexcept>

class Instances;
class ClassifierSplitModel;
class Instance;


/// <summary>
/// Class for handling a distribution of class values.
/// </summary>
class Distribution
{

	/// <summary>
	/// Weight of instances per class per bag. </summary>
protected:
	std::vector<std::vector<double>> mperClassPerBag;

	/// <summary>
	/// Weight of instances per bag. </summary>
	std::vector<double> mperBag;

	/// <summary>
	/// Weight of instances per class. </summary>
	std::vector<double> mperClass;

	/// <summary>
	/// Total weight of instances. </summary>
	double totaL = 0;

	/// <summary>
	/// Creates and initializes a new distribution.
	/// </summary>
public:
	Distribution(int numBags, int numClasses);

	/// <summary>
	/// Creates and initializes a new distribution using the given array. WARNING:
	/// it just copies a reference to this array.
	/// </summary>
	Distribution(std::vector<std::vector<double>> &table);

	/// <summary>
	/// Creates a distribution with only one bag according to instances in source.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	Distribution(Instances *source);

	/// <summary>
	/// Creates a distribution according to given instances and split model.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>

	Distribution(Instances *source, ClassifierSplitModel *modelToUse);

	/// <summary>
	/// Creates distribution with only one bag by merging all bags of given
	/// distribution.
	/// </summary>
	Distribution(Distribution *toMerge);

	/// <summary>
	/// Creates distribution with two bags by merging all bags apart of the
	/// indicated one.
	/// </summary>
	Distribution(Distribution *toMerge, int index);

	/// <summary>
	/// Returns number of non-empty bags of distribution.
	/// </summary>
	int actualNumBags();

	/// <summary>
	/// Returns number of classes actually occuring in distribution.
	/// </summary>
	int actualNumClasses();

	/// <summary>
	/// Returns number of classes actually occuring in given bag.
	/// </summary>
	int actualNumClasses(int bagIndex);

	/// <summary>
	/// Adds given instance to given bag.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void add(int bagIndex, Instance *instance);

	/// <summary>
	/// Subtracts given instance from given bag.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void sub(int bagIndex, Instance *instance);

	/// <summary>
	/// Adds counts to given bag.
	/// </summary>
	void add(int bagIndex, std::vector<double> &counts);

	/// <summary>
	/// Adds all instances with unknown values for given attribute, weighted
	/// according to frequency of instances in each bag.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void addInstWithUnknown(Instances *source, int attIndex);

	/// <summary>
	/// Adds all instances in given range to given bag.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void addRange(int bagIndex, Instances *source, int startIndex, int lastPlusOne);

	/// <summary>
	/// Adds given instance to all bags weighting it according to given weights.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void addWeights(Instance *instance, std::vector<double> &weights);

	/// <summary>
	/// Checks if at least two bags contain a minimum number of instances.
	/// </summary>
	bool check(double minNoObj);

	/// <summary>
	/// Clones distribution (Deep copy of distribution).
	/// </summary>
	void *clone();

	/// <summary>
	/// Deletes given instance from given bag.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void del(int bagIndex, Instance *instance);

	/// <summary>
	/// Deletes all instances in given range from given bag.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void delRange(int bagIndex, Instances *source, int startIndex, int lastPlusOne);

	/// <summary>
	/// Prints distribution.
	/// </summary>

	std::string dumpDistribution();

	/// <summary>
	/// Sets all counts to zero.
	/// </summary>
	void initialize();

	/// <summary>
	/// Returns matrix with distribution of class values.
	/// </summary>
	std::vector<std::vector<double>> matrix();

	/// <summary>
	/// Returns index of bag containing maximum number of instances.
	/// </summary>
	int maxBag();

	/// <summary>
	/// Returns class with highest frequency over all bags.
	/// </summary>
	int maxClass();

	/// <summary>
	/// Returns class with highest frequency for given bag.
	/// </summary>
	int maxClass(int index);

	/// <summary>
	/// Returns number of bags.
	/// </summary>
	int numBags();

	/// <summary>
	/// Returns number of classes.
	/// </summary>
	int numClasses();

	/// <summary>
	/// Returns perClass(maxClass()).
	/// </summary>
	double numCorrect();

	/// <summary>
	/// Returns perClassPerBag(index,maxClass(index)).
	/// </summary>
	double numCorrect(int index);

	/// <summary>
	/// Returns total-numCorrect().
	/// </summary>
	double numIncorrect();

	/// <summary>
	/// Returns perBag(index)-numCorrect(index).
	/// </summary>
	double numIncorrect(int index);

	/// <summary>
	/// Returns number of (possibly fractional) instances of given class in given
	/// bag.
	/// </summary>
	double perClassPerBag(int bagIndex, int classIndex);

	/// <summary>
	/// Returns number of (possibly fractional) instances in given bag.
	/// </summary>
	double perBag(int bagIndex);

	/// <summary>
	/// Returns number of (possibly fractional) instances of given class.
	/// </summary>
	double perClass(int classIndex);

	/// <summary>
	/// Returns relative frequency of class over all bags with Laplace correction.
	/// </summary>
	double laplaceProb(int classIndex);

	/// <summary>
	/// Returns relative frequency of class for given bag.
	/// </summary>
	double laplaceProb(int classIndex, int intIndex);

	/// <summary>
	/// Returns relative frequency of class over all bags.
	/// </summary>
	double prob(int classIndex);

	/// <summary>
	/// Returns relative frequency of class for given bag.
	/// </summary>
	double prob(int classIndex, int intIndex);

	/// <summary>
	/// Subtracts the given distribution from this one. The results has only one
	/// bag.
	/// </summary>
	Distribution *subtract(Distribution *toSubstract);

	/// <summary>
	/// Returns total number of (possibly fractional) instances.
	/// </summary>
	double total();

	/// <summary>
	/// Shifts given instance from one bag to another one.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void shift(int from, int to, Instance *instance);

	/// <summary>
	/// Shifts all instances in given range from one bag to another one.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	void shiftRange(int from, int to, Instances *source, int startIndex, int lastPlusOne);

};

#endif	// _DISTRIBUTION_
