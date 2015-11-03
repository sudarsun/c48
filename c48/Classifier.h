#ifndef _CLASSIFIER_
#define _CLASSIFIER_

#include <vector>
#include "core/Instances.h"
#include "core/Instance.h"


/// <summary>
/// Classifier interface. All schemes for numeric or nominal prediction in
/// Weka implement this interface. Note that a classifier MUST either implement
/// distributionForInstance() or classifyInstance().
/// </summary>
class Classifier {

	/// <summary>
	/// Generates a classifier. Must initialize all fields of the classifier
	/// that are not being set via options (ie. multiple calls of buildClassifier
	/// must always lead to the same result). Must not change the dataset
	/// in any way.
	/// </summary>
	/// <param name="data"> set of instances serving as training data </param>
	/// <exception cref="Exception"> if the classifier has not been
	/// generated successfully </exception>
public:
	virtual void buildClassifier(Instances *data) = 0;

	/// <summary>
	/// Classifies the given test instance. The instance has to belong to a
	/// dataset when it's being classified. Note that a classifier MUST
	/// implement either this or distributionForInstance().
	/// </summary>
	/// <param name="instance"> the instance to be classified </param>
	/// <returns> the predicted most likely class for the instance or
	/// Utils.missingValue() if no prediction is made </returns>
	/// <exception cref="Exception"> if an error occurred during the prediction </exception>
	virtual double classifyInstance(Instance *instance) = 0;

	/// <summary>
	/// Predicts the class memberships for a given instance. If
	/// an instance is unclassified, the returned array elements
	/// must be all zero. If the class is numeric, the array
	/// must consist of only one element, which contains the
	/// predicted value. Note that a classifier MUST implement
	/// either this or classifyInstance().
	/// </summary>
	/// <param name="instance"> the instance to be classified </param>
	/// <returns> an array containing the estimated membership
	/// probabilities of the test instance in each class
	/// or the numeric prediction </returns>
	/// <exception cref="Exception"> if distribution could not be
	/// computed successfully </exception>
	virtual std::vector<double> distributionForInstance(Instance *instance) = 0;

};



#endif	//_CLASSIFIER_
