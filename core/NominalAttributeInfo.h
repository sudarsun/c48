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
    /// The attribute's values. </summary>
protected:
    std::vector<std::string> mValues; // @ spec_public @

    /// <summary>
    /// Mapping of values to indices. </summary>
    std::unordered_map<std::string, int> mHashtable;

    /// <summary>
    /// Constructs the info based on argument.
    /// </summary>
public:
    NominalAttributeInfo( std::vector<std::string> &attributeValues, const std::string &attributeName);
    int getAttributeIndex(const std::string key);
    size_t getSize(void);
    size_t getHashTableSize(void);
    void setHashTable(std::unordered_map<std::string, int> hashtable);
    std::unordered_map<std::string, int> getHashTable();
    std::string getAttributeValue(const int key);
    void setAttributeValue(const int key, std::string value);
    void addAttribute(const std::string key, int index, bool overwriteIndex = false);
    void clearAttribute(void);
    void removeHashTable(std::string key);
    void removeIndex(const int index);
};


#endif	// _NOMINALATTRIBUTEINFO_
