#include "core/DataSource.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "core/Utils.h"
#include "c48/C48.h"
#include "evaluation/Evaluation.h"
#include "evaluation/CostMatrix.h"
#include <iostream>
#include <ctime>

void classify(char *fileName, bool isDumpTree = false);
void processClassifierPrediction(Instance *toPredict,
	Classifier *classifier, Evaluation &eval);

int main( int argc, char *argv[]  )
{
	if (argc != 3)
	{
		std::cout << std::endl;
		std::cout << "please check the argument(s). the tool expects min. 2 arguments " << std::endl;
		std::cout << "1.\tfileStem.names|fileStem.data\n2.\t<dump tree structure for classification> => enable->1, disable->0" << std::endl;
		std::cout << "\nExample: " << argv[0] << " sample.names|sample.data <0|1>" << std::endl;
		std::cout << std::endl;
		exit(0);
	}
	bool isDumpTree = atoi(argv[2]) == 0 ? false : true;
	classify(argv[1], isDumpTree);
    return 0;
}

void classify(char *fileName, bool isDumpTree)
{
	time_t startTime, TimeElapsed;
	
	string path(fileName);

	// Set data source
	DataSource data(path);
	Instances *inst = nullptr;
	time(&startTime);
	inst = data.getDataSet(); // Read data from *.data file
	time(&TimeElapsed);

	int num = inst->numAttributes(); // Get total number of attributes
	int totalInst = inst->numInstances();
	std::cout << "Schema:       " << "C++ 4.8 Decision Tree Implementation" << std::endl;
	std::cout << "Relation:     " << inst->getRelationName() << std::endl;
	std::cout << "Instances:    " << totalInst << std::endl;
	std::cout << "Attributes:   " << std::endl;
	for (int i = 0; i<num; i++)
		std::cout << "              " << inst->attribute(i)->name() << std::endl;

	std::cout << "\nTime taken to read data : "
		<< difftime(TimeElapsed, startTime)
		<< " seconds"
		<< std::endl << std::endl;
	std::cout << "Test mode : evaluate on training data" << std::endl;
	C48 *classifier = new C48();
	time(&startTime);
	classifier->buildClassifier(inst);
	time(&TimeElapsed);
	std::cout << "=== Classifier model (full training set) ===" << std::endl << std::endl;
	std::cout << classifier->toString(isDumpTree) << std::endl;
	std::cout << "\nTime taken to build model : "
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
	std::cout << " === Evaluation on training set ===" << std::endl;
	std::cout << eval->toSummaryString(true);
	std::cout << eval->toClassDetailsString() << std::endl;
	std::cout << eval->toMatrixString() << std::endl;
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
