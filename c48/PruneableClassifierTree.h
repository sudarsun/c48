#ifndef _PRUNEABLECLASSIFIERTREE_
#define _PRUNEABLECLASSIFIERTREE_

#include "ClassifierTree.h"
#include <string>
#include <stdexcept>

class Instances;
class ClassifierSplitModel;


    /// <summary>
    /// Class for handling a tree structure that can
    /// be pruned using a pruning set.
    ///
    /// @author Eibe Frank (eibe@cs.waikato.ac.nz)
    /// @version $Revision$
    /// </summary>
    class PruneableClassifierTree : public ClassifierTree {

      /// <summary>
      /// True if the tree is to be pruned. </summary>
  protected:
      bool pruneTheTree = false;

      /// <summary>
      /// How many subsets of equal size? One used for pruning, the rest for training. </summary>
      int numSets = 3;

      /// <summary>
      /// Cleanup after the tree has been built. </summary>
      bool mcleanup = true;

      /// <summary>
      /// The random number seed. </summary>
      int mseed = 1;

      /// <summary>
      /// Constructor for pruneable tree structure. Stores reference
      /// to associated training data at each node.
      /// </summary>
      /// <param name="toSelectLocModel"> selection method for local splitting model </param>
      /// <param name="pruneTree"> true if the tree is to be pruned </param>
      /// <param name="num"> number of subsets of equal size </param>
      /// <param name="cleanup"> </param>
      /// <param name="seed"> the seed value to use </param>
      /// <exception cref="Exception"> if something goes wrong </exception>
  public:
      PruneableClassifierTree( ModelSelection *toSelectLocModel, bool pruneTree, int num, bool cleanup, int seed );

      /// <summary>
      /// Method for building a pruneable classifier tree.
      /// </summary>
      /// <param name="data"> the data to build the tree from </param>
      /// <exception cref="Exception"> if tree can't be built successfully </exception>
      virtual void buildClassifier( Instances *data );

      /// <summary>
      /// Prunes a tree.
      /// </summary>
      /// <exception cref="Exception"> if tree can't be pruned successfully </exception>
      virtual void prune();

      /// <summary>
      /// Returns a newly created tree.
      /// </summary>
      /// <param name="train"> the training data </param>
      /// <param name="test"> the test data </param>
      /// <returns> the generated tree </returns>
      /// <exception cref="Exception"> if something goes wrong </exception>
  protected:
      virtual ClassifierTree *getNewTree( Instances *train, Instances *test );

      /// <summary>
      /// Computes estimated errors for tree.
      /// </summary>
      /// <returns> the estimated errors </returns>
      /// <exception cref="Exception"> if error estimate can't be computed </exception>
  private:
      double errorsForTree();

      /// <summary>
      /// Computes estimated errors for leaf.
      /// </summary>
      /// <returns> the estimated errors </returns>
      /// <exception cref="Exception"> if error estimate can't be computed </exception>
      double errorsForLeaf();

      /// <summary>
      /// Method just exists to make program easier to read.
      /// </summary>
      ClassifierSplitModel *localModel();

      /// <summary>
      /// Method just exists to make program easier to read.
      /// </summary>
      PruneableClassifierTree *son( int index );

    };


#endif	//#ifndef _PRUNEABLECLASSIFIERTREE_
