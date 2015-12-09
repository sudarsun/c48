#ifndef _INSTANCE_
#define _INSTANCE_

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "Attribute.h"

class Instances;

class Instance
{

public:
	//int s_numericAfterDecimalPoint;
	Instance( Instance *instance );
	Instance(double weight, std::vector<double> attValues);
	//DenseInstance( int numAttributes );
	Attribute *attribute(int index);
	int classIndex();
	int numAttributes();
	//double weight();
	void setClassMissing();
	void setMissing(int attIndex);
	void setValue(int attIndex, const std::string &value);
	void setValue(int attIndex, double value);
	void setDataset(Instances* data);
	Instances* getDataset();
	double classValue();
	double weight();
	double value(int attIndex);
	std::vector<double> toDoubleArray();
	Attribute *classAttribute();
	int numClasses();
	bool isMissing(int attIndex);
	void setWeight(double weight);
	double missingValue();
	bool classIsMissing();
	Instance* copy();
	
protected:
	std::vector<double> mAttValues;
	double mWeight;
	Instances *mDataset;

private:
	void freshAttributeVector();

};

#endif	// _DENSEINSTANCE_
