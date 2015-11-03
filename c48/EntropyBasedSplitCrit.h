#ifndef _ENTROPYBASEDSPLITCRIT_
#define _ENTROPYBASEDSPLITCRIT_

#include "SplitCriterion.h"

class Distribution;




/// <summary>
/// "Abstract" class for computing splitting criteria
/// based on the entropy of a class distribution.
/// 
/// @author Eibe Frank (eibe@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class EntropyBasedSplitCrit : public SplitCriterion {


	/// <summary>
	/// Help method for computing entropy.
	/// </summary>
public:
	double lnFunc(double num);

	/// <summary>
	/// Computes entropy of distribution before splitting.
	/// </summary>
	double oldEnt(Distribution *bags);

	/// <summary>
	/// Computes entropy of distribution after splitting.
	/// </summary>
	double newEnt(Distribution *bags);

	/// <summary>
	/// Computes entropy after splitting without considering the
	/// class values.
	/// </summary>
	double splitEnt(Distribution *bags);
};


#endif	//#ifndef _ENTROPYBASEDSPLITCRIT_
