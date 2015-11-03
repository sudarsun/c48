#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include "AttributeInfo.h"
#include "AttributeMetaInfo.h"
#include "NominalAttributeInfo.h"
#include <vector>

class Attribute
{

public:

	const std::string mName;
	int mType = 0;
	int mIndex = -1;
	double mWeight = 1.0;
	AttributeInfo *mAttributeInfo;
	AttributeMetaInfo *mAttributeMetaInfo;

	Attribute(const std::string &attributeName);
	Attribute(const std::string &attributeName, std::vector<std::string> &attributeValues);

	void *copy();
	bool equals(void *other);
	int hashCode();

	/// <summary>
	/// Tests if given attribute is equal to this attribute. If they're not the
	/// same a message detailing why they differ will be returned, otherwise null.
	/// </summary>
	/// <param name="other"> the Object to be compared to this attribute </param>
	/// <returns> null if the given attribute is equal to this attribute </returns>
	std::string equalsMsg(void *other);

	/// <summary>
	/// Returns a string representation of the attribute type.
	/// </summary>
	/// <param name="att"> the attribute to return the type string for </param>
	/// <returns> the string representation of the attribute type </returns>
	static std::string typeToString(Attribute *att);

	/// <summary>
	/// Returns a string representation of the attribute type.
	/// </summary>
	/// <param name="type"> the type of the attribute </param>
	/// <returns> the string representation of the attribute type </returns>
	static std::string typeToString(int type);

	/// <summary>
	/// Returns a short string representation of the attribute type.
	/// </summary>
	/// <param name="att"> the attribute to return the type string for </param>
	/// <returns> the string representation of the attribute type </returns>
	static std::string typeToStringShort(Attribute *att);

	/// <summary>
	/// Returns a short string representation of the attribute type.
	/// </summary>
	/// <param name="type"> the type of the attribute </param>
	/// <returns> the string representation of the attribute type </returns>
	static std::string typeToStringShort(int type);
	/// <summary>
	/// Returns the index of this attribute.
	/// </summary>
	/// <returns> the index of this attribute </returns>
	// @ ensures \result == mIndex;
	int index();

	/// <summary>
	/// Returns the index of a given attribute value. (The index of the first
	/// occurence of this value.)
	/// </summary>
	/// <param name="value"> the value for which the index is to be returned </param>
	/// <returns> the index of the given attribute value if attribute is nominal or a
	///         string, -1 if it is not or the value can't be found </returns>
	int indexOfValue(const std::string &value);

	/// <summary>
	/// Test if the attribute is nominal.
	/// </summary>
	/// <returns> true if the attribute is nominal </returns>
	// @ ensures \result <==> (mType == NOMINAL);
	bool isNominal();

	/// <summary>
	/// Tests if the attribute is numeric.
	/// </summary>
	/// <returns> true if the attribute is numeric </returns>
	// @ ensures \result <==> ((mType == NUMERIC) || (mType == DATE));
	bool isNumeric();

	/// <summary>
	/// Tests if the attribute is relation valued.
	/// </summary>
	/// <returns> true if the attribute is relation valued </returns>
	// @ ensures \result <==> (mType == RELATIONAL);
	bool isRelationValued();

	/// <summary>
	/// Tests if the attribute is a string.
	/// </summary>
	/// <returns> true if the attribute is a string </returns>
	// @ ensures \result <==> (mType == STRING);
	bool isString();

	/// <summary>
	/// Tests if the attribute is a date type.
	/// </summary>
	/// <returns> true if the attribute is a date type </returns>
	// @ ensures \result <==> (mType == DATE);
	bool isDate();

	/// <summary>
	/// Returns the attribute's name.
	/// </summary>
	/// <returns> the attribute's name as a string </returns>
	// @ ensures \result == mName;
	std::string name();

	/// <summary>
	/// Returns the number of attribute values. Returns 0 for attributes that are
	/// not either nominal, string, or relation-valued.
	/// </summary>
	/// <returns> the number of attribute values </returns>
	int numValues();

	/// <summary>
	/// Returns a description of this attribute in ARFF format. Quotes strings if
	/// they contain whitespace characters, or if they are a question mark.
	/// </summary>
	/// <returns> a description of this attribute as a string </returns>
	//virtual std::string toString();

