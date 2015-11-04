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
//#include "Random.h"

class Instances : Instance
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
	virtual void initialize(Instances *dataset, int capacity);

public:
	Instances(const std::string &name, std::vector<Attribute*> &attInfo, int capacity);
	Instances(Instances *dataset);
	Instances(Instances *dataset, int capacity);
	virtual Attribute *attribute(int index);
	virtual Attribute *attribute(const std::string &name);
	virtual int numAttributes();
	virtual int classIndex();
	virtual bool add(Instance *instance);
	virtual void add(int index, Instance *instance);

	virtual void setClassIndex(int classIndex);
	virtual int numInstances();
	virtual void copyInstances(int from, Instances *dest, int num);
	virtual Instance *instance(int index);
	virtual Attribute *classAttribute();
	virtual int numClasses();
	virtual double classValue();
	virtual double weight();
	virtual void setWeight(double weight);
	virtual bool isMissing(int attIndex);
	virtual Instance *lastInstance();
	virtual double value(int attIndex);

	virtual void Sort(int attIndex);
	virtual void Sort(Attribute *att);
	virtual void sortBasedOnNominalAttribute(int attIndex);
	void deleteWithMissingClass();
	void deleteWithMissing(Attribute *att);
	void deleteWithMissing(int attIndex);
	double sumOfWeights();
	std::string toString();
	virtual Instances *trainCV(int numFolds, int numFold);
	virtual Instances *testCV(int numFolds, int numFold);

};


#endif // _INSTANCES_HPP
