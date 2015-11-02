#ifndef _NBTREESPLIT_
#define _NBTREESPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class C45Split;
class NBTreeNoSplit;
class Instances;
class Instance;


                /// <summary>
                /// Class implementing a NBTree split on an attribute.
                /// 
                /// @author Mark Hall (mhall@cs.waikato.ac.nz)
                /// @version $Revision$
                /// </summary>
                class NBTreeSplit : public ClassifierSplitModel {

                  /// <summary>
                  /// Desired number of branches. </summary>
              protected:
                  int mcomplexityIndex = 0;

                  /// <summary>
                  /// Attribute to split on. </summary>
                  const int mattIndex;

                  /// <summary>
                  /// The sum of the weights of the instances. </summary>
                  const double msumOfWeights;

                  /// <summary>
                  /// The weight of the instances incorrectly classified by the naive bayes
                  /// models arising from this split
                  /// </summary>
                  double merrors = 0;

                  C45Split *mc45S;

                  /// <summary>
                  /// The global naive bayes model for this node </summary>
              public:
                  NBTreeNoSplit *mglobalNB;

                  /// <summary>
                  /// Initializes the split model.
                  /// </summary>
                  NBTreeSplit( int attIndex, int minNoObj, double sumOfWeights );

                  /// <summary>
                  /// Creates a NBTree-type split on the given data. Assumes that none of the
                  /// class values is missing.
                  /// </summary>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual void buildClassifier( Instances *trainInstances );

                  /// <summary>
                  /// Returns index of attribute for which split was generated.
                  /// </summary>
                  int attIndex();

                  /// <summary>
                  /// Creates split on enumerated attribute.
                  /// </summary>
                  /// <exception cref="Exception"> if something goes wrong </exception>
              private:
                  void handleEnumeratedAttribute( Instances *trainInstances );

                  /// <summary>
                  /// Creates split on numeric attribute.
                  /// </summary>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  void handleNumericAttribute( Instances *trainInstances );

                  /// <summary>
                  /// Returns index of subset instance is assigned to. Returns -1 if instance is
                  /// assigned to more than one subset.
                  /// </summary>
                  /// <exception cref="Exception"> if something goes wrong </exception>
              public:
                  virtual int whichSubset( Instance *instance );

                  /// <summary>
                  /// Returns weights if instance is assigned to more than one subset. Returns
                  /// null if instance is only assigned to one subset.
                  /// </summary>
                  virtual std::vector<double> weights( Instance *instance );

                  /// <summary>
                  /// Returns a string containing java source code equivalent to the test made at
                  /// this node. The instance being tested is called "i".
                  /// </summary>
                  /// <param name="index"> index of the nominal value tested </param>
                  /// <param name="data"> the data containing instance structure info </param>
                  /// <returns> a value of type 'String' </returns>
                  virtual std::string sourceExpression( int index, Instances *data );

                  /// <summary>
                  /// Prints the condition satisfied by instances in a subset.
                  /// </summary>
                  /// <param name="index"> of subset </param>
                  /// <param name="data"> training set. </param>
                  virtual std::string rightSide( int index, Instances *data );

                  /// <summary>
                  /// Prints left side of condition..
                  /// </summary>
                  /// <param name="data"> training set. </param>
                  virtual std::string leftSide( Instances *data );

                  /// <summary>
                  /// Return the probability for a class value
                  /// </summary>
                  /// <param name="classIndex"> the index of the class value </param>
                  /// <param name="instance"> the instance to generate a probability for </param>
                  /// <param name="theSubset"> the subset to consider </param>
                  /// <returns> a probability </returns>
                  /// <exception cref="Exception"> if an error occurs </exception>
                  virtual double classProb( int classIndex, Instance *instance, int theSubset );

                  /// <summary>
                  /// Return the global naive bayes model for this node
                  /// </summary>
                  /// <returns> a <code>NBTreeNoSplit</code> value </returns>
                  virtual NBTreeNoSplit *getGlobalModel();

                  /// <summary>
                  /// Set the global naive bayes model for this node
                  /// </summary>
                  /// <param name="global"> a <code>NBTreeNoSplit</code> value </param>
                  virtual void setGlobalModel( NBTreeNoSplit *global );

                  /// <summary>
                  /// Return the errors made by the naive bayes models arising from this split.
                  /// </summary>
                  /// <returns> a <code>double</code> value </returns>
                  virtual double getErrors();

                };

#endif	//#ifndef _NBTREESPLIT_
