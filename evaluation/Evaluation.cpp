#include "Evaluation.h"

class UnivariateKernelEstimator;
class AbstractEvaluationMetric;

#include "CostMatrix.h"
#include "Prediction.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "c48/Classifier.h"


Evaluation::Evaluation()
{
    //ctor
}

Evaluation::~Evaluation()
{
    //dtor
}

Evaluation::Evaluation(Instances data)
{

}

Evaluation::Evaluation(Instances data, CostMatrix costMatrix)
{

}

std::vector<double> Evaluation::evaluateModel(Classifier *classifier, Instances *data)
{
	return { 0.0 };
}

void Evaluation::setPriors(Instances train)
{

}

double Evaluation::evaluateModelOnceAndRecordPrediction(Classifier *classifier, Instances *data)
{
	return 0.0;
}

double Evaluation::evaluateModelOnceAndRecordPrediction(std::vector<double> dist, Instance *instance)
{
	return 0.0;
}

double Evaluation::evaluationForSingleInstance(std::vector<double> dist, Instance *instance, bool storePredictions)
{
	return 0.0;
}
