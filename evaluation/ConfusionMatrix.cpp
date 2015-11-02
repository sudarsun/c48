#include "ConfusionMatrix.h"
#include "CostMatrix.h"
#include "TwoClassStats.h"
#include "NominalPrediction.h"
#include "Prediction.h"
#include "StringBuffer.h"

namespace weka {
    namespace classifiers {
        namespace evaluation {
            using weka::classifiers::CostMatrix;
            using weka::core::RevisionUtils;
            using weka::core::Utils;
            using weka::core::matrix::Matrix;

            ConfusionMatrix::ConfusionMatrix( std::vector<std::wstring> &classNames ) : weka::core::matrix::Matrix( classNames.length, classNames.length ) {

              m_ClassNames = classNames.clone();
            }

            ConfusionMatrix *ConfusionMatrix::makeWeighted( CostMatrix *costs ) throw( std::exception ) {

              if( costs->size() != size() ) {
                throw std::exception( L"Cost and confusion matrices must be the same size" );
              }
              ConfusionMatrix *weighted = new ConfusionMatrix( m_ClassNames );
              for( int row = 0; row < size(); row++ ) {
                for( int col = 0; col < size(); col++ ) {
                  weighted->set( row, col, get( row, col ) * costs->getElement( row, col ) );
                }
              }
              return weighted;
            }

            void *ConfusionMatrix::clone() {

              ConfusionMatrix *m = static_cast<ConfusionMatrix*>( Matrix::clone() );
              m->m_ClassNames = m_ClassNames.clone();
              return m;
            }

            int ConfusionMatrix::size() {

              return m_ClassNames.size();
            }

            std::wstring ConfusionMatrix::className( int index ) {

              return m_ClassNames[index];
            }

            void ConfusionMatrix::addPrediction( NominalPrediction *pred ) throw( std::exception ) {

              if( pred->predicted() == NominalPrediction::MISSING_VALUE ) {
                throw std::exception( L"No predicted value given." );
              }
              if( pred->actual() == NominalPrediction::MISSING_VALUE ) {
                throw std::exception( L"No actual value given." );
              }
              set( static_cast<int>( pred->actual() ), static_cast<int>(pred->predicted()), get(static_cast<int>(pred->actual()), static_cast<int>(pred->predicted())) + pred->weight() );

            }

            void ConfusionMatrix::addPredictions( std::vector<Prediction*> &predictions ) throw( std::exception ) {

              for( int i = 0; i < predictions.size(); i++ ) {
                addPrediction( static_cast<NominalPrediction*>( predictions[i] ) );
              }
            }

            TwoClassStats *ConfusionMatrix::getTwoClassStats( int classIndex ) {

              double fp = 0, tp = 0, fn = 0, tn = 0;
              for( int row = 0; row < size(); row++ ) {
                for( int col = 0; col < size(); col++ ) {
                  if( row == classIndex ) {
                    if( col == classIndex ) {
                      tp += get( row, col );
                    } else {
                      fn += get( row, col );
                    }
                  } else {
                    if( col == classIndex ) {
                      fp += get( row, col );
                    } else {
                      tn += get( row, col );
                    }
                  }
                }
              }
              return new TwoClassStats( tp, fp, tn, fn );
            }

            double ConfusionMatrix::correct() {

              double correct = 0;
              for( int i = 0; i < size(); i++ ) {
                correct += get( i, i );
              }
              return correct;
            }

            double ConfusionMatrix::incorrect() {

              double incorrect = 0;
              for( int row = 0; row < size(); row++ ) {
                for( int col = 0; col < size(); col++ ) {
                  if( row != col ) {
                    incorrect += get( row, col );
                  }
                }
              }
              return incorrect;
            }

            double ConfusionMatrix::total() {

              double total = 0;
              for( int row = 0; row < size(); row++ ) {
                for( int col = 0; col < size(); col++ ) {
                  total += get( row, col );
                }
              }
              return total;
            }

            double ConfusionMatrix::errorRate() {

              return incorrect() / total();
            }

            std::wstring ConfusionMatrix::toString() {

              return toString( L"=== Confusion Matrix ===\n" );
            }

            std::wstring ConfusionMatrix::toString( const std::wstring &title ) {

              StringBuffer *text = new StringBuffer();
              std::vector<wchar_t> IDChars = { L'a', L'b', L'c', L'd', L'e', L'f', L'g', L'h', L'i', L'j', L'k', L'l', L'm', L'n', L'o', L'p', L'q', L'r', L's', L't', L'u', L'v', L'w', L'x', L'y', L'z' };
              int IDWidth;
              bool fractional = false;

              // Find the maximum value in the matrix
              // and check for fractional display requirement
              double maxval = 0;
              for( int i = 0; i < size(); i++ ) {
                for( int j = 0; j < size(); j++ ) {
                  double current = get( i, j );
                  if( current < 0 ) {
                    current *= -10;
                  }
                  if( current > maxval ) {
                    maxval = current;
                  }
                  double fract = current - Math::rint( current );
                  if( !fractional && ( ( log( fract ) / log( 10 ) ) >= -2 ) ) {
                    fractional = true;
                  }
                }
              }

              IDWidth = 1 + std::max( static_cast<int>( log( maxval ) / log( 10 ) + ( fractional ? 3 : 0 ) ), static_cast<int>( log( size() ) / log(IDChars.size()) ) );
              text->append( title )->append( L"\n" );
              for( int i = 0; i < size(); i++ ) {
                if( fractional ) {
                  text->append( L" " )->append( num2ShortID( i, IDChars, IDWidth - 3 ) )->append( L"   " );
                } else {
                  text->append( L" " )->append( num2ShortID( i, IDChars, IDWidth ) );
                }
              }
              text->append( L"     actual class\n" );
              for( int i = 0; i < size(); i++ ) {
                for( int j = 0; j < size(); j++ ) {
                  text->append( L" " )->append( Utils::doubleToString( get( i, j ), IDWidth, ( fractional ? 2 : 0 ) ) );
                }
                text->append( L" | " )->append( num2ShortID( i, IDChars, IDWidth ) )->append( L" = " )->append( m_ClassNames[i] )->append( L"\n" );
              }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
              return text->toString();
            }

            std::wstring ConfusionMatrix::num2ShortID( int num, std::vector<wchar_t> &IDChars, int IDWidth ) {

              std::vector<wchar_t> ID( IDWidth );
              int i;

              for( i = IDWidth - 1; i >= 0; i-- ) {
                ID[i] = IDChars[num % IDChars.size()];
                num = num / IDChars.size() - 1;
                if( num < 0 ) {
                  break;
                }
              }
              for( i--; i >= 0; i-- ) {
                ID[i] = L' ';
              }

              return std::wstring( ID );
            }

            std::wstring ConfusionMatrix::getRevision() {
              return RevisionUtils::extract( L"$Revision: 10169 $" );
            }
        }
    }
}
