#ifndef _SPLITCRITERION_
#define _SPLITCRITERION_


//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Distribution;



/// <summary>
/// Abstract class for computing splitting criteria
/// with respect to distributions of class values.
/// 
/// @author Eibe Frank (eibe@cs.waikato.ac.nz)
/// @version $Revision$
/// </summary>
class SplitCriterion {


	/// <summary>
	/// Computes result of splitting criterion for given distribution.
	/// </summary>
	/// <returns> value of splitting criterion. 0 by default </returns>
public:
	virtual double splitCritValue(Distribution *bags);

	/// <summary>
	/// Computes result of splitting criterion for given training and
	/// test distributions.
	/// </summary>
	/// <returns> value of splitting criterion. 0 by default </returns>
	virtual double splitCritValue(Distribution *train, Distribution *test);

	/// <summary>
	/// Computes result of splitting criterion for given training and
	/// test distributions and given number of classes.
	/// </summary>
	/// <returns> value of splitting criterion. 0 by default </returns>
	virtual double splitCritValue(Distribution *train, Distribution *test, int noClassesDefault);

	/// <summary>
	/// Computes result of splitting criterion for given training and
	/// test distributions and given default distribution.
	/// </summary>
	/// <returns> value of splitting criterion. 0 by default </returns>
	virtual double splitCritValue(Distribution *train, Distribution *test, Distribution *defC);
};


#endif	//#ifndef _SPLITCRITERION_
