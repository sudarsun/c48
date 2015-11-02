#include "NBTreeModelSelection.h"
#include "NBTreeSplit.h"
#include "Instances.hpp"
#include "ClassifierSplitModel.h"
#include "NBTreeNoSplit.h"
#include "Distribution.h"
#include "Attribute.h"
#include "Utils.hpp"



                NBTreeModelSelection::NBTreeModelSelection( int minNoObj, Instances *allData ) : mminNoObj( minNoObj ) {
                  mallData = allData;
                }

                void NBTreeModelSelection::cleanup() {
                  delete mallData;
                }

                ClassifierSplitModel *NBTreeModelSelection::selectModel( Instances *data ) {

                  double globalErrors = 0;

                  double minResult;
                  std::vector<NBTreeSplit*> currentModel;
                  NBTreeSplit *bestModel = nullptr;
                  NBTreeNoSplit *noSplitModel = nullptr;
                  int validModels = 0;
                  Distribution *checkDistribution;
                  Attribute *attribute;
                  double sumOfWeights;
                  int i;

                  try {
                    // build the global model at this node
                    noSplitModel = new NBTreeNoSplit();
                    noSplitModel->buildClassifier( data );
                    if( data->numInstances() < 5 ) {
                      return noSplitModel;
                    }

                    // evaluate it
                    globalErrors = noSplitModel->getErrors();
                    if( globalErrors == 0 ) {
                      return noSplitModel;
                    }

                    // Check if all Instances belong to one class or if not
                    // enough Instances to split.
                    checkDistribution = new Distribution( data );
                    if( Utils::sm( checkDistribution->total(), mminNoObj ) || Utils::eq(checkDistribution->total(), checkDistribution->perClass(checkDistribution->maxClass())) ) {
                      return noSplitModel;
                    }

                    // Check if all attributes are nominal and have a
                    // lot of values.
                    if( mallData != nullptr ) {
                      java::util::Iterator<Attribute*> enu = data->enumerateAttributes();
                      while( enu->hasMoreElements() ) {
                        attribute = enu->nextElement();
                        if( ( attribute->isNumeric() ) || (Utils::sm(attribute->numValues(), (0.3 * mallData->numInstances()))) ) {
                          break;
                        }
                      }
                    }

                    currentModel = std::vector<NBTreeSplit*>( data->numAttributes() );
                    sumOfWeights = data->sumOfWeights();

                    // For each attribute.
                    for( i = 0; i < data->numAttributes(); i++ ) {

                      // Apart from class attribute.
                      if( i != ( data )->classIndex() ) {

                        // Get models for current attribute.
                        currentModel[i] = new NBTreeSplit( i, mminNoObj, sumOfWeights );
                        currentModel[i]->setGlobalModel( noSplitModel );
                        currentModel[i]->buildClassifier( data );

                        // Check if useful split for current attribute
                        // exists and check for enumerated attributes with
                        // a lot of values.
                        if( currentModel[i]->checkModel() ) {
                          validModels++;
                        }
                      } else {
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                        delete currentModel[i];
                      }
                    }

                    // Check if any useful split was found.
                    if( validModels == 0 ) {
                      return noSplitModel;
                    }

                    // Find "best" attribute to split on.
                    minResult = globalErrors;
                    for( i = 0; i < data->numAttributes(); i++ ) {
                      if( ( i != ( data )->classIndex() ) && (currentModel[i]->checkModel()) ) {
                        /*
                         * System.err.println("Errors for "+data.attribute(i).name()+" "+
                         * currentModel[i].getErrors());
                         */
                        if( currentModel[i]->getErrors() < minResult ) {
                          bestModel = currentModel[i];
                          minResult = currentModel[i]->getErrors();
                        }
                      }
                    }
                    // System.exit(1);
                    // Check if useful split was found.

                    if( ( ( globalErrors - minResult ) / globalErrors ) < 0.05 ) {
                      return noSplitModel;
                    }

                    /*
                     * if (bestModel == null) {
                     * System.err.println("This shouldn't happen! glob : "+globalErrors+
                     * " minRes : "+minResult); System.exit(1); }
                     */
                    // Set the global model for the best split
                    // bestModel.setGlobalModel(noSplitModel);

                    return bestModel;
                  } catch( std::exception &e ) {
                    std::cout << e.what();
                  }
                  return nullptr;
                }

                ClassifierSplitModel *NBTreeModelSelection::selectModel( Instances *train, Instances *test ) {

                  return selectModel( train );
                }
           