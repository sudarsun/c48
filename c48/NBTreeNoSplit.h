#ifndef _NBTREENOSPLIT_
#define _NBTREENOSPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Instances;
class NaiveBayesUpdateable;
class Discretize;
class Instance;
class Classifier;
class Evaluation;

                /// <summary>
                /// Class implementing a "no-split"-split (leaf node) for naive bayes
                /// trees.
                /// 
                /// @author Mark Hall (mhall@cs.waikato.ac.nz)
                /// @version $Revision$
                /// </summary>
                class NBTreeNoSplit : public ClassifierSplitModel {

                  /// <summary>
                  /// the naive bayes classifier </summary>
              protected:
                  NaiveBayesUpdateable *mnb;

                  /// <summary>
                  /// the discretizer used </summary>
                  Discretize *mdisc;

                  /// <summary>
                  /// errors on the training data at this node </summary>
                  double merrors = 0;

              public:
                  NBTreeNoSplit();

                  /// <summary>
                  /// Build the no-split node
                  /// </summary>
                  /// <param name="instances"> an <code>Instances</code> value </param>
                  /// <exception cref="Exception"> if an error occurs </exception>
                  void buildClassifier( Instances *instances );

                  /// <summary>
                  /// Return the errors made by the naive bayes model at this node
                  /// </summary>
                  /// <returns> the number of errors made </returns>
                  double getErrors();

                  /// <summary>
                  /// Return the discretizer used at this node
                  /// </summary>
                  /// <returns> a <code>Discretize</code> value </returns>
                  Discretize *getDiscretizer();

                  /// <summary>
                  /// Get the naive bayes model at this node
                  /// </summary>
                  /// <returns> a <code>NaiveBayesUpdateable</code> value </returns>
                  NaiveBayesUpdateable *getNaiveBayesModel();

                  /// <summary>
                  /// Always returns 0 because only there is only one subset.
                  /// </summary>
                  int whichSubset( Instance *instance );

                  /// <summary>
                  /// Always returns null because there is only one subset.
                  /// </summary>
                  std::vector<double> weights( Instance *instance );

                  /// <summary>
                  /// Does nothing because no condition has to be satisfied.
                  /// </summary>
                  std::string leftSide( Instances *instances );

                  /// <summary>
                  /// Does nothing because no condition has to be satisfied.
                  /// </summary>
                  std::string rightSide( int index, Instances *instances );

                  /// <summary>
                  /// Returns a string containing java source code equivalent to the test
                  /// made at this node. The instance being tested is called "i".
                  /// </summary>
                  /// <param name="index"> index of the nominal value tested </param>
                  /// <param name="data"> the data containing instance structure info </param>
                  /// <returns> a value of type 'String' </returns>
                  std::string sourceExpression( int index, Instances *data );

                  /// <summary>
                  /// Return the probability for a class value
                  /// </summary>
                  /// <param name="classIndex"> the index of the class value </param>
                  /// <param name="instance"> the instance to generate a probability for </param>
                  /// <param name="theSubset"> the subset to consider </param>
                  /// <returns> a probability </returns>
                  /// <exception cref="Exception"> if an error occurs </exception>
                  double classProb( int classIndex, Instance *instance, int theSubset );

                  /// <summary>
                  /// Return a textual description of the node
                  /// </summary>
                  /// <returns> a <code>String</code> value </returns>
                  virtual std::string toString() override;

                  /// <summary>
                  /// Utility method for fast 5-fold cross validation of a naive bayes
                  /// model
                  /// </summary>
                  /// <param name="fullModel"> a <code>NaiveBayesUpdateable</code> value </param>
                  /// <param name="trainingSet"> an <code>Instances</code> value </param>
                  /// <param name="r"> a <code>Random</code> value </param>
                  /// <returns> a <code>double</code> value </returns>
                  /// <exception cref="Exception"> if an error occurs </exception>
                  static double crossValidate( NaiveBayesUpdateable *fullModel, Instances *trainingSet, Random *r );
                 
                };


#endif	//#ifndef _NBTREENOSPLIT_
