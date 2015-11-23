#include "DataSource.h"
#include "Instance.h"
#include "DenseInstance.h"
#include "Instances.h"
#include "Attribute.h"
#include "Utils.h"
#include "Consts.h"
#include <sstream>

DataSource::DataSource(const std::string &location)
{
	mNumAttribs = 0;
	FILE *file = fopen(location.c_str(), "r");;
	mStructure = nullptr;

	if (file == nullptr)
	{
		throw  "Source file is not found";
	}

	std::string fname = location.substr(location.find_last_of("/\\")+1, location.length());
	std::string fileStem;
	std::string path = location.substr(0, location.find_last_of("/\\"));

	if (fname.find('.') < 0)
	{
		fileStem = fname;
		fname += std::string(".names");
	}
	else
	{
		fileStem = fname.substr(0, fname.rfind('.'));
		fname = fileStem + std::string(".names");
	}
	mFileStem = fileStem;
	mSourceFileName = path + "/" + fname;
	try
	{
		mNamesReader.open(mSourceFileName, std::ios_base::in);
	}
	catch (std::exception ex)
	{
		throw std::string("File not found : ") + (path + "/" + fname);
	}

	mSourceFileData = path + "/" + fileStem + std::string(".data");
	try
	{
		mDataReader.open(mSourceFileData, std::ios_base::in);
	}
	catch (std::exception ex)
	{
		throw std::string("File not found : ") + (path + "/" + fname);
	}
}

void DataSource::reset()
{
	mDataReader.close();
	mNamesReader.close();
}

Instances *DataSource::getStructure()
{
	if (mSourceFileName.empty())
	{
		throw "No source has been specified";
	}

	if (mStructure == nullptr)
	{
		readHeader(mNamesReader);
	}

	return mStructure;
}

Instances *DataSource::getStructure(int classIndex)
{
	Instances *result;
	result = getStructure();
	if (result != nullptr)
	{
		result->setClassIndex(classIndex);
	}

	return result;
}

void DataSource::readHeader(std::fstream &inNameStream)
{

	std::vector<Attribute*> attribDefs;
	std::vector<int> ignores;

	if (inNameStream.good() && inNameStream.eof())
	{
		throw  "premature end of file";
	}

	mNumAttribs = 1;
	// Read the class values
	std::vector<std::string> classVals;
	std::string line;

	while (std::getline(inNameStream, line))
	{
		if (line.empty() || line.find("|") == 0)
		{
			continue;
		}
		std::stringstream ss(line);
		std::string val;
		while (std::getline(ss, val, ','))
		{
			if (val.length() > 0)
			{
				val = removeTrailingPeriod(val);
				classVals.push_back(val);
			}
		}
		break;
	}

	// read the attribute names and types
	int counter = 0;
	std::vector<std::string> attributeCollection;
	std::stringstream ss;
	std::string token;
	std::string attribName;
	std::stringstream attributeType;
	std::vector<std::string> attribVals;
	while (std::getline(inNameStream, line))
	{
		ss.clear();
		if (line.empty() || line.find("|") == 0)
		{
			continue;
		}
		ss << line;
		attributeCollection.clear();
		attributeType.clear();
		while (std::getline(ss, token, ':'))
		{
			if (token.length() > 0)
			{
				token = removeTrailingPeriod(token);
				attributeCollection.push_back(token);
			}
		}
		attribName = attributeCollection[0];
		attributeType << attributeCollection[1];

		// read the values of the attribute
		attribVals.clear();

		while (std::getline(attributeType, token, ','))
		{
			if (token.length() > 0)
			{
				token = removeTrailingPeriod(token);
			}
			if (token.compare("ignore") == 0 || token.compare("label") == 0)
			{
				ignores.push_back(int(counter));
				counter++;
			}
			else if (token.compare("continuous") == 0)
			{
				attribDefs.push_back(new Attribute(attribName));
				counter++;
			}
			else
			{
				counter++;
				attribVals.push_back(token);
			}
		}
		if (attribVals.size() > 0)
			attribDefs.push_back(new Attribute(attribName, attribVals));
	}

	bool ok = true;
	int i = -1;
	if (classVals.size() == 1)
	{
		// look to see if this is an attribute name (ala c5 names file style)
		for (i = 0; i < (int)attribDefs.size(); i++)
		{
			if ((attribDefs[i]->name()).compare(classVals[0]) == 0)
			{
				ok = false;
				mNumAttribs--;
				break;
			}
		}
	}

	if (ok)
	{
		attribDefs.push_back(new Attribute("Class", classVals));
	}

	mStructure = new Instances(mFileStem, attribDefs, 0);

	try
	{
		if (ok)
		{
			mStructure->setClassIndex(mStructure->numAttributes() - 1);
		}
		else
		{
			mStructure->setClassIndex(i);
		}
	}
	catch (std::exception &ex)
	{
		throw ex;
	}

	mNumAttribs = mStructure->numAttributes() + (int)ignores.size();
	mIgnore = std::vector<bool>(mNumAttribs);
	for (int i = 0; i < (int)ignores.size(); i++)
	{
		mIgnore[ignores[i]] = true;
	}
}
std::string DataSource::removeTrailingPeriod(std::string &val)
{
	val.erase(val.find_last_not_of(TRIMCHARS) + 1);
	val.erase(0, val.find_first_not_of(TRIMCHARS));
	return val;
}

Instance *DataSource::getInstance(std::string inData)
{
	std::vector<double> instance = std::vector<double>(mStructure->numAttributes());

	int i = 0, counter = 0;
	std::stringstream dataStream;
	std::string value;

	dataStream << inData;
	while (std::getline(dataStream, value, ','))
	{
		if (!mIgnore[i])
		{
			value = removeTrailingPeriod(value);
			if (value.compare("?") == 0)
			{
				instance[counter++] = Utils::missingValue();
			}
			else
			{
				if (mStructure->attribute(counter)->isNominal())
				{
					int index = mStructure->attribute(counter)->indexOfValue(value);
					if (index == -1)
					{
						throw std::string("nominal value not declared in header :") + value + std::string(" column ") + std::to_string(i);
					}
					instance[counter++] = index;
				}
				else if (mStructure->attribute(counter)->isNumeric())
				{
					instance[counter++] = std::stod(value);

				}
				else
				{
					throw "Shouldn't get here";
				}
			}
		}
		i++;
	}
	return new DenseInstance(1.0, instance);
}

Instances *DataSource::getDataSet()
{
	Instances *result = nullptr;
	if (mStructure == nullptr)
		getStructure();
	result = getInstance(mDataReader);
	try
	{
		// close the stream
		mDataReader.close();
		// reset();
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
	return result;
}

Instances *DataSource::getInstance(std::fstream& inDataStream)
{
	if (inDataStream.good() && inDataStream.eof())
	{
		throw  "premature end of file";
	}
	std::string line;

	Instances *result = new Instances(mStructure);
	while (std::getline(inDataStream, line))
	{
		if (line.empty() || line.find("|") == 0)
		{
			continue;
		}
		result->add(getInstance(line));
	}
	return result;
}
