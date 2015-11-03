#ifndef _NBTREEMODELSELECTION_
#define _NBTREEMODELSELECTION_

#include "ModelSelection.h"
#include <string>
#include <vector>
#include <stdexcept>

class NBTreeSplit;
class Instances;
class ClassifierSplitModel;
class NBTreeNoSplit;
class Distribution;
class Attribute;


/// <summary>
/// Class for selecting a NB tree split.
/// 
/// @author Mark Hall (mhall@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class NBTreeModelSelection : public ModelSelection {

	/// <summary>
	/// Minimum number of objects in interval. </summary>
protected:
	const int mminNoObj;

	/// <summary>
	/// All the training data </summary>
	Instances *mallData;

	/// <summary>
	/// Initializes the split selection method with the given parameters.
	/// </summary>
	/// <param name="minNoObj"> minimum number of instances that have to occur in at least
	///          two subsets induced by split </param>
	/// <param name="allData"> FULL training dataset (necessary for selection of split
	///          points). </param>
public:
	NBTreeModelSelection(int minNoObj, Instances *allData);

	/// <summary>
	/// Sets reference to training data to null.
	/// </summary>
	virtual void cleanup();

	/// <summary>
	/// Selects NBTree-type split for the given dataset.
	/// </summary>
	ClassifierSplitModel *selectModel(Instances *data);

	/// <summary>
	/// Selects NBTree-type split for the given dataset.
	/// </summary>
	ClassifierSplitModel *selectModel(Instances *train, Instances *test);

};


#endif	//#ifndef _NBTREEMODELSELECTION_
