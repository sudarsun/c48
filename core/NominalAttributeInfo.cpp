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
        //mValues = string_array(attributeValues.size());
        //mHashtable = std::unordered_map<string, int>(attributeValues.size());
        for (int i = 0; i < (int)attributeValues.size(); i++)
        {
            //std::cout << "before" << attributeValues[i] << std::endl;
            auto store = attributeValues[i];
            //std::cout << "after" << store << std::endl;
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

int NominalAttributeInfo::getAttributeIndex(const string key)
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

void NominalAttributeInfo::setAttributeValue(int key, string value)
{
    mValues.assign(key, value);
}

string NominalAttributeInfo::getAttributeValue(const int key)
{
    return mValues[key];
}

void NominalAttributeInfo::addAttribute(const string key, int index, bool overwriteIndex)
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

void NominalAttributeInfo::removeHashTable(string key)
{
    mHashtable.erase(key);
}

void NominalAttributeInfo::removeIndex(const int index)
{
    mValues.erase(mValues.begin() + index);
}

string_hashtable NominalAttributeInfo::getHashTable()
{
    return mHashtable;
}

void NominalAttributeInfo::clearAttribute(void)
{
    mValues.clear();
    mHashtable.clear();
}