	/// <summary>
	/// Returns the attribute's type as an integer.
	/// </summary>
	/// <returns> the attribute's type. </returns>
	// @ ensures \result == mType;
	int type();

	/// <summary>
	/// Returns the Date format pattern in case this attribute is of type DATE,
	/// otherwise an empty string.
	/// </summary>
	/// <returns> the date format pattern </returns>
	/// <seealso cref= java.text.SimpleDateFormat </seealso>
	std::string getDateFormat();
	/// <summary>
	/// Returns a value of a nominal or string attribute. Returns an empty string
	/// if the attribute is neither a string nor a nominal attribute.
	/// </summary>
	/// <param name="valIndex"> the value's index </param>
	/// <returns> the attribute's value as a string </returns>

	std::string value(int valIndex);
	/// <summary>
	/// Constructor for a numeric attribute with a particular index.
	/// </summary>
	/// <param name="attributeName"> the name for the attribute </param>
	/// <param name="index"> the attribute's index </param>
	// @ requires attributeName != null;
	// @ requires index >= 0;
	// @ ensures mName == attributeName;
	// @ ensures mIndex == index;
	Attribute(const std::string &attributeName, int index);

	/// <summary>
	/// Constructor for date attributes with a particular index.
	/// </summary>
	/// <param name="attributeName"> the name for the attribute </param>
	/// <param name="dateFormat"> a string suitable for use with SimpleDateFormatter for
	///          parsing dates. Null for a default format string. </param>
	/// <param name="index"> the attribute's index </param>
	// @ requires attributeName != null;
	// @ requires index >= 0;
	// @ ensures mName == attributeName;
	// @ ensures mIndex == index;
	Attribute(const std::string &attributeName, const std::string &dateFormat, int index);

	/// <summary>
	/// Constructor for nominal attributes and string attributes with a particular
	/// index. If a null vector of attribute values is passed to the method, the
	/// attribute is assumed to be a string.
	/// </summary>
	/// <param name="attributeName"> the name for the attribute </param>
	/// <param name="attributeValues"> a vector of strings denoting the attribute values.
	///          Null if the attribute is a string attribute. </param>
	/// <param name="index"> the attribute's index </param>
	// @ requires attributeName != null;
	// @ requires index >= 0;
	// @ ensures mName == attributeName;
	// @ ensures mIndex == index;
	Attribute(const std::string &attributeName, std::vector<std::string> &attributeValues, int index);

	/// <summary>
	/// Adds a string value to the list of valid strings for attributes of type
	/// STRING and returns the index of the string.
	/// </summary>
	/// <param name="value"> The string value to add </param>
	/// <returns> the index assigned to the string, or -1 if the attribute is not of
	///         type Attribute.STRING </returns>
	/*
	 * @ requires value != null; ensures isString() && 0 <= \result && \result <
	 * mValues.size() || ! isString() && \result == -1;
	 */
	virtual int addStringValue(const std::string &value);

	/// <summary>
	/// Clear the map and list of values and set them to contain just the supplied
	/// value
	/// </summary>
	/// <param name="value"> the current (and only) value of this String attribute. If null
	/// then just the map is cleared. </param>
	virtual void setStringValue(const std::string &value);

	/// <summary>
	/// Adds a string value to the list of valid strings for attributes of type
	/// STRING and returns the index of the string. This method is more efficient
	/// than addStringValue(String) for long strings.
	/// </summary>
	/// <param name="src"> The Attribute containing the string value to add. </param>
	/// <param name="index"> the index of the string value in the source attribute. </param>
	/// <returns> the index assigned to the string, or -1 if the attribute is not of
	///         type Attribute.STRING </returns>
	/*
	 * @ requires src != null; requires 0 <= index && index < src.mValues.size();
	 * ensures isString() && 0 <= \result && \result < mValues.size() || !
	 * isString() && \result == -1;
	 */
	virtual int addStringValue(Attribute *src, int index);

	/// <summary>
	/// Adds an attribute value. Creates a fresh list of attribute values before
	/// adding it.
	/// </summary>
	/// <param name="value"> the attribute value </param>
	void addValue(const std::string &value);

