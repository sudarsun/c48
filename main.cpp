#include "core/DataSource.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "core/Utils.h"
#include "c48/C48.h"
#include "evaluation/Evaluation.h"
#include "evaluation/CostMatrix.h"
#include <iostream>
#include <ctime>
#include <string.h>

void classify(char *trainFile, char *testFile, bool isDumpTree = false);

int main( int argc, char *argv[]  )
{
    char *trainFile = nullptr, *testFile = nullptr;
    bool isDumpTree;

    if (argc < 4)
    {
        std::cout << "C++ 4.8 Decision Tree Tool" << std::endl;
        std::cout << "-train <training data> - fileStem.names|fileStem.data" << std::endl;
        std::cout << "-test <testing data> - fileStem.test (optional)" << std::endl;
        std::cout << "-display <0|1> - dump tree structure for classification" << std::endl;
        std::cout << "Example: " << argv[0] << " -train sample.names -display 0" << std::endl;
        std::cout << "Example: " << argv[0] << " -train sample.data -test sample.test -display 1" << std::endl;
        std::cout << std::endl;
        exit(0);
    }
    else
    {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i],"-train") == 0) {
                // We know the next argument *should* be the filename:
                trainFile = argv[i + 1];
            }
            else if (strcmp(argv[i],"-test") == 0) {
                testFile = argv[i + 1];
            }
            else if (strcmp(argv[i], "-display") == 0) {
                isDumpTree = atoi(argv[i + 1]) == 0 ? false : true;;
            }
            else {
            }
        }
    }

    classify(trainFile, testFile, isDumpTree);
    return 0;
}

void classify(char *trainFile, char *testFile, bool isDumpTree)
{
    time_t startTime, TimeElapsed;
    
    string trainFilePpath(trainFile);

    // Set data source
    DataSource trainSource(trainFilePpath);
    Instances *instTrain = nullptr;
    time(&startTime);
    instTrain = trainSource.getDataSet(); // Read data from *.data file
    time(&TimeElapsed);

    int num = instTrain->numAttributes(); // Get total number of attributes
    int totalInst = instTrain->numInstances();
    std::cout << "Schema:       " << "C++ 4.8 Decision Tree Implementation" << std::endl;
    std::cout << "Relation:     " << instTrain->getRelationName() << std::endl;
    std::cout << "Instances:    " << totalInst << std::endl;
    std::cout << "Attributes:   " << std::endl;
    for (int i = 0; i<num; i++)
        std::cout << "              " << instTrain->attribute(i).name() << std::endl;

    std::cout << "\nTime taken to read data : "
        << difftime(TimeElapsed, startTime)
        << " seconds"
        << std::endl << std::endl;
    std::cout << "Test mode : evaluate on training data" << std::endl;
    C48 *classifier = new C48();
    time(&startTime);
    classifier->buildClassifier(*instTrain);
    time(&TimeElapsed);
    std::cout << "=== Classifier model (full training set) ===" << std::endl << std::endl;
    std::cout << classifier->toString(isDumpTree) << std::endl;
    std::cout << "\nTime taken to build model : "
         << difftime(TimeElapsed, startTime)
         << " seconds\n\n";
    Evaluation *eval = nullptr;
    CostMatrix *costMatrix = nullptr;
    eval = new Evaluation(*instTrain, costMatrix);

    if (testFile != nullptr)
    {
        string testFilePath(testFile);
        DataSource testSource(testFilePath);
        Instances *instTest = nullptr;
        instTest = testSource.getDataSet(); // Read data from *.test file

        totalInst = instTest->numInstances();
        for (int i = 0; i < totalInst; i++)
        {
            Instance &instance = instTest->instance(i);
            eval->evaluateModelOnceAndRecordPrediction(*classifier, instance);
        }
    }
    else
    {
        totalInst = instTrain->numInstances();
        for (int i = 0; i < totalInst; i++)
        {
            Instance &instance = instTrain->instance(i);
            eval->evaluateModelOnceAndRecordPrediction(*classifier, instance);
        }
    }

    
    std::cout << " === Evaluation on training set ===" << std::endl;
    std::cout << eval->toSummaryString(true);
    std::cout << eval->toClassDetailsString() << std::endl;
    std::cout << eval->toMatrixString() << std::endl;
}
