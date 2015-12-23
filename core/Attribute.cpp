#include "Attribute.h"
#include "Consts.h"
#include "iostream"

Attribute::Attribute( const string &attributeName ) : mName( attributeName)
{
	//
}

Attribute::Attribute( const string &attributeName, string_array &attributeValues ) : mName( attributeName )
{

	mAttributeInfo = new NominalAttributeInfo( attributeValues, attributeName );
	//std::cout << (static_cast<NominalAttributeInfo*>(mAttributeInfo))->getSize() << std::endl;
	if( attributeValues.empty() )
	{
		mType = ATTRIBUTE::STRING;
	}
	else
	{
		mType = ATTRIBUTE::NOMINAL;
	}
}

string Attribute::typeToString( Attribute *att )
{
	return typeToString( att->type() );
}

string Attribute::typeToString( int type )
{
	string result;

	switch( type )
	{
	case ATTRIBUTE::NUMERIC:
		result = "numeric";
		break;

	case ATTRIBUTE::NOMINAL:
		result = "nominal";
		break;

	case ATTRIBUTE::STRING:
		result = "string";
		break;

	case ATTRIBUTE::DATE:
		result = "date";
		break;

	case ATTRIBUTE::RELATIONAL:
		result = "relational";
		break;

	default:
		result = string( "unknown(" ) + std::to_string(type) + string( ")" );
	}

	return result;
}

string Attribute::typeToStringShort( Attribute *att )
{
	return typeToStringShort( att->type() );
}

string Attribute::typeToStringShort( int type )
{
	string result;

	switch( type )
	{
	case ATTRIBUTE::NUMERIC:
		result = "Num";
		break;

	case ATTRIBUTE::NOMINAL:
		result = "Nom";
		break;

	case ATTRIBUTE::STRING:
		result = "Str";
		break;

	case ATTRIBUTE::DATE:
		result = "Dat";
		break;

	case ATTRIBUTE::RELATIONAL:
		result = "Rel";
		break;

	default:
		result = "???";
	}

	return result;
}

int Attribute::index()
{

	return mIndex;
}

int Attribute::indexOfValue( const string &value )
{
	if( !isNominal() && !isString() )
	{
		return -1;
	}
	try
	{
		return (static_cast<NominalAttributeInfo*>( mAttributeInfo ))->getAttributeIndex(value);
	}
	catch (std::exception &ex)
	{
		return -1;
	}
}

bool Attribute::isNominal()
{
	return ( mType == ATTRIBUTE::NOMINAL );
}

bool Attribute::isNumeric()
{
	return ( ( mType == ATTRIBUTE::NUMERIC ) || ( mType == ATTRIBUTE::DATE ) );
}

bool Attribute::isRelationValued()
{
	return ( mType == ATTRIBUTE::RELATIONAL );
}

bool Attribute::isString()
{
	return ( mType == ATTRIBUTE::STRING );
}

bool Attribute::isDate()
{
	return ( mType == ATTRIBUTE::DATE );
}

string Attribute::name()
{
	return mName;
}

int Attribute::numValues()
{

	if( !isNominal() && !isString() && !isRelationValued() )
	{
		return 0;
	}
	else
	{
		return (int)( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize();
	}
}

int Attribute::type()
{
	return mType;
}

string Attribute::value( int valIndex )
{

	if( !isNominal() && !isString() )
	{
		return "";
	}
	else
	{
		return ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getAttributeValue( valIndex );
	}
}

Attribute::Attribute( const string &attributeName, int index ) : Attribute( attributeName )
{

	mIndex = index;
}

Attribute::Attribute( const string &attributeName, string_array &attributeValues, int index ) : Attribute( attributeName, attributeValues )
{

	mIndex = index;
}

int Attribute::addStringValue( const string &value )
{
	if( !isString() )
	{
		return -1;
	}
	NominalAttributeInfo* nominalPtr = static_cast<NominalAttributeInfo*>( mAttributeInfo );
	int intIndex  = -1;
	try
	{
		return nominalPtr->getAttributeIndex( value.c_str() );
	}
	catch(std::exception &ex)
	{
		intIndex = (int)nominalPtr->getSize();
		nominalPtr->addAttribute(value.c_str(),intIndex);
		return intIndex;
	}
}

void Attribute::setStringValue( const string &value )
{
	if( !isString() )
	{
		return;
	}

	(static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->clearAttribute();
	if( value != "" )
	{
		addStringValue( value );
	}
}

int Attribute::addStringValue( Attribute *src, int index )
{
	if( !isString() )
	{
		return -1;
	}
	auto store = ( static_cast<NominalAttributeInfo*>( src->mAttributeInfo ) )->getAttributeValue( index );
	try
	{
		return ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getAttributeIndex( store );
	}
	catch(std::exception &ex)
	{
		int intIndex = (int)( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize();
		( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->addAttribute(store,intIndex);
		return intIndex;
	}

}

void Attribute::addValue( const string &value )
{
	forceAddValue( value );
}

Attribute *Attribute::copy( const string &newName )
{

	Attribute *copy = new Attribute( newName );

	copy->mIndex = mIndex;
	copy->mType = mType;
	copy->mAttributeInfo = mAttributeInfo;
	//copy->mAttributeMetaInfo = mAttributeMetaInfo;

	return copy;
}

void Attribute::Delete( int index )
{

	if( !isNominal() && !isString() && !isRelationValued() )
	{
		throw std::invalid_argument( string( "Can only remove value of " ) + string( "nominal, string or relation-" ) + string( " valued attribute!" ) );
	}
	else
	{
		NominalAttributeInfo * nominal = static_cast<NominalAttributeInfo*>( mAttributeInfo );
		nominal->removeIndex( index );
		if( !isRelationValued() )
		{
			std::unordered_map<string, int> _hash( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getHashTableSize() );

			for(auto key : nominal->getHashTable())
			{
				int valIndexObject = nominal->getAttributeIndex(key.first );
				int valIndex = valIndexObject;
				if( valIndex > index )
				{
					_hash[key.first] = int( valIndex - 1 );
				}
				else if( valIndex < index )
				{
					_hash[key.first] = valIndexObject;
				}
			}
			nominal->setHashTable(_hash);
		}
	}
}

void Attribute::forceAddValue( const string &value )
{
	auto store = value.c_str();
	( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->addAttribute( store, int( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize() - 1 ) );
}

void Attribute::setIndex( int index )
{
	mIndex = index;
}

void Attribute::setValue( int index, const string &stringName )
{
	NominalAttributeInfo* nominal = static_cast<NominalAttributeInfo*>( mAttributeInfo ) ;
	switch( mType )
	{
	case ATTRIBUTE::NOMINAL:
	case ATTRIBUTE::STRING:
		{
			nominal->removeHashTable( nominal->getAttributeValue( index ) );
			nominal->addAttribute( stringName.c_str(), int( index ), true );
			break;
		}
	default:
		throw std::invalid_argument( string( "Can only set values for nominal" ) + string( " or string attributes!" ) );
	}
}

double Attribute::weight()
{
	return mWeight;
}

void Attribute::setWeight( double value )
{
	mWeight = value;
}


