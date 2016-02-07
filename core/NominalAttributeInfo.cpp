#include "NominalAttributeInfo.h"
#include <iostream>
#include <exception>      // std::out_of_range

NominalAttributeInfo::NominalAttributeInfo(string_array &attributeValues, const string &attributeName)
{

    if (attributeValues.empty())
    {
        mValues = string_array();
        mHashtable = string_hashtable();
    }
    else
    {
        for (int i = 0; i < (int)attributeValues.size(); i++)
        {
            auto store = attributeValues[i];
            if (mHashtable.find(store) != mHashtable.end())
            {
                throw std::invalid_argument(string("A nominal attribute (") + attributeName + string(") cannot") +
                    string(" have duplicate labels (") + attributeValues[i] + string(")."));
            }
            mValues.push_back(store);
            mHashtable[store] = int(i);
        }
    }
}

int NominalAttributeInfo::getAttributeIndex(const string &key) const
{
    try
    {
        return mHashtable.at(key);
    }
    catch (std::exception& ex)
    {
        throw ex;
    }
}

size_t NominalAttributeInfo::getSize(void) const
{
    return mValues.size();
}

size_t NominalAttributeInfo::getHashTableSize(void) const
{
    return mHashtable.size();
}

void NominalAttributeInfo::setAttributeValue(const int key, const string &value)
{
    mValues.assign(key, value);
}

string NominalAttributeInfo::getAttributeValue(const int key) const
{
    return mValues[key];
}

void NominalAttributeInfo::addAttribute(const string &key, int index, bool overwriteIndex)
{
    if (overwriteIndex)
        mValues.push_back(key);
    else
        mValues[index] = key;

    mHashtable[key] = int(index);
}

void NominalAttributeInfo::setHashTable(string_hashtable hashtable)
{
    mHashtable = hashtable;
}

void NominalAttributeInfo::removeHashTable(const string &key)
{
    mHashtable.erase(key);
}

void NominalAttributeInfo::removeIndex(const int index)
{
    mValues.erase(mValues.begin() + index);
}

string_hashtable NominalAttributeInfo::getHashTable() const
{
    return mHashtable;
}

void NominalAttributeInfo::clearAttribute(void)
{
    mValues.clear();
    mHashtable.clear();
}
