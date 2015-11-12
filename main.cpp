#include "core/DataSource.h"
#include "core/Instances.h"
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
	// Set data source
	DataSource data("E:\\C4.5\\sleep.names");
	Instances *inst = nullptr;
	time(&startTime);
	inst = data.getStructure(); // Read meta information from *.names file
	inst = data.getDataSet(); // Read data from *.data file
	time(&TimeElapsed);

	int num = inst->numAttributes(); // Get total number of attributes

	cout << "Schema:       " << "C++ 4.8 Decision Tree Implementation" << endl;
	cout << "Relation:     " << inst->getRelationName() << endl;
	cout << "Instances:    " << inst->numInstances() << endl;
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
		
}
