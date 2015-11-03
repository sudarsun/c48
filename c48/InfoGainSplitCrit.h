#ifndef _INFOGAINSPLITCRIT_
#define _INFOGAINSPLITCRIT_

#include "EntropyBasedSplitCrit.h"
#include <string>
#include <limits>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Distribution;


/// <summary>
/// Class for computing the information gain for a given distribution.
/// 
/// @author Eibe Frank (eibe@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class InfoGainSplitCrit : public EntropyBasedSplitCrit {


	/// <summary>
	/// This method is a straightforward implementation of the information gain
	/// criterion for the given distribution.
	/// </summary>
public:
	double splitCritValue(Distribution *bags);

	/// <summary>
	/// This method computes the information gain in the same way C4.5 does.
	/// </summary>
	/// <param name="bags"> the distribution </param>
	/// <param name="totalNoInst"> weight of ALL instances (including the ones with missing
	///          values). </param>
	double splitCritValue(Distribution *bags, double totalNoInst);

	/// <summary>
	/// This method computes the information gain in the same way C4.5 does.
	/// </summary>
	/// <param name="bags"> the distribution </param>
	/// <param name="totalNoInst"> weight of ALL instances </param>
	/// <param name="oldEnt"> entropy with respect to "no-split"-model. </param>
	double splitCritValue(Distribution *bags, double totalNoInst, double oldEnt);

};


#endif	//#ifndef _INFOGAINSPLITCRIT_
