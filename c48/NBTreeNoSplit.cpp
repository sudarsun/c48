#include "NBTreeNoSplit.h"
#include "Instances.h"
#include "NaiveBayesUpdateable.h"
#include "Discretize.h"
#include "Instance.h"
#include "Classifier.h"
#include "Evaluation.h"


                NBTreeNoSplit::NBTreeNoSplit() {
                  mnumSubsets = 1;
                }

                void NBTreeNoSplit::buildClassifier( Instances *instances ) {
                  mnb = new NaiveBayesUpdateable();
                  mdisc = new Discretize();
                  mdisc->setInputFormat( instances );
                  Instances *temp = Filter::useFilter( instances, mdisc );
                  mnb->buildClassifier( temp );
                  if( temp->numInstances() >= 5 ) {
                    merrors = crossValidate( mnb, temp, new Random( 1 ) );
                  }
                  mnumSubsets = 1;
                }

                double NBTreeNoSplit::getErrors() {
                  return merrors;
                }

                Discretize *NBTreeNoSplit::getDiscretizer() {
                  return mdisc;
                }

                NaiveBayesUpdateable *NBTreeNoSplit::getNaiveBayesModel() {
                  return mnb;
                }

                int NBTreeNoSplit::whichSubset( Instance *instance ) {

                  return 0;
                }

                std::vector<double> NBTreeNoSplit::weights( Instance *instance ) {

                  return nullptr;
                }

                std::string NBTreeNoSplit::leftSide( Instances *instances ) {

                  return "";
                }

                std::string NBTreeNoSplit::rightSide( int index, Instances *instances ) {

                  return "";
                }

                std::string NBTreeNoSplit::sourceExpression( int index, Instances *data ) {

                  return "true"; // or should this be false??
                }

                double NBTreeNoSplit::classProb( int classIndex, Instance *instance, int theSubset ) {
                  mdisc->input( instance );
                  Instance *temp = mdisc->output();
                  return mnb->distributionForInstance( temp )[classIndex];
                }

                std::string NBTreeNoSplit::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                  return mnb->toString();
                }

                double NBTreeNoSplit::crossValidate( NaiveBayesUpdateable *fullModel, Instances *trainingSet, Random *r ) {
                  // make some copies for fast evaluation of 5-fold xval
                  std::vector<Classifier*> copies = AbstractClassifier::makeCopies( fullModel, 5 );
                  Evaluation *eval = new Evaluation( trainingSet );
                  // make some splits
                  for( int j = 0; j < 5; j++ ) {
                    Instances *test = trainingSet->testCV( 5, j );
                    // unlearn these test instances
                    for( int k = 0; k < test->numInstances(); k++ ) {
                  test->instance( k )->setWeight( -test->instance( k )->weight() );
                  ( static_cast<NaiveBayesUpdateable*>( copies[j] ) )->updateClassifier( test->instance( k ) );
                  // reset the weight back to its original value
                  test->instance( k )->setWeight( -test->instance( k )->weight() );
                    }
                    eval->evaluateModel( copies[j], test );
                  }
                  return eval->incorrect();
                }

            