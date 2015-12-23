#ifndef _NOMINALATTRIBUTEINFO_
#define _NOMINALATTRIBUTEINFO_

#include "AttributeInfo.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <exception>


/// <summary>
/// Stores information for nominal and string attributes.
/// </summary>
class NominalAttributeInfo : public AttributeInfo
{

    /// <summary>
    /// The attribute's values.
	/// </summary>
protected:
    string_array mValues;

    /// <summary>
    /// Mapping of values to indices. </summary>
    std::unordered_map<string, int> mHashtable;

    /// <summary>
    /// Constructs the info based on argument.
    /// </summary>
public:
    NominalAttributeInfo( string_array &attributeValues, const string &attributeName);
    int getAttributeIndex(const string key);
    size_t getSize(void);
    size_t getHashTableSize(void);
    void setHashTable(std::unordered_map<string, int> hashtable);
    std::unordered_map<string, int> getHashTable();
    string getAttributeValue(const int key);
    void setAttributeValue(const int key, string value);
    void addAttribute(const string key, int index, bool overwriteIndex = false);
    void clearAttribute(void);
    void removeHashTable(string key);
    void removeIndex(const int index);
};


#endif	// _NOMINALATTRIBUTEINFO_