	/// <summary>
	/// Produces a shallow copy of this attribute with a new name.
	/// </summary>
	/// <param name="newName"> the name of the new attribute </param>
	/// <returns> a copy of this attribute with the same index </returns>
	// @ requires newName != null;
	// @ ensures \result.mName == newName;
	// @ ensures \result.mIndex == mIndex;
	// @ ensures \result.mType == mType;
	Attribute *copy(const std::string &newName);

	/// <summary>
	/// Removes a value of a nominal, string, or relation-valued attribute. Creates
	/// a fresh list of attribute values before removing it.
	/// </summary>
	/// <param name="index"> the value's index </param>
	/// <exception cref="IllegalArgumentException"> if the attribute is not of the correct
	///           type </exception>
	// @ requires isNominal() || isString() || isRelationValued();
	// @ requires 0 <= index && index < mValues.size();
	void Delete(int index);

	/// <summary>
	/// Adds an attribute value.
	/// </summary>
	/// <param name="value"> the attribute value </param>
	// @ requires value != null;
	// @ ensures mValues.size() == \old(mValues.size()) + 1;
	void forceAddValue(const std::string &value);

	/// <summary>
	/// Sets the index of this attribute.
	/// </summary>
	/// <param name="index"> the index of this attribute </param>
	// @ requires 0 <= index;
	// @ assignable mIndex;
	// @ ensures mIndex == index;
	void setIndex(int index);

	/// <summary>
	/// Sets a value of a nominal attribute or string attribute. Creates a fresh
	/// list of attribute values before it is set.
	/// </summary>
	/// <param name="index"> the value's index </param>
	/// <param name="string"> the value </param>
	/// <exception cref="IllegalArgumentException"> if the attribute is not nominal or string. </exception>
	// @ requires string != null;
	// @ requires isNominal() || isString();
	// @ requires 0 <= index && index < mValues.size();
	void setValue(int index, const std::string &string);

	/// <summary>
	/// Returns the ordering of the attribute. One of the following:
	///
	/// ORDERING_SYMBOLIC - attribute values should be treated as symbols.
	/// ORDERING_ORDERED - attribute values have a global ordering. ORDERING_MODULO
	/// - attribute values have an ordering which wraps.
	/// </summary>
	/// <returns> the ordering type of the attribute </returns>
	int ordering();

	/// <summary>
	/// Returns whether the attribute values are equally spaced.
	/// </summary>
	/// <returns> whether the attribute is regular or not </returns>
	bool isRegular();

	/// <summary>
	/// Returns whether the attribute can be averaged meaningfully.
	/// </summary>
	/// <returns> whether the attribute can be averaged or not </returns>
	bool isAveragable();

	/// <summary>
	/// Returns whether the attribute has a zeropoint and may be added
	/// meaningfully.
	/// </summary>
	/// <returns> whether the attribute has a zeropoint or not </returns>
	bool hasZeropoint();

	/// <summary>
	/// Returns the attribute's weight.
	/// </summary>
	/// <returns> the attribute's weight as a double </returns>
	double weight();

	/// <summary>
	/// Sets the new attribute's weight. Does not modify the weight info stored in the
	/// attribute's meta data object!
	/// </summary>
	/// <param name="value"> the new weight </param>
	virtual void setWeight(double value);

	/// <summary>
	/// Returns the lower bound of a numeric attribute.
	/// </summary>
	/// <returns> the lower bound of the specified numeric range </returns>
	double getLowerNumericBound();

	/// <summary>
	/// Returns whether the lower numeric bound of the attribute is open.
	/// </summary>
	/// <returns> whether the lower numeric bound is open or not (closed) </returns>
	bool lowerNumericBoundIsOpen();

	/// <summary>
	/// Returns the upper bound of a numeric attribute.
	/// </summary>
	/// <returns> the upper bound of the specified numeric range </returns>
	double getUpperNumericBound();

	/// <summary>
	/// Returns whether the upper numeric bound of the attribute is open.
	/// </summary>
	/// <returns> whether the upper numeric bound is open or not (closed) </returns>
	bool upperNumericBoundIsOpen();

	/// <summary>
	/// Determines whether a value lies within the bounds of the attribute.
	/// </summary>
	/// <param name="value"> the value to check </param>
	/// <returns> whether the value is in range </returns>
	bool isInRange(double value);

};

#endif // _ATTRIBUTE_HPP
