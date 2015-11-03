#ifndef _C45MODELSELECTION_
#define _C45MODELSELECTION_

#include "ModelSelection.h"
#include <string>
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class C45Split;
class Instances;
class ClassifierSplitModel;
class NoSplit;
class Distribution;
class Attribute;


/// <summary>
/// Class for selecting a C4.5-type split for a given dataset.
/// 
/// @author Eibe Frank (eibe@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class C45ModelSelection : public ModelSelection {


	/// <summary>
	/// Minimum number of objects in interval. </summary>
protected:
	const int mminNoObj;

	/// <summary>
	/// Use MDL correction? </summary>
	const bool museMDLcorrection;

	/// <summary>
	/// All the training data </summary>
	Instances *mallData;

	/// <summary>
	/// Do not relocate split point to actual data value </summary>
	const bool mdoNotMakeSplitPointActualValue;

	/// <summary>
	/// Initializes the split selection method with the given parameters.
	/// </summary>
	/// <param name="minNoObj"> minimum number of instances that have to occur in at least
	///          two subsets induced by split </param>
	/// <param name="allData"> FULL training dataset (necessary for selection of split
	///          points). </param>
	/// <param name="useMDLcorrection"> whether to use MDL adjustement when finding splits
	///          on numeric attributes </param>
	/// <param name="doNotMakeSplitPointActualValue"> if true, split point is not relocated
	///          by scanning the entire dataset for the closest data value </param>
public:
	C45ModelSelection(int minNoObj, Instances *allData, bool useMDLcorrection, bool doNotMakeSplitPointActualValue);

	/// <summary>
	/// Sets reference to training data to null.
	/// </summary>
	virtual void cleanup();

	/// <summary>
	/// Selects C4.5-type split for the given dataset.
	/// </summary>
	ClassifierSplitModel *selectModel(Instances *data);

	/// <summary>
	/// Selects C4.5-type split for the given dataset.
	/// </summary>
	ClassifierSplitModel *selectModel(Instances *train, Instances *test);

};

#endif	//#ifndef _C45MODELSELECTION_
