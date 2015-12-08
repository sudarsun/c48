#include "core/DataSource.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "core/Utils.h"
#include "c48/C48.h"
#include "evaluation/Evaluation.h"
#include "evaluation/CostMatrix.h"
#include <iostream>
#include <ctime>

using namespace std;
void classify(char *fileName, bool isDumpTree = false);
void processClassifierPrediction(Instance *toPredict,
	Classifier *classifier, Evaluation &eval);

int main( int argc, char *argv[]  )
{
	if (argc != 3)
	{
		std::cout << endl;
		std::cout << "please check the argument(s). the tool expects min. 2 arguments " << endl;
		std::cout << "1.\tfileStem.names|fileStem.data\n2.\t<dump tree structure for classification> => enable->1, disable->0" << std::endl;
		std::cout << "\nExample: " << argv[0] << " sample.names|sample.data <0|1>" << std::endl;
		std::cout << endl;
		exit(0);
	}
	bool isDumpTree = atoi(argv[2]) == 0 ? false : true;
	classify(argv[1], isDumpTree);
    return 0;
}

void classify(char *fileName, bool isDumpTree)
{
	time_t startTime, TimeElapsed;
	
	std::string path(fileName);

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
	cout << classifier->toString(isDumpTree) << endl;
	cout << "\nTime taken to build model : "
		 << difftime(TimeElapsed, startTime)
		 << " seconds\n\n";
	Evaluation *eval = nullptr;
	CostMatrix *costMatrix = nullptr;
	eval = new Evaluation(*inst, costMatrix);
	for (int i = 0; i < totalInst; i++)
	{
		Instance *instance = inst->instance(i);
		processClassifierPrediction(instance, classifier, *eval);
	}
	cout << " === Evaluation on training set ===" << endl;
	cout << eval->toSummaryString(true);
	cout << eval->toClassDetailsString() << endl;
	cout << eval->toMatrixString() << endl;
}

void processClassifierPrediction(Instance *toPredict,
	Classifier *classifier, Evaluation &eval)
{
	try
	{
		double pred = eval.evaluateModelOnceAndRecordPrediction(classifier,	toPredict);
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
}
