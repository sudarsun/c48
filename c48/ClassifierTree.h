#ifndef _CLASSIFIERTREE_
#define _CLASSIFIERTREE_

#include <string>
#include <vector>
#include <list>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ModelSelection;
class ClassifierSplitModel;
class Instances;
class Distribution;
class Instance;


                /// <summary>
                /// Class for handling a tree structure used for classification.
                ///
                /// @author Eibe Frank (eibe@cs.waikato.ac.nz)
                /// @version $Revision$
                /// </summary>
                class ClassifierTree  {

                  /// <summary>
                  /// The model selection method. </summary>
              protected:
                  ModelSelection *mtoSelectModel;

                  /// <summary>
                  /// Local model at node. </summary>
                  ClassifierSplitModel *mlocalModel;

                  /// <summary>
                  /// References to sons. </summary>
                  std::vector<ClassifierTree*> msons;

                  /// <summary>
                  /// True if node is leaf. </summary>
                  bool misLeaf = false;

                  /// <summary>
                  /// True if node is empty. </summary>
                  bool misEmpty = false;

                  /// <summary>
                  /// The training instances. </summary>
                  Instances *mtrain;

                  /// <summary>
                  /// The pruning instances. </summary>
                  Distribution *mtest;

                  /// <summary>
                  /// The id for the node. </summary>
                  int mid = 0;

                  /// <summary>
                  /// For getting a unique ID when outputting the tree (hashcode isn't guaranteed
                  /// unique)
                  /// </summary>
              private:
                  static long long PRINTED_NODES;

                  /// <summary>
                  /// Gets the next unique node ID.
                  /// </summary>
                  /// <returns> the next unique node ID. </returns>
              protected:
                  static long long nextID();

                  /// <summary>
                  /// Resets the unique node ID counter (e.g. between repeated separate print
                  /// types)
                  /// </summary>
                  static void resetID();

                  /// <summary>
                  /// Constructor.
                  /// </summary>
              public:
                  ClassifierTree( ModelSelection *toSelectLocModel );

                  /// <summary>
                  /// Method for building a classifier tree.
                  /// </summary>
                  /// <param name="data"> the data to build the tree from </param>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual void buildClassifier( Instances *data );

                  /// <summary>
                  /// Builds the tree structure.
                  /// </summary>
                  /// <param name="data"> the data for which the tree structure is to be generated. </param>
                  /// <param name="keepData"> is training data to be kept? </param>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual void buildTree( Instances *data, bool keepData );

                  /// <summary>
                  /// Builds the tree structure with hold out set
                  /// </summary>
                  /// <param name="train"> the data for which the tree structure is to be generated. </param>
                  /// <param name="test"> the test data for potential pruning </param>
                  /// <param name="keepData"> is training Data to be kept? </param>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual void buildTree( Instances *train, Instances *test, bool keepData );

                  /// <summary>
                  /// Classifies an instance.
                  /// </summary>
                  /// <param name="instance"> the instance to classify </param>
                  /// <returns> the classification </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual double classifyInstance( Instance *instance );

                  /// <summary>
                  /// Cleanup in order to save memory.
                  /// </summary>
                  /// <param name="justHeaderInfo"> </param>
                  void cleanup( Instances *justHeaderInfo );

                  /// <summary>
                  /// Returns class probabilities for a weighted instance.
                  /// </summary>
                  /// <param name="instance"> the instance to get the distribution for </param>
                  /// <param name="useLaplace"> whether to use laplace or not </param>
                  /// <returns> the distribution </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  std::vector<double> distributionForInstance( Instance *instance, bool useLaplace );

                  /// <summary>
                  /// Assigns a uniqe id to every node in the tree.
                  /// </summary>
                  /// <param name="lastID"> the last ID that was assign </param>
                  /// <returns> the new current ID </returns>
                  virtual int assignIDs( int lastID );

                  /// <summary>
                  /// Returns the type of graph this classifier represents.
                  /// </summary>
                  /// <returns> Drawable.TREE </returns>
                  virtual int graphType();

                  /// <summary>
                  /// Returns graph describing the tree.
                  /// </summary>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  /// <returns> the tree as graph </returns>
                  virtual std::string graph();

                  /// <summary>
                  /// Returns tree in prefix order.
                  /// </summary>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  /// <returns> the prefix order </returns>
                  virtual std::string prefix();

                  /// <summary>
                  /// Returns source code for the tree as an if-then statement. The class is
                  /// assigned to variable "p", and assumes the tested instance is named "i". The
                  /// results are returned as two stringbuffers: a section of code for assignment
                  /// of the class, and a section of code containing support code (eg: other
                  /// support methods).
                  /// </summary>
                  /// <param name="className"> the classname that this static classifier has </param>
                  /// <returns> an array containing two stringbuffers, the first string containing
                  ///         assignment code, and the second containing source for support code. </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual std::vector<std::string*> toSource( const std::string &className );

                  /// <summary>
                  /// Returns number of leaves in tree structure.
                  /// </summary>
                  /// <returns> the number of leaves </returns>
                  virtual int numLeaves();

                  /// <summary>
                  /// Returns number of nodes in tree structure.
                  /// </summary>
                  /// <returns> the number of nodes </returns>
                  virtual int numNodes();

                  /// <summary>
                  /// Prints tree structure.
                  /// </summary>
                  /// <returns> the tree structure </returns>
                  virtual std::string toString();

                  /// <summary>
                  /// Returns a newly created tree.
                  /// </summary>
                  /// <param name="data"> the training data </param>
                  /// <returns> the generated tree </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
              protected:
                  virtual ClassifierTree *getNewTree( Instances *data );

                  /// <summary>
                  /// Returns a newly created tree.
                  /// </summary>
                  /// <param name="train"> the training data </param>
                  /// <param name="test"> the pruning data. </param>
                  /// <returns> the generated tree </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  virtual ClassifierTree *getNewTree( Instances *train, Instances *test );

                  /// <summary>
                  /// Help method for printing tree structure.
                  /// </summary>
                  /// <param name="depth"> the current depth </param>
                  /// <param name="text"> for outputting the structure </param>
                  /// <exception cref="Exception"> if something goes wrong </exception>
              private:
                  void dumpTree( int depth, std::string *text );

                  /// <summary>
                  /// Help method for printing tree structure as a graph.
                  /// </summary>
                  /// <param name="text"> for outputting the tree </param>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  void graphTree( std::string *text );

                  /// <summary>
                  /// Prints the tree in prefix form
                  /// </summary>
                  /// <param name="text"> the buffer to output the prefix form to </param>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  void prefixTree( std::string *text );

                  /// <summary>
                  /// Help method for computing class probabilities of a given instance.
                  /// </summary>
                  /// <param name="classIndex"> the class index </param>
                  /// <param name="instance"> the instance to compute the probabilities for </param>
                  /// <param name="weight"> the weight to use </param>
                  /// <returns> the laplace probs </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  double getProbsLaplace( int classIndex, Instance *instance, double weight );

                  /// <summary>
                  /// Help method for computing class probabilities of a given instance.
                  /// </summary>
                  /// <param name="classIndex"> the class index </param>
                  /// <param name="instance"> the instance to compute the probabilities for </param>
                  /// <param name="weight"> the weight to use </param>
                  /// <returns> the probs </returns>
                  /// <exception cref="Exception"> if something goes wrong </exception>
                  double getProbs( int classIndex, Instance *instance, double weight );

                  /// <summary>
                  /// Method just exists to make program easier to read.
                  /// </summary>
                  ClassifierSplitModel *localModel();

                  /// <summary>
                  /// Method just exists to make program easier to read.
                  /// </summary>
                  ClassifierTree *son( int index );

                  /// <summary>
                  /// Computes a list that indicates node membership
                  /// </summary>
              public:
                  virtual std::vector<double> getMembershipValues( Instance *instance );

                  /// <summary>
                  /// Returns the revision string.
                  /// </summary>
                  /// <returns> the revision </returns>
                  virtual std::string getRevision();
                };

#endif	//#ifndef _CLASSIFIERTREE_
