#include "Instances.h"
#include "Consts.h"
#include "Utils.h"

#include <iostream>
#include <unordered_set>
#include <exception>
#include <stdexcept>


Instances::Instances( const std::string &name, std::vector<Attribute*> &attInfo, int capacity )
{
    // check whether the attribute names are unique
    std::unordered_set<std::string> attr_names;
    std::string nonUniqueNames = "";
    for( auto att : attInfo )
    {
        if( std::find( attr_names.begin(), attr_names.end(), att->name() ) != attr_names.end() )
        {
            nonUniqueNames.append( std::string( "'" ) + att->name() + std::string("' ") );
        }
        attr_names.insert( att->name() );
    }
    if( attr_names.size() != attInfo.size() )
    {
        throw std::invalid_argument( std::string( "Attribute names are not unique!" ) + std::string( " Causes: " ) + nonUniqueNames );
    }
    attr_names.clear();

    mRelationName = name;
    mClassIndex = -1;
    mAttributes = attInfo;
    mNamesToAttributeIndices = std::unordered_map<std::string, int>( static_cast<int>( numAttributes() / 0.75 ) );
    for( int i = 0; i < numAttributes(); i++ )
    {
        (attribute( i ))->setIndex( i );
        mNamesToAttributeIndices[(attribute( i ))->name()] = i;
    }
    mInstances = std::vector<Instance*>( capacity );
}

Instances::Instances( Instances *dataset ):Instances( dataset, dataset->numInstances() )
{
    dataset->copyInstances( 0, this, dataset->numInstances() );
}

Instances::Instances( Instances *dataset, int capacity )
{
    initialize( dataset, capacity );
}

Attribute *Instances::attribute( int index )
{
    // @pure@

    return mAttributes[index];
}

Attribute *Instances::attribute( const std::string &name )
{
    // @pure@

    int index = mNamesToAttributeIndices[name];
    if( index != -1 )
    {
        return attribute( index );
    }

    return nullptr;
}

int Instances::numAttributes()
{
    return mAttributes.size();
}

int Instances::classIndex()
{
    return mClassIndex;
}

void Instances::setClassIndex( int classIndex )
{
    if( classIndex >= numAttributes() )
    {
        throw  std::string( "Invalid class index: " ) + std::to_string( classIndex );
    }
    mClassIndex = classIndex;
}

bool Instances::add( Instance *instance )
{
    //Instance *newInstance = static_cast<Instance*>( instance );
    mInstances.push_back( instance );

    return true;
}

void Instances::add( int index, Instance *instance )
{
    // @non_null@

    //Instance *newInstance = static_cast<Instance*>( instance );

    //newInstance->setDataset( this );
    //mInstances.insert( index, instance );
}

int Instances::numInstances()
{
    return mInstances.size();
}

void Instances::copyInstances( int from, Instances *dest, int num )
{
    // @non_null@

    for( int i = 0; i < num; i++ )
    {
        dest->add( instance( from + i ) );
    }
}

Instance *Instances::instance( int index )
{
    return mInstances[index];
}

void Instances::initialize( Instances *dataset, int capacity )
{
    if( capacity < 0 )
    {
        capacity = 0;
    }

    // Strings only have to be "shallow" copied because
    // they can't be modified.
    mClassIndex = dataset->mClassIndex;
    mRelationName = dataset->mRelationName;
    mAttributes = dataset->mAttributes;
    mNamesToAttributeIndices = dataset->mNamesToAttributeIndices;
    mInstances = std::vector<Instance*>( capacity );
}

Attribute *Instances::classAttribute()
{
    if( mClassIndex < 0 )
    {
        throw "Class index is negative (not set)!" ;
    }
    return attribute( mClassIndex );
}
int Instances::numClasses()
{
    if( mClassIndex < 0 )
    {
        throw "Class index is negative (not set)!" ;
    }
    if( !classAttribute()->isNominal() )
    {
        return 1;
    }
    else
    {
        return classAttribute()->numValues();
    }
}
double Instances::classValue()
{

    return 0.0;
}

double Instances::weight()
{
    return 0.0;
}
bool Instances::isMissing( int attIndex )
{
    return false;
}

Instance *Instances::lastInstance()
{
    return mInstances[mInstances.size() - 1];
}

void Instances::setWeight( double weight )
{

    mWeight = weight;
}

void Instances::deleteWithMissing( int attIndex )
{

    std::vector<Instance*> newInstances( numInstances() );

    for( int i = 0; i < numInstances(); i++ )
    {
        if( !instance( i )->isMissing( attIndex ) )
        {
            newInstances.push_back( instance( i ) );
        }
    }
    mInstances = newInstances;
}

void Instances::deleteWithMissing( Attribute *att )
{
    deleteWithMissing( att->index() );
}

void Instances::deleteWithMissingClass()
{

    if( mClassIndex < 0 )
    {
        throw "Class index is negative (not set)!" ;
    }
    deleteWithMissing( mClassIndex );
}

double Instances::sumOfWeights()
{
    // @pure@

    double sum = 0;

    for( int i = 0; i < numInstances(); i++ )
    {
        sum += instance( i )->weight();
    }
    return sum;
}

java::util::Iterator<Attribute*> *Instances::enumerateAttributes()
{ // @non_null pure@

  return new WekaEnumeration<Attribute*>( mAttributes, mClassIndex );
}

java::util::Iterator<Instance*> *Instances::enumerateInstances()
{ // @non_null pure@

  return new WekaEnumeration<Instance*>( mInstances );
}
