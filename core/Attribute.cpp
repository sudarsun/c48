#include "Attribute.h"
#include "Consts.h"
#include "iostream"

Attribute::Attribute( const std::string &attributeName ) : mName( attributeName)
{
	mAttributeMetaInfo = new AttributeMetaInfo(this);
}

Attribute::Attribute( const std::string &attributeName, std::vector<std::string> &attributeValues ) : mName( attributeName )
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

void *Attribute::copy()
{
	// @ pure non_null @

	return copy( mName );
}

bool Attribute::equals( void *other )
{
	// @ pure @
	return ( equalsMsg( other ) == "" );
}

std::string Attribute::typeToString( Attribute *att )
{
	return typeToString( att->type() );
}

std::string Attribute::typeToString( int type )
{
	std::string result;

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
		result = std::string( "unknown(" ) + std::to_string(type) + std::string( ")" );
	}

	return result;
}

std::string Attribute::typeToStringShort( Attribute *att )
{
	return typeToStringShort( att->type() );
}

std::string Attribute::typeToStringShort( int type )
{
	std::string result;

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
	// @ pure @

	return mIndex;
}

int Attribute::indexOfValue( const std::string &value )
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
	// @ pure @

	return ( mType == ATTRIBUTE::RELATIONAL );
}

bool Attribute::isString()
{
	// @ pure @

	return ( mType == ATTRIBUTE::STRING );
}

bool Attribute::isDate()
{
	// @ pure @

	return ( mType == ATTRIBUTE::DATE );
}

std::string Attribute::name()
{
	// @ pure @

	return mName;
}

int Attribute::numValues()
{
	// @ pure @

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
	// @ pure @

	return mType;
}

std::string Attribute::value( int valIndex )
{
	// @ non_null pure @

	if( !isNominal() && !isString() )
	{
		return "";
	}
	else
	{
		return ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getAttributeValue( valIndex );
	}
}

Attribute::Attribute( const std::string &attributeName, int index ) : Attribute( attributeName )
{

	mIndex = index;
}

Attribute::Attribute( const std::string &attributeName, std::vector<std::string> &attributeValues, int index ) : Attribute( attributeName, attributeValues )
{

	mIndex = index;
}

int Attribute::addStringValue( const std::string &value )
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

void Attribute::setStringValue( const std::string &value )
{
	if( !isString() )
	{
		return;
	}

	( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->clearAttribute();
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

void Attribute::addValue( const std::string &value )
{
	forceAddValue( value );
}

Attribute *Attribute::copy( const std::string &newName )
{
	// @ pure non_null @

	Attribute *copy = new Attribute( newName );

	copy->mIndex = mIndex;
	copy->mType = mType;
	copy->mAttributeInfo = mAttributeInfo;
	copy->mAttributeMetaInfo = mAttributeMetaInfo;

	return copy;
}

void Attribute::Delete( int index )
{

	if( !isNominal() && !isString() && !isRelationValued() )
	{
		throw std::invalid_argument( std::string( "Can only remove value of " ) + std::string( "nominal, string or relation-" ) + std::string( " valued attribute!" ) );
	}
	else
	{
		//( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->mValues = Utils::cast( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->mValues.clone() );
		NominalAttributeInfo * nominal = static_cast<NominalAttributeInfo*>( mAttributeInfo );
		nominal->removeIndex( index );
		if( !isRelationValued() )
		{
			std::unordered_map<std::string, int> _hash( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getHashTableSize() );

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

void Attribute::forceAddValue( const std::string &value )
{

	auto store = value.c_str();
	( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->addAttribute( store, int( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize() - 1 ) );
}

void Attribute::setIndex( int index )
{

	mIndex = index;
}

void Attribute::setValue( int index, const std::string &stringName )
{
	NominalAttributeInfo* nominal = static_cast<NominalAttributeInfo*>( mAttributeInfo ) ;
	switch( mType )
	{
	case ATTRIBUTE::NOMINAL:
	case ATTRIBUTE::STRING:
		{
			//nominal->mValues = Utils::cast( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->mValues.clone() );
			//nominal->mHashtable = Utils::cast( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->mHashtable.clone() );
			nominal->removeHashTable( nominal->getAttributeValue( index ) );
			nominal->addAttribute( stringName.c_str(), int( index ), true );
			break;
		}
	default:
		throw std::invalid_argument( std::string( "Can only set values for nominal" ) + std::string( " or string attributes!" ) );
	}
}

double Attribute::weight()
{
	// @ pure @

	return mWeight;
}

void Attribute::setWeight( double value )
{

	mWeight = value;
}

std::string Attribute::equalsMsg( void *other ) {
	if( other == nullptr ) {
		return "Comparing with null object";
	}

	Attribute *att = static_cast<Attribute*>( other );
	if( mName != att->mName ) {
		return std::string( "Names differ: " ) + mName + std::string( " != " ) + att->mName;
	}

	if( isNominal() && att->isNominal() ) {
		if( ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize() != (static_cast<NominalAttributeInfo*>(att->mAttributeInfo))->getSize() ) {
			return std::string( "Different number of labels: " ) + std::to_string(( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize()) + std::string(" != ") + std::to_string((static_cast<NominalAttributeInfo*>(att->mAttributeInfo))->getSize());
		}

		for( int i = 0; i < (int) ( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getSize(); i++ ) {
			if( (static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getAttributeValue( i )  !=  ( static_cast<NominalAttributeInfo*>( att->mAttributeInfo ) )->getAttributeValue( i ) )  {
				return std::string( "Labels differ at position " ) + std::to_string( i + 1 ) + std::string( ": " ) + /*static_cast<std::string>(( static_cast<NominalAttributeInfo*>( mAttributeInfo ) )->getAttributeValue( i )) +*/ std::string( " != " ) /*+ static_cast<std::string>(( static_cast<NominalAttributeInfo*>( att->mAttributeInfo ) )->getAttributeValue( i ))*/;
			}
		}

		return "";
	}

	if( ( type() != att->type() ) ) {
		return std::string( "Types differ: " ) + typeToString( this ) + std::string( " != " ) + typeToString( att );
	}

	return "";
}
