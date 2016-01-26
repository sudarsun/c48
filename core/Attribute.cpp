#include "Attribute.h"
#include "Consts.h"
#include "iostream"

Attribute::Attribute(const string &attributeName) : mName(attributeName)
{
    //
}

Attribute::Attribute(const string &attributeName, string_array &attributeValues) : mName(attributeName)
{

    mAttributeInfo = new NominalAttributeInfo(attributeValues, attributeName);

    if (attributeValues.empty())
    {
        mType = ATTRIBUTE::STRING;
    }
    else
    {
        mType = ATTRIBUTE::NOMINAL;
    }
}

Attribute::Attribute(const string &attributeName, int index) : Attribute(attributeName)
{
    mIndex = index;
}

Attribute::Attribute(const string &attributeName, string_array &attributeValues, int index) : Attribute(attributeName, attributeValues)
{
    mIndex = index;
}

int Attribute::index() const
{
    return mIndex;
}

int Attribute::indexOfValue(const string &value) const
{
    if (!isNominal() && !isString())
    {
        return -1;
    }
    try
    {
        return (static_cast<NominalAttributeInfo*>(mAttributeInfo))->getAttributeIndex(value);
    }
    catch (std::exception &ex)
    {
        return -1;
    }
}

bool Attribute::isNominal() const
{
    return (mType == ATTRIBUTE::NOMINAL);
}

bool Attribute::isNumeric() const
{
    return ((mType == ATTRIBUTE::NUMERIC) || (mType == ATTRIBUTE::DATE));
}

bool Attribute::isRelationValued() const
{
    return (mType == ATTRIBUTE::RELATIONAL);
}

bool Attribute::isString() const
{
    return (mType == ATTRIBUTE::STRING);
}

bool Attribute::isDate() const
{
    return (mType == ATTRIBUTE::DATE);
}

string Attribute::name() const
{
    return mName;
}

int Attribute::numValues() const
{

    if (!isNominal() && !isString() && !isRelationValued())
    {
        return 0;
    }
    else
    {
        return (int)(static_cast<NominalAttributeInfo*>(mAttributeInfo))->getSize();
    }
}

int Attribute::type() const
{
    return mType;
}

string Attribute::value(int valIndex) const
{

    if (!isNominal() && !isString())
    {
        return "";
    }
    else
    {
        return (static_cast<NominalAttributeInfo*>(mAttributeInfo))->getAttributeValue(valIndex);
    }
}

int Attribute::addStringValue(const string &value)
{
    if (!isString())
    {
        return -1;
    }
    NominalAttributeInfo* nominalPtr = static_cast<NominalAttributeInfo*>(mAttributeInfo);
    int intIndex = -1;
    try
    {
        return nominalPtr->getAttributeIndex(value.c_str());
    }
    catch (std::exception &ex)
    {
        intIndex = (int)nominalPtr->getSize();
        nominalPtr->addAttribute(value.c_str(), intIndex);
        return intIndex;
    }
}

//void Attribute::setStringValue(const string &value)
//{
//    if (!isString())
//    {
//        return;
//    }
//
//    (static_cast<NominalAttributeInfo*>(mAttributeInfo))->clearAttribute();
//    if (value != "")
//    {
//        addStringValue(value);
//    }
//}

int Attribute::addStringValue(Attribute *src, int index) const
{
    if (!isString())
    {
        return -1;
    }
    auto store = (static_cast<NominalAttributeInfo*>(src->mAttributeInfo))->getAttributeValue(index);
    try
    {
        return (static_cast<NominalAttributeInfo*>(mAttributeInfo))->getAttributeIndex(store);
    }
    catch (std::exception &ex)
    {
        int intIndex = (int)(static_cast<NominalAttributeInfo*>(mAttributeInfo))->getSize();
        (static_cast<NominalAttributeInfo*>(mAttributeInfo))->addAttribute(store, intIndex);
        return intIndex;
    }

}

void Attribute::addValue(const string &value)
{
    forceAddValue(value);
}

Attribute *Attribute::copy(const string &newName) const
{

    Attribute *copy = new Attribute(newName);

    copy->mIndex = mIndex;
    copy->mType = mType;
    copy->mAttributeInfo = mAttributeInfo;

    return copy;
}

void Attribute::Delete(int index)
{

    if (!isNominal() && !isString() && !isRelationValued())
    {
        throw std::invalid_argument(string("Can only remove value of ") + string("nominal, string or relation-") + string(" valued attribute!"));
    }
    else
    {
        NominalAttributeInfo * nominal = static_cast<NominalAttributeInfo*>(mAttributeInfo);
        nominal->removeIndex(index);
        if (!isRelationValued())
        {
            std::unordered_map<string, int> _hash((static_cast<NominalAttributeInfo*>(mAttributeInfo))->getHashTableSize());

            for (auto key : nominal->getHashTable())
            {
                int valIndexObject = nominal->getAttributeIndex(key.first);
                int valIndex = valIndexObject;
                if (valIndex > index)
                {
                    _hash[key.first] = int(valIndex - 1);
                }
                else if (valIndex < index)
                {
                    _hash[key.first] = valIndexObject;
                }
            }
            nominal->setHashTable(_hash);
        }
    }
}

void Attribute::forceAddValue(const string &value)
{
    auto store = value.c_str();
    (static_cast<NominalAttributeInfo*>(mAttributeInfo))->addAttribute(store, int((static_cast<NominalAttributeInfo*>(mAttributeInfo))->getSize() - 1));
}

void Attribute::setIndex(int index)
{
    mIndex = index;
}

void Attribute::setValue(int index, const string &stringName)
{
    NominalAttributeInfo* nominal = static_cast<NominalAttributeInfo*>(mAttributeInfo);
    switch (mType)
    {
    case ATTRIBUTE::NOMINAL:
    case ATTRIBUTE::STRING:
    {
        nominal->removeHashTable(nominal->getAttributeValue(index));
        nominal->addAttribute(stringName.c_str(), int(index), true);
        break;
    }
    default:
        throw std::invalid_argument(string("Can only set values for nominal") + string(" or string attributes!"));
    }
}

double Attribute::weight() const
{
    return mWeight;
}

void Attribute::setWeight(double value)
{
    mWeight = value;
}
