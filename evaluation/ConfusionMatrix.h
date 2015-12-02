#ifndef _CONFUSIONMATRIX_
#define _CONFUSIONMATRIX_

#include "core/Matrix.h"
#include "Prediction.h"
#include "NominalPrediction.h"
#include <string>
#include <vector>

// Forward class declarations:
class CostMatrix;
class TwoClassStats;

/// <summary>
/// Cells of this matrix correspond to counts of the number (or weight) of
/// predictions for each actual value / predicted value combination.
/// 
/// </summary>
class ConfusionMatrix : public Matrix {

	/// <summary>
	/// Stores the names of the classes </summary>
protected:
	std::vector<std::string> mClassNames;

	/// <summary>
	/// Creates the confusion matrix with the given class names.
	/// </summary>
	/// <param name="classNames"> an array containing the names the classes. </param>
public:
	ConfusionMatrix(std::vector<std::string> classNames);

	/// <summary>
	/// Makes a copy of this ConfusionMatrix after applying the supplied CostMatrix
	/// to the cells. The resulting ConfusionMatrix can be used to get
	/// cost-weighted statistics.
	/// </summary>
	/// <param name="costs"> the CostMatrix. </param>
	/// <returns> a ConfusionMatrix that has had costs applied. </returns>
	/// <exception cref="Exception"> if the CostMatrix is not of the same size as this
	///              ConfusionMatrix. </exception>
	virtual ConfusionMatrix *makeWeighted(CostMatrix *costs);

	/// <summary>
	/// Gets the number of classes.
	/// </summary>
	/// <returns> the number of classes </returns>
	virtual int size();

	/// <summary>
	/// Gets the name of one of the classes.
	/// </summary>
	/// <param name="index"> the index of the class. </param>
	/// <returns> the class name. </returns>
	virtual std::string className(int index);

	/// <summary>
	/// Includes a prediction in the confusion matrix.
	/// </summary>
	/// <param name="pred"> the NominalPrediction to include </param>
	/// <exception cref="Exception"> if no valid prediction was made (i.e. unclassified). </exception>
	virtual void addPrediction(NominalPrediction *pred);

	/// <summary>
	/// Includes a whole bunch of predictions in the confusion matrix.
	/// </summary>
	/// <param name="predictions"> a FastVector containing the NominalPredictions to
	///          include </param>
	/// <exception cref="Exception"> if no valid prediction was made (i.e. unclassified). </exception>
	virtual void addPredictions(std::vector<Prediction*> &predictions);

	/// <summary>
	/// Gets the performance with respect to one of the classes as a TwoClassStats
	/// object.
	/// </summary>
	/// <param name="classIndex"> the index of the class of interest. </param>
	/// <returns> the generated TwoClassStats object. </returns>
	virtual TwoClassStats *getTwoClassStats(int classIndex);

	/// <summary>
	/// Gets the number of correct classifications (that is, for which a correct
	/// prediction was made). (Actually the sum of the weights of these
	/// classifications)
	/// </summary>
	/// <returns> the number of correct classifications </returns>
	virtual double correct();

	/// <summary>
	/// Gets the number of incorrect classifications (that is, for which an
	/// incorrect prediction was made). (Actually the sum of the weights of these
	/// classifications)
	/// </summary>
	/// <returns> the number of incorrect classifications </returns>
	virtual double incorrect();

	/// <summary>
	/// Gets the number of predictions that were made (actually the sum of the
	/// weights of predictions where the class value was known).
	/// </summary>
	/// <returns> the number of predictions with known class </returns>
	virtual double total();

	/// <summary>
	/// Returns the estimated error rate.
	/// </summary>
	/// <returns> the estimated error rate (between 0 and 1). </returns>
	virtual double errorRate();

	/// <summary>
	/// Calls toString() with a default title.
	/// </summary>
	/// <returns> the confusion matrix as a string </returns>
	virtual std::string toString();

	/// <summary>
	/// Outputs the performance statistics as a classification confusion matrix.
	/// For each class value, shows the distribution of predicted class values.
	/// </summary>
	/// <param name="title"> the title for the confusion matrix </param>
	/// <returns> the confusion matrix as a String </returns>
	virtual std::string toString(const std::string &title);

	/// <summary>
	/// Method for generating indices for the confusion matrix.
	/// </summary>
	/// <param name="num"> integer to format </param>
	/// <returns> the formatted integer as a string </returns>
private:
	static std::string num2ShortID(int num, std::vector<char> &IDChars, int IDWidth);

};

#endif	//#ifndef _CONFUSIONMATRIX_
