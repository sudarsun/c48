#ifndef _ABSTRACTINSTANCE_HPP
#define _ABSTRACTINSTANCE_HPP

#include <string>
#include <vector>

class Attribute;
class Instances;

class AbstractInstance
{

public:

	virtual Attribute *attribute(int index) = 0;
	virtual int classIndex() = 0;
	//virtual bool equalHeaders( Instance *inst ) = 0;
	//virtual std::string equalHeadersMsg( Instance *inst ) = 0;
	virtual int numAttributes() = 0;
	virtual Attribute *classAttribute() = 0;
	virtual int numClasses() = 0;
	virtual double classValue() = 0;
	virtual double weight() = 0;
	virtual void setWeight(double weight) = 0;
	virtual bool isMissing(int attIndex) = 0;
	virtual double missingValue() = 0;
	virtual double value(int attIndex) = 0;
	virtual void setDataset(Instances *data) = 0;
	virtual  Instances* getDataset() = 0;
	virtual bool classIsMissing() = 0;
};

#endif // _ABSTRACTINSTANCE_HPP
