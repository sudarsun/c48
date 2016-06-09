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

void classify(C48 &, char *, char *, bool isDumpTree = false);

int main( int argc, char *argv[]  )
{
    char *trainFile = nullptr, *testFile = nullptr;
    bool isDumpTree;
    bool minArg = false;
    C48 *classifier = new C48();
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-train") == 0) {
            minArg = true;
            // We know the next argument *should* be the filename
            if (i+1 < argc)
            trainFile = argv[i++];
            else
            minArg = false;
        }
        else if (strcmp(argv[i], "-test") == 0) {
          if (i+1 < argc)
            testFile = argv[i++];
          else
            minArg = false;
        }
        else if (strcmp(argv[i], "-D") == 0) {
            isDumpTree = true;
        }
    }
    // Assign Command line 
    if (!classifier->setParameters(argc, argv) || !minArg)
    {
        std::cout << "C++ 4.8 Decision Tree Tool" << std::endl;
        std::cout << argv[0] << " <source> <flag> <options> " << std::endl;
        std::cout << "  SOURCE:" << std::endl;
        std::cout << "\t-train <training data> - fileStem.names|fileStem.data" << std::endl;
        std::cout << "\t-test <testing data> - fileStem.test (optional)" << std::endl;
        std::cout << "  FLAG:" << std::endl;
        std::cout << "\t-D - To dump tree structure for classification" << std::endl;
        std::cout << "\t-U - To use unpruned tree" << std::endl;
        std::cout << "\t-R - To use reduced error pruning" << std::endl;
        std::cout << "\t-S - Don't perform subtree raising" << std::endl;
        std::cout << "\t-L - Do not clean up after the tree has been built" << std::endl;
        std::cout << "\t-A - Laplace smoothing for predicted probabilities" << std::endl;
        std::cout << "  OPTIONS:" << std::endl;
        std::cout << "\t-C <pruning confidence> - Set confidence threshold for pruning (default .25)" << std::endl;
        std::cout << "\t-M <minimum number of instances> - Set minimum number of instances per leaf (default 2)" << std::endl;
        std::cout << "\t-N <number of folds> - Set number of folds for reduced error pruning. One fold is used as pruning set (default 3)" << std::endl;
        std::cout << std::endl;
        exit(0);
    }
    exit(0);
    classify(*classifier, trainFile, testFile, isDumpTree);
    return 0;
}

void classify(C48 &classifier, char *trainFile, char *testFile, bool isDumpTree)
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
    //C48 *classifier = new C48();
    time(&startTime);
    classifier.buildClassifier(*instTrain);
    time(&TimeElapsed);
    std::cout << "=== Classifier model (full training set) ===" << std::endl << std::endl;
    std::cout << classifier.toString(isDumpTree) << std::endl;
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
            eval->evaluateModelOnceAndRecordPrediction(classifier, instance);
        }
    }
    else
    {
        totalInst = instTrain->numInstances();
        for (int i = 0; i < totalInst; i++)
        {
            Instance &instance = instTrain->instance(i);
            eval->evaluateModelOnceAndRecordPrediction(classifier, instance);
        }
    }

    
    std::cout << " === Evaluation on training set ===" << std::endl;
    std::cout << eval->toSummaryString(true);
    std::cout << eval->toClassDetailsString() << std::endl;
    std::cout << eval->toMatrixString() << std::endl;
}
