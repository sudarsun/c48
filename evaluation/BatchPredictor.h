#ifndef _BATCHPREDICTOR_
#define _BATCHPREDICTOR_

#include <string>
#include <vector>

	/// <summary>
	/// Interface to something that can produce predictions in a batch manner 
	/// when presented with a set of Instances.
	/// 
	/// @author Mark Hall (mhall{[at]}pentaho{[dot]}com)
	/// @version $Revision: 11958 $
	/// </summary>
	class BatchPredictor {

	  /// <summary>
	  /// Set the batch size to use. The implementer will  
	  /// prefer (but not necessarily expect) this many instances 
	  /// to be passed in to distributionsForInstances().
	  /// </summary>
	  /// <param name="size"> the batch size to use </param>
  public:
	  virtual void setBatchSize( const std::string &size ) = 0;

	  /// <summary>
	  /// Get the batch size to use. The implementer will prefer (but not  
	  /// necessarily expect) this many instances to be passed in to 
	  /// distributionsForInstances(). Allows the preferred batch size 
	  /// to be encapsulated with the client.
	  /// </summary>
	  /// <returns> the batch size to use </returns>
	  virtual std::string getBatchSize() = 0;

	  /// <summary>
	  /// Batch scoring method
	  /// </summary>
	  /// <param name="insts"> the instances to get predictions for </param>
	  /// <returns> an array of probability distributions, one for each instance </returns>
	  /// <exception cref="Exception"> if a problem occurs </exception>
	  virtual std::vector<std::vector<double>> distributionsForInstances( Instances *insts ) = 0;

	  /// <summary>
	  /// Returns true if this BatchPredictor can generate batch predictions
	  /// in an efficient manner.
	  /// </summary>
	  /// <returns> true if batch predictions can be generated efficiently </returns>
	  virtual bool implementsMoreEfficientBatchPrediction() = 0;
    };
#endif	//#ifndef _BATCHPREDICTOR_
