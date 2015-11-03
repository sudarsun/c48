#ifndef _NBTREECLASSIFIERTREE_
#define _NBTREECLASSIFIERTREE_

#include "ClassifierTree.h"
#include <string>
#include <stdexcept>

class ModelSelection;
class Instances;


/// <summary>
/// Class for handling a naive bayes tree structure used for classification.
/// 
/// @author Mark Hall (mhall@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class NBTreeClassifierTree : public ClassifierTree {

public:
	NBTreeClassifierTree(ModelSelection *toSelectLocModel);


	/// <summary>
	/// Method for building a naive bayes classifier tree
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual void buildClassifier(Instances *data);
	/// <summary>
	/// Print the models at the leaves
	/// </summary>
	/// <returns> textual description of the leaf models </returns>
	virtual std::string printLeafModels();

	/// <summary>
	/// Prints tree structure.
	/// </summary>
	virtual std::string toString();
	/// <summary>
	/// Returns graph describing the tree.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual std::string graph();


	/// <summary>
	/// Assigns a uniqe id to every node in the tree.
	/// 
	/// public int assignIDs(int lastID) {
	/// 
	/// int currLastID = lastID + 1;
	/// 
	/// mid = currLastID; if (msons != null) { for (int i = 0; i < msons.length;
	/// i++) { currLastID = msons[i].assignIDs(currLastID); } } return currLastID;
	/// }
	/// </summary>

	/// <summary>
	/// Returns a newly created tree.
	/// </summary>
	/// <param name="data"> the training data </param>
	/// <exception cref="Exception"> if something goes wrong </exception>
protected:
	virtual ClassifierTree *getNewTree(Instances *data);

	/// <summary>
	/// Returns a newly created tree.
	/// </summary>
	/// <param name="train"> the training data </param>
	/// <param name="test"> the pruning data. </param>
	/// <exception cref="Exception"> if something goes wrong </exception>
	virtual ClassifierTree *getNewTree(Instances *train, Instances *test);

	/// <summary>
	/// Help method for printing tree structure.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>
private:
	void dumpTreeNB(int depth, std::string text);

	/// <summary>
	/// Help method for printing tree structure as a graph.
	/// </summary>
	/// <exception cref="Exception"> if something goes wrong </exception>

	void graphTree(std::string text);

};

#endif	//#ifndef _NBTREECLASSIFIERTREE_
