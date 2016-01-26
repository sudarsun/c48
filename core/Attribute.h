#ifndef _ATTRIBUTE_HPP_
#define _ATTRIBUTE_HPP_

#include "AttributeInfo.h"
#include "NominalAttributeInfo.h"
#include <vector>

/**
 * Class for handling an attribute. Once an attribute has been created,
 * it can't be changed.
 *
 */
class Attribute
{

public:
    /** The attribute's name. */
    const string mName;

    /** The attribute's type. */
    int mType = 0;

    /** The attribute's index. */
    int mIndex = -1;

    /** The attribute's weight. */
    double mWeight = 1.0;

    /** The attribute's info. */
    AttributeInfo *mAttributeInfo;

    /**
     * Constructor for a numeric attribute.
     *
     * @param attributeName the name for the attribute
     */
    Attribute(const string &attributeName);

    /**
     * Constructor for nominal attributes and string attributes.
     * If a null vector of attribute values is passed to the method,
     * the attribute is assumed to be a string.
     *
     * @param attributeName the name for the attribute
     * @param attributeValues a vector of strings denoting the
     * attribute values. Null if the attribute is a string attribute.
     */
    Attribute(const string &attributeName, string_array &attributeValues);

    /**
    * Constructor for a numeric attribute with a particular index.
    *
    * @param attributeName the name for the attribute
    * @param index the attribute's index
    */
    // @ requires attributeName != null;
    // @ requires index >= 0;
    // @ ensures mName == attributeName;
    // @ ensures mIndex == index;
    Attribute(const string &attributeName, int index);

    /**
    * Constructor for date attributes with a particular index.
    *
    * @param attributeName the name for the attribute
    * @param dateFormat a string suitable for use with
    * SimpleDateFormatter for parsing dates.  Null for a default format
    * string.
    * @param index the attribute's index
    */
    //@ requires attributeName != null;
    //@ requires index >= 0;
    //@ ensures  m_Name == attributeName;
    //@ ensures  m_Index == index;
    Attribute(const string &attributeName, string_array &attributeValues, int index);

    /**
     * Returns the index of this attribute.
     *
     * @return the index of this attribute
     */
    int index() const;

    /**
     * Returns the index of a given attribute value. (The index of
     * the first occurence of this value.)
     *
     * @param value the value for which the index is to be returned
     * @return the index of the given attribute value if attribute
     * is nominal or a string, -1 if it is not or the value
     * can't be found
     */
    int indexOfValue(const string &value) const;

    /**
     * Test if the attribute is nominal.
     *
     * @return true if the attribute is nominal
     */
    bool isNominal() const;

    /**
     * Tests if the attribute is numeric.
     *
     * @return true if the attribute is numeric
     */
    bool isNumeric() const;

    /**
     * Tests if the attribute is relation valued.
     *
     * @return true if the attribute is relation valued
     */
    bool isRelationValued() const;

    /**
     * Tests if the attribute is a string.
     *
     * @return true if the attribute is a string
     */
    bool isString() const;

    /**
     * Tests if the attribute is a date type.
     *
     * @return true if the attribute is a date type
     */
    bool isDate() const;

    /**
     * Returns the attribute's name.
     *
     * @return the attribute's name as a string
     */
    string name() const;

    /**
     * Returns the number of attribute values. Returns 0 for
     * attributes that are not either nominal, string, or
     * relation-valued.
     *
     * @return the number of attribute values
     */
    int numValues() const;

    /**
     * Returns the attribute's type as an integer.
     *
     * @return the attribute's type.
     */
    int type() const;

    /**
     * Returns a value of a nominal or string attribute.  Returns an
     * empty string if the attribute is neither a string nor a nominal
     * attribute.
     *
     * @param valIndex the value's index
     * @return the attribute's value as a string
     */
    string value(int valIndex) const;

    /**
     * Adds a string value to the list of valid strings for attributes
     * of type STRING and returns the index of the string.
     *
     * @param value The string value to add
     * @return the index assigned to the string, or -1 if the attribute is not
     * of type Attribute.STRING
     */
     /*@ requires value != null;
         ensures  isString() && 0 <= \result && \result < m_Values.size() ||
                ! isString() && \result == -1;
     */
    int addStringValue(const string &value);

    /**
      * Adds a string value to the list of valid strings for attributes
      * of type STRING and returns the index of the string. This method is
      * more efficient than addStringValue(String) for long strings.
      *
      * @param src The Attribute containing the string value to add.
      * @param index the index of the string value in the source attribute.
      * @return the index assigned to the string, or -1 if the attribute is not
      * of type Attribute.STRING
      */
      /*@ requires src != null;
          requires 0 <= index && index < src.m_Values.size();
          ensures  isString() && 0 <= \result && \result < m_Values.size() ||
                 ! isString() && \result == -1;
      */
    virtual int addStringValue(Attribute *src, int index) const;

    /**
     * Adds an attribute value. Creates a fresh list of attribute
     * values before adding it.
     *
     * @param value the attribute value
     */
    void addValue(const string &value);

    /**
      * Produces a shallow copy of this attribute with a new name.
      *
      * @param newName the name of the new attribute
      * @return a copy of this attribute with the same index
      */
      //@ requires newName != null;
      //@ ensures \result.m_Name  == newName;
      //@ ensures \result.m_Index == m_Index;
      //@ ensures \result.m_Type  == m_Type;
    Attribute *copy(const string &newName) const;

    /**
      * Removes a value of a nominal, string, or relation-valued
      * attribute. Creates a fresh list of attribute values before
      * removing it.
      *
      * @param index the value's index
      * @throws IllegalArgumentException if the attribute is not
      * of the correct type
      */
      //@ requires isNominal() || isString() || isRelationValued();
      //@ requires 0 <= index && index < m_Values.size();
    void Delete(int index);

    /**
     * Adds an attribute value.
     *
     * @param value the attribute value
     */
     //@ requires value != null;
     //@ ensures  m_Values.size() == \old(m_Values.size()) + 1;
    void forceAddValue(const string &value);

    /**
     * Sets the index of this attribute.
     *
     * @param index the index of this attribute
     */
     //@ requires 0 <= index;
     //@ assignable m_Index;
     //@ ensures m_Index == index;
    void setIndex(int index);

    /**
     * Sets a value of a nominal attribute or string attribute.
     * Creates a fresh list of attribute values before it is set.
     *
     * @param index the value's index
     * @param string the value
     * @throws IllegalArgumentException if the attribute is not nominal or
     * string.
     */
     //@ requires string != null;
     //@ requires isNominal() || isString();
     //@ requires 0 <= index && index < m_Values.size();
    void setValue(int index, const string &string);

    /**
      * Returns the attribute's weight.
      *
      * @return the attribute's weight as a double
      */
    double weight() const;

    /**
     * Sets the new attribute's weight
     *
     * @param value    the new weight
     */
    void setWeight(double value);

};

#endif // _ATTRIBUTE_HPP_
