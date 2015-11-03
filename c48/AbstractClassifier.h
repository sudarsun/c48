#ifndef _ABSTRACTCLASSIFIER_
#define _ABSTRACTCLASSIFIER_

#include "Classifier.h"
#include "evaluation/BatchPredictor.h"
#include <string>
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Instance;
class Instances;

/// <summary>
/// Abstract classifier. All schemes for numeric or nominal prediction in Weka
/// extend this class. Note that a classifier MUST either implement
/// distributionForInstance() or classifyInstance().
///
/// </summary>
class AbstractClassifier : public Classifier, public BatchPredictor {


	/// <summary>
	/// Whether the classifier is run in debug mode. </summary>
protected:
	bool m_Debug = false;

	/// <summary>
	/// Whether capabilities should not be checked before classifier is built. </summary>
	bool m_DoNotCheckCapabilities = false;
	int m_numDecimalPlaces = 2;
	/// <summary>
	/// The number of decimal places used when printing numbers in the model.
	/// </summary>
public:
	static const int NUM_DECIMAL_PLACES_DEFAULT = 2;



	/// <summary>
	/// Default preferred batch size for batch predictions </summary>
	std::string m_BatchSize = "100";


	/// <summary>
	/// Classifies the given test instance. The instance has to belong to a dataset
	/// when it's being classified. Note that a classifier MUST implement either
	/// this or distributionForInstance().
	/// </summary>
	/// <param name="instance"> the instance to be classified </param>
	/// <returns> the predicted most likely class for the instance or
	///         Utils.missingValue() if no prediction is made </returns>
	/// <exception cref="Exception"> if an error occurred during the prediction </exception>
public:
	virtual double classifyInstance(Instance *instance);

	/// <summary>
	/// Predicts the class memberships for a given instance. If an instance is
	/// unclassified, the returned array elements must be all zero. If the class is
	/// numeric, the array must consist of only one element, which contains the
	/// predicted value. Note that a classifier MUST implement either this or
	/// classifyInstance().
	/// </summary>
	/// <param name="instance"> the instance to be classified </param>
	/// <returns> an array containing the estimated membership probabilities of the
	///         test instance in each class or the numeric prediction </returns>
	/// <exception cref="Exception"> if distribution could not be computed successfully </exception>
	virtual std::vector<double> distributionForInstance(Instance *instance);

	/// <summary>
	/// Get whether debugging is turned on.
	/// </summary>
	/// <returns> true if debugging output is on </returns>
	virtual bool getDebug();

	/// <summary>
	/// Set debugging mode.
	/// </summary>
	/// <param name="debug"> true if debug output should be printed </param>
	virtual void setDebug(bool debug);

	/// <summary>
	/// Get whether capabilities checking is turned off.
	/// </summary>
	/// <returns> true if capabilities checking is turned off. </returns>
	virtual bool getDoNotCheckCapabilities();

	/// <summary>
	/// Set whether not to check capabilities.
	/// </summary>
	/// <param name="doNotCheckCapabilities"> true if capabilities are not to be checked. </param>
	virtual void setDoNotCheckCapabilities(bool doNotCheckCapabilities);

	/// <summary>
	/// Get the number of decimal places.
	/// </summary>
	virtual int getNumDecimalPlaces();

	/// <summary>
	/// Set the number of decimal places.
	/// </summary>
	virtual void setNumDecimalPlaces(int num);

	/// <summary>
	/// Set the preferred batch size for batch prediction.
	/// </summary>
	/// <param name="size"> the batch size to use </param>
	virtual void setBatchSize(const std::string &size);

	/// <summary>
	/// Get the preferred batch size for batch prediction.
	/// </summary>
	/// <returns> the preferred batch size </returns>
	virtual std::string getBatchSize();

	/// <summary>
	/// Return true if this classifier can generate batch predictions in an
	/// efficient manner. Default implementation here returns false. Subclasses to
	/// override as appropriate.
	/// </summary>
	/// <returns> true if this classifier can generate batch predictions in an
	///         efficient manner. </returns>
	virtual bool implementsMoreEfficientBatchPrediction();

	/// <summary>
	/// Batch prediction method. This default implementation simply calls
	/// distributionForInstance() for each instance in the batch. If subclasses can
	/// produce batch predictions in a more efficient manner than this they should
	/// override this method and also return true from
	/// implementsMoreEfficientBatchPrediction()
	/// </summary>
	/// <param name="batch"> the instances to get predictions for </param>
	/// <returns> an array of probability distributions, one for each instance in the
	///         batch </returns>
	/// <exception cref="Exception"> if a problem occurs. </exception>
	virtual std::vector<std::vector<double>> distributionsForInstances(Instances *batch);
};


#endif	// _ABSTRACTCLASSIFIER_
