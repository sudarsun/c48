#ifndef _DENSEINSTANCE_
#define _DENSEINSTANCE_

#include "Instance.h"
#include "Attribute.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

class DenseInstance : public Instance
{

public:
	//int s_numericAfterDecimalPoint;
	//DenseInstance( Instance *instance );
	DenseInstance(double weight, std::vector<double> &attValues);
	//DenseInstance( int numAttributes );
	virtual Attribute *attribute(int index);
	virtual int classIndex();
	virtual int numAttributes();
	//virtual double weight();
	virtual void setClassMissing();
	virtual void setMissing(int attIndex);
	virtual void setValue(int attIndex, const std::string &value);
	virtual void setValue(int attIndex, double value);
	/*
	virtual void *copy();
	virtual int index( int position );
	virtual Instance *mergeInstance( Instance *inst );
	virtual int numAttributes();
	virtual int numValues();
	virtual void replaceMissingValues( std::vector<double> &array_Renamed );

	virtual void setValueSparse( int indexOfIndex, double value );
	virtual std::vector<double> toDoubleArray();
	virtual std::string toStringNoWeight();
	virtual std::string toStringNoWeight( int afterDecimalPoint );*/
	virtual double classValue();
	virtual double weight();
	virtual double value(int attIndex);
	virtual std::vector<double> toDoubleArray();
	virtual Attribute *classAttribute();
	virtual int numClasses();
	virtual bool isMissing(int attIndex);
	virtual void setWeight(double weight);
	
protected:
	std::vector<double> mAttValues;
	double mWeight;
	/*virtual void forceDeleteAttributeAt( int position );
	virtual void forceInsertAttributeAt( int position );*/

private:
	void freshAttributeVector();

};

#endif	// _DENSEINSTANCE_
