#ifndef _NOMINALATTRIBUTEINFO_
#define _NOMINALATTRIBUTEINFO_

#include "AttributeInfo.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <exception>

/**
 * Stores information for nominal and string attributes.
 */
class NominalAttributeInfo : public AttributeInfo
{

protected:

    /** The attribute's values. */
    string_array mValues;

    /** Mapping of values to indices. */
    string_hashtable mHashtable;

public:

    /**
    * Constructs the info based on argument.
    */
    NominalAttributeInfo(string_array &attributeValues, const string &attributeName);

    /**
     * Returns the Attribute Index for the corrsponding key argument.
     */
    int getAttributeIndex(const string key);

    /**
     * Returns the number of attribute's values.
     */
    size_t getSize(void);

    /**
     * Returns the size of Hashtable.
     */
    size_t getHashTableSize(void);

    /**
     * Sets the Hashtable.
     */
    void setHashTable(string_hashtable hashtable);

    /**
     * Returns the Hashtable.
     */
    string_hashtable getHashTable();

    /**
     * Returns the attribute Value.
     */
    string getAttributeValue(const int key);

    /**
     * Sets the attribute Value.
     */
    void setAttributeValue(const int key, string value);

    /**
     * Adds the attribute with index.
     */
    void addAttribute(const string key, int index, bool overwriteIndex = false);

    /**
     * Clear the attributes.
     */
    void clearAttribute(void);

    /**
     * Deletes the record from Hashtable using key.
     */
    void removeHashTable(string key);

    /**
     * Deletes the record from Hashtable using index.
     */
    void removeIndex(const int index);
};


#endif    // _NOMINALATTRIBUTEINFO_
