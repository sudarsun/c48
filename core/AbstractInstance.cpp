#include "AbstractInstance.h"
#include "Instances.h"
#include "Attribute.h"
#include "StringBuffer.h"

namespace weka {
    namespace core {

int AbstractInstance::s_numericAfterDecimalPoint = 6;

        Attribute *AbstractInstance::attribute( int index )
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->attribute( index );
        }

        Attribute *AbstractInstance::attributeSparse( int indexOfIndex )
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->attribute( index( indexOfIndex ) );
        }

        Attribute *AbstractInstance::classAttribute()
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->classAttribute();
        }

        int AbstractInstance::classIndex()
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->classIndex();
        }

        bool AbstractInstance::classIsMissing()
        { // @pure@

          int classIndex = classIndex();
          if( classIndex < 0 ) {
            throw UnassignedClassException( L"Class is not set!" );
          }
          return isMissing( classIndex );
        }

        double AbstractInstance::classValue()
        { // @pure@

          int classIndex = classIndex();
          if( classIndex < 0 ) {
            throw UnassignedClassException( L"Class is not set!" );
          }
          return value( classIndex );
        }

        Instances *AbstractInstance::dataset()
        { // @pure@

          return m_Dataset;
        }

        void AbstractInstance::deleteAttributeAt( int position ) {

          if( m_Dataset != nullptr ) {
            throw std::exception( L"Instance has access to a dataset!" );
          }
          forceDeleteAttributeAt( position );
        }

        java::util::Iterator<Attribute*> *AbstractInstance::enumerateAttributes()
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->enumerateAttributes();
        }

        bool AbstractInstance::equalHeaders( Instance *inst )
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->equalHeaders( inst->dataset() );
        }

        std::wstring AbstractInstance::equalHeadersMsg( Instance *inst ) {
          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }

          return m_Dataset->equalHeadersMsg( inst->dataset() );
        }

        bool AbstractInstance::hasMissingValue()
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          int classIndex = classIndex();
          for( int i = 0; i < numValues(); i++ ) {
            if( index( i ) != classIndex ) {
              if( isMissingSparse( i ) ) {
                return true;
              }
            }
          }
          return false;
        }

        void AbstractInstance::insertAttributeAt( int position ) {

          if( m_Dataset != nullptr ) {
            throw std::exception( L"Instance has accesss to a dataset!" );
          }
          if( ( position < 0 ) || ( position > numAttributes() ) ) {
            throw IllegalArgumentException( std::wstring( L"Can't insert attribute: index out " ) + std::wstring( L"of range" ) );
          }
          forceInsertAttributeAt( position );
        }

        bool AbstractInstance::isMissing( int attIndex )
        { // @pure@

          if( Utils::isMissingValue( value( attIndex ) ) ) {
            return true;
          }
          return false;
        }

        bool AbstractInstance::isMissingSparse( int indexOfIndex )
        { // @pure@

          if( Utils::isMissingValue( valueSparse( indexOfIndex ) ) ) {
            return true;
          }
          return false;
        }

        bool AbstractInstance::isMissing( Attribute *att )
        { // @pure@

          return isMissing( att->index() );
        }

        int AbstractInstance::numClasses()
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return m_Dataset->numClasses();
        }

        void AbstractInstance::setClassMissing() {

          int classIndex = classIndex();
          if( classIndex < 0 ) {
            throw UnassignedClassException( L"Class is not set!" );
          }
          setMissing( classIndex );
        }

        void AbstractInstance::setClassValue( double value ) {

          int classIndex = classIndex();
          if( classIndex < 0 ) {
            throw UnassignedClassException( L"Class is not set!" );
          }
          setValue( classIndex, value );
        }

        void AbstractInstance::setClassValue( const std::wstring &value ) {

          int classIndex = classIndex();
          if( classIndex < 0 ) {
            throw UnassignedClassException( L"Class is not set!" );
          }
          setValue( classIndex, value );
        }

        void AbstractInstance::setDataset( Instances *instances ) {

          m_Dataset = instances;
        }

        void AbstractInstance::setMissing( int attIndex ) {

          setValue( attIndex, Utils::missingValue() );
        }

        void AbstractInstance::setMissing( Attribute *att ) {

          setMissing( att->index() );
        }

        void AbstractInstance::setValue( int attIndex, const std::wstring &value ) {

          int valIndex;

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          if( !attribute( attIndex )->isNominal() && !attribute(attIndex)->isString() ) {
            throw IllegalArgumentException( L"Attribute neither nominal nor string!" );
          }
          valIndex = attribute( attIndex )->indexOfValue( value );
          if( valIndex == -1 ) {
            if( attribute( attIndex )->isNominal() ) {
              throw IllegalArgumentException( L"Value not defined for given nominal attribute!" );
            } else {
              attribute( attIndex )->forceAddValue( value );
              valIndex = attribute( attIndex )->indexOfValue( value );
            }
          }
          setValue( attIndex, valIndex );
        }

        void AbstractInstance::setValue( Attribute *att, double value ) {

          setValue( att->index(), value );
        }

        void AbstractInstance::setValue( Attribute *att, const std::wstring &value ) {

          if( !att->isNominal() && !att->isString() ) {
            throw IllegalArgumentException( L"Attribute neither nominal nor string!" );
          }
          int valIndex = att->indexOfValue( value );
          if( valIndex == -1 ) {
            if( att->isNominal() ) {
              throw IllegalArgumentException( L"Value not defined for given nominal attribute!" );
            } else {
              att->forceAddValue( value );
              valIndex = att->indexOfValue( value );
            }
          }
          setValue( att->index(), valIndex );
        }

        void AbstractInstance::setWeight( double weight ) {

          m_Weight = weight;
        }

        Instances *AbstractInstance::relationalValue( int attIndex )
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return relationalValue( m_Dataset->attribute( attIndex ) );
        }

        Instances *AbstractInstance::relationalValue( Attribute *att )
        { // @pure@

          int attIndex = att->index();
          if( att->isRelationValued() ) {
            if( isMissing( attIndex ) ) {
              return nullptr;
            }
            return att->relation( static_cast<int>( value( attIndex ) ) );
          } else {
            throw IllegalArgumentException( L"Attribute isn't relation-valued!" );
          }
        }

        std::wstring AbstractInstance::stringValue( int attIndex )
        { // @pure@

          if( m_Dataset == nullptr ) {
            throw UnassignedDatasetException( L"Instance doesn't have access to a dataset!" );
          }
          return stringValue( m_Dataset->attribute( attIndex ) );
        }

        std::wstring AbstractInstance::stringValue( Attribute *att )
        { // @pure@

          int attIndex = att->index();
          if( isMissing( attIndex ) ) {
            return L"?";
          }
          switch( att->type() ) {
          case Attribute::NOMINAL:
          case Attribute::STRING:
            return att->value( static_cast<int>( value( attIndex ) ) );
          case Attribute::DATE:
            return att->formatDate( value( attIndex ) );
          case Attribute::RELATIONAL:
            return att->relation( static_cast<int>( value( attIndex ) ) ).stringWithoutHeader();
          default:
            throw IllegalArgumentException( L"Attribute isn't nominal, string or date!" );
          }
        }

        std::wstring AbstractInstance::toStringMaxDecimalDigits( int afterDecimalPoint ) {
          StringBuffer *text = new StringBuffer( toStringNoWeight( afterDecimalPoint ) );

          if( m_Weight != 1.0 ) {
            text->append( std::wstring( L",{" ) + Utils::doubleToString( m_Weight, afterDecimalPoint ) + std::wstring( L"}" ) );
          }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
          return text->toString();
        }

        std::wstring AbstractInstance::toString() {

          return toStringMaxDecimalDigits( s_numericAfterDecimalPoint );
        }

        std::wstring AbstractInstance::toString( int attIndex ) {
          return toString( attIndex, s_numericAfterDecimalPoint );
        }

        std::wstring AbstractInstance::toString( int attIndex, int afterDecimalPoint )
        { // @pure@

          StringBuffer *text = new StringBuffer();

          if( isMissing( attIndex ) ) {
            text->append( L"?" );
          } else {
            if( m_Dataset == nullptr ) {
              text->append( Utils::doubleToString( value( attIndex ), afterDecimalPoint ) );
            } else {
              switch( m_Dataset->attribute( attIndex ).type() ) {
              case Attribute::NOMINAL:
              case Attribute::STRING:
              case Attribute::DATE:
              case Attribute::RELATIONAL:
                text->append( Utils::quote( stringValue( attIndex ) ) );
                break;
              case Attribute::NUMERIC:
                text->append( Utils::doubleToString( value( attIndex ), afterDecimalPoint ) );
                break;
              default:
                throw IllegalStateException( L"Unknown attribute type" );
              }
            }
          }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
          return text->toString();
        }

        std::wstring AbstractInstance::toString( Attribute *att ) {

          return toString( att->index() );
        }

        std::wstring AbstractInstance::toString( Attribute *att, int afterDecimalPoint ) {

          return toString( att->index(), afterDecimalPoint );
        }

        double AbstractInstance::value( Attribute *att )
        { // @pure@

          return value( att->index() );
        }

        double AbstractInstance::valueSparse( int indexOfIndex )
        { // @pure@

          return m_AttValues[indexOfIndex];
        }

        double AbstractInstance::weight()
        { // @pure@

          return m_Weight;
        }

        std::wstring AbstractInstance::getRevision() {
          return RevisionUtils::extract( L"$Revision: 10649 $" );
        }
    }
}
