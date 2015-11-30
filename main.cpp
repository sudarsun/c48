#include "core/DataSource.h"
#include "core/Instances.h"
#include "core/DenseInstance.h"
#include "core/Utils.h"
#include "c48/C48.h"
#include "evaluation/Evaluation.h"
#include "evaluation/CostMatrix.h"
#include <iostream>
#include <ctime>

using namespace std;
void classify();
int main()
{
	classify();	
    return 0;
}

void classify()
{
	
	time_t startTime, TimeElapsed;
#ifdef _WIN64
	std::string path = "E:\\C4.5\\sleep.names";
#else
	std::string path = "/opt/c48/sleep.names";
#endif
	// Set data source
	DataSource data(path);
	Instances *inst = nullptr;
	time(&startTime);
	inst = data.getDataSet(); // Read data from *.data file
	time(&TimeElapsed);

	int num = inst->numAttributes(); // Get total number of attributes
	int totalInst = inst->numInstances();
	cout << "Schema:       " << "C++ 4.8 Decision Tree Implementation" << endl;
	cout << "Relation:     " << inst->getRelationName() << endl;
	cout << "Instances:    " << totalInst << endl;
	cout << "Attributes:   " << endl;
	for (int i = 0; i<num; i++)
		cout << "              " << inst->attribute(i)->name() << endl;

	cout << "\nTime taken to read data : "
		<< difftime(TimeElapsed, startTime)
		<< " seconds"
		<< endl << endl; 
	cout << "Test mode : " << endl;
	C48 *classifier = new C48();
	time(&startTime);
	classifier->buildClassifier(inst);
	time(&TimeElapsed);
	cout << "=== Classifier model (full training set) ===" << endl << endl;
	cout << classifier->toString() << endl;
	cout << "\nTime taken to build model : "
		 << difftime(TimeElapsed, startTime)
		 << " seconds\n\n";
	cout << "=== Evaluation on training set ===" << endl;

	Evaluation *eval = nullptr;
	CostMatrix *costMatrix = nullptr;
	eval = new Evaluation(inst, costMatrix);
	for (int i = 0; i < totalInst; i++)
	{

	}
}
void processClassifierPrediction(Instance *toPredict,
	Classifier *classifier, Evaluation eval, Instances *plotInstances)
{
	try
	{
		double pred = eval.evaluateModelOnceAndRecordPrediction(classifier,	toPredict);
		if (plotInstances != nullptr) {
			std::vector<double> values = std::vector<double>(plotInstances->numAttributes());
			for (int i = 0; i < plotInstances->numAttributes(); i++) {
				if (i < toPredict->classIndex()) {
					values[i] = toPredict->value(i);
				}
				else if (i == toPredict->classIndex()) {
					values[i] = pred;
					values[i + 1] = toPredict->value(i);
					/*
					* // if the class value of the instances to predict is missing then
					* // set it to the predicted value if (toPredict.isMissing(i)) {
					* values[i+1] = pred; }
					*/
					i++;
				}
				else {
					values[i] = toPredict->value(i - 1);
				}
			}
			plotInstances->add(new DenseInstance(1.0, values));
		}

	}
	catch (std::exception ex)
	{

	}
}
