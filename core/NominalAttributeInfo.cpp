#include "NominalAttributeInfo.h"
#include <iostream>
#include <exception>      // std::out_of_range


NominalAttributeInfo::NominalAttributeInfo(std::vector<std::string> &attributeValues, const std::string &attributeName)
{

	if (attributeValues.empty())
	{
		mValues = std::vector<std::string>();
		mHashtable = std::unordered_map<std::string, int>();
	}
	else
	{
		mValues = std::vector<std::string>(attributeValues.size());
		mHashtable = std::unordered_map<std::string, int>(attributeValues.size());
		for (int i = 0; i < (int)attributeValues.size(); i++)
		{
			//std::cout << "before" << attributeValues[i] << std::endl;
			auto store = attributeValues[i];
			//std::cout << "after" << store << std::endl;
			if (mHashtable.find(store) != mHashtable.end())
			{
				throw std::invalid_argument(std::string("A nominal attribute (") + attributeName + std::string(") cannot") +
					std::string(" have duplicate labels (") + attributeValues[i] + std::string(")."));
			}
			mValues.push_back(store);
			mHashtable[store] = int(i);
		}
	}
}

int NominalAttributeInfo::getAttributeIndex(const std::string key)
{
	try
	{
		/*for(auto &par : mHashtable)
		 {
		 std::cout << par.first << ": " << " -> " << par.second << std::endl;
		 }*/
		return mHashtable.at(key);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
}

size_t NominalAttributeInfo::getSize(void)
{
	return mValues.size();
}

size_t NominalAttributeInfo::getHashTableSize(void)
{
	return mHashtable.size();
}

void NominalAttributeInfo::setAttributeValue(int key, std::string value)
{
	mValues.assign(key, value);
}

std::string NominalAttributeInfo::getAttributeValue(const int key)
{
	return mValues[key];
}

void NominalAttributeInfo::addAttribute(const std::string key, int index, bool overwriteIndex)
{
	if (overwriteIndex)
		mValues.push_back(key);
	else
		mValues[index] = key;

	mHashtable[key] = int(index);
}

void NominalAttributeInfo::setHashTable(std::unordered_map<std::string, int> hashtable)
{
	mHashtable = hashtable;
}

void NominalAttributeInfo::removeHashTable(std::string key)
{
	mHashtable.erase(key);
}

void NominalAttributeInfo::removeIndex(const int index)
{
	mValues.erase(mValues.begin() + index);
}

std::unordered_map<std::string, int> NominalAttributeInfo::getHashTable()
{
	return mHashtable;
}

void NominalAttributeInfo::clearAttribute(void)
{
	mValues.clear();
	mHashtable.clear();
}
