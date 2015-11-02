#include "AttributeMetaInfo.h"
#include "Attribute.h"
#include "Consts.h"
#include <exception>


AttributeMetaInfo::AttributeMetaInfo( Attribute *att )
{
    mOrdering = 0;
    mIsRegular = false;
    mIsAveragable = false;
    mHasZeropoint = false;
    mLowerBound = 0;
    mLowerBoundIsOpen = false;
    mUpperBound = 0;
    mUpperBoundIsOpen = false;
    setMetadata( att );
}

void AttributeMetaInfo::setMetadata( Attribute *att )
{

    if( att->mType == ATTRIBUTE::DATE )
    {
        mOrdering = ATTRIBUTE_ORDERING::ORDERING_ORDERED;
        mIsRegular = true;
        mIsAveragable = false;
        mHasZeropoint = false;
    }
    else
    {

        // get ordering
        std::string orderString = "";

        // numeric ordered attributes are averagable and zeropoint by default
        std::string def;
        if( att->mType == ATTRIBUTE::NUMERIC && orderString.compare( "modulo" ) != 0 && orderString.compare( "symbolic" ) != 0 )
        {
            def = "true";
        }
        else
        {
            def = "false";
        }

        // determine boolean states
        mIsAveragable = 0;
        mHasZeropoint = 0;
        // averagable or zeropoint implies regular
        if( mIsAveragable || mHasZeropoint )
        {
            def = "true";
        }
        mIsRegular = 0;

        // determine ordering
        if( orderString.compare( "symbolic" ) == 0 )
        {
            mOrdering = ATTRIBUTE_ORDERING::ORDERING_SYMBOLIC;
        }
        else if( orderString.compare( "ordered" ) == 0 )
        {
            mOrdering = ATTRIBUTE_ORDERING::ORDERING_ORDERED;
        }
        else if( orderString.compare( "modulo" ) == 0 )
        {
            mOrdering = ATTRIBUTE_ORDERING::ORDERING_MODULO;
        }
        else
        {
            if( att->mType == ATTRIBUTE::NUMERIC || mIsAveragable || mHasZeropoint )
            {
                mOrdering = ATTRIBUTE_ORDERING::ORDERING_ORDERED;
            }
            else
            {
                mOrdering = ATTRIBUTE_ORDERING::ORDERING_SYMBOLIC;
            }
        }
    }



    // determine weight
    att->mWeight = 1.0;

    // determine numeric range
    if( att->mType == ATTRIBUTE::NUMERIC )
    {
        setNumericRange( "" );
    }
}

void AttributeMetaInfo::setNumericRange( const std::string &rangeString )
{
    // set defaults
    mLowerBound = -std::numeric_limits<double>::infinity();
    mLowerBoundIsOpen = false;
    mUpperBound = std::numeric_limits<double>::infinity();
    mUpperBoundIsOpen = false;

    if( rangeString == "" )
    {
        return;
    }
}

