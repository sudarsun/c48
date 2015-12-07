#ifndef _INSTANCES_HPP
#define _INSTANCES_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include <fstream>

#include "Attribute.h"
#include "AttributeStats.h"
#include "Instance.h"

class Instances
{

protected:

	std::string mRelationName;

	/// <summary>
	/// The attribute information. </summary>
	std::vector<Attribute*> mAttributes;

	/// <summary>
	/// A map to quickly find attribute indices based on their names. </summary>
	std::unordered_map<std::string, int> mNamesToAttributeIndices;

	/// <summary>
	/// The instances. </summary>
	std::vector<Instance*> mInstances;

	/// <summary>
	/// The class attribute's index </summary>
	int mClassIndex;
	double mWeight;
	void initialize(Instances *dataset, int capacity);

public:
	Instances(const std::string &name, std::vector<Attribute*> &attInfo, int capacity);
	Instances(Instances *dataset);
	Instances(Instances *dataset, int capacity);
	Attribute *attribute(int index);
	Attribute *attribute(const std::string &name);
	int numAttributes();
	int classIndex();
	bool add(Instance *instance);
	void add(int index, Instance *instance);

	void setClassIndex(int classIndex);
	int numInstances();
	void copyInstances(int from, Instances *dest, int num);
	Instance *instance(int index);
	Attribute *classAttribute();
	int numClasses();
	Instance *lastInstance();
	void setWeight(double weight);
	void Sort(int attIndex);
	void Sort(Attribute *att);
	void sortBasedOnNominalAttribute(int attIndex);
	void deleteWithMissingClass();
	void deleteWithMissing(Attribute *att);
	void deleteWithMissing(int attIndex);
	double sumOfWeights();
	Instances *trainCV(int numFolds, int numFold);
	Instances *testCV(int numFolds, int numFold);
	std::string getRelationName();
	void setRelationName(const std::string name);
	std::vector<double> attributeToDoubleArray(int index);

};

#endif // _INSTANCES_HPP
