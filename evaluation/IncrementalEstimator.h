#ifndef _INCREMENTALESTIMATOR_
#define _INCREMENTALESTIMATOR_

/// <summary>
/// Interface for an incremental probability estimators.<para>
/// 
/// </para>
/// </summary>
class IncrementalEstimator {

	/// <summary>
	/// Add one value to the current estimator.
	/// </summary>
	/// <param name="data"> the new data value </param>
	/// <param name="weight"> the weight assigned to the data value  </param>
public:
	virtual void addValue(double data, double weight) = 0;

};
#endif //_INCREMENTALESTIMATOR_