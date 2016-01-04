#include "C45ModelSelection.h"
#include "C45Split.h"
#include "core/Instances.h"
#include "ClassifierSplitModel.h"
#include "NoSplit.h"
#include "Distribution.h"
#include "core/Attribute.h"
#include "core/Utils.h"
#include <iostream>


C45ModelSelection::C45ModelSelection(int minNoObj, Instances *allData, bool useMDLcorrection, bool doNotMakeSplitPointActualValue) :
    mminNoObj(minNoObj),
    museMDLcorrection(useMDLcorrection),
    mdoNotMakeSplitPointActualValue(doNotMakeSplitPointActualValue)
{
    mallData = allData;
}

void C45ModelSelection::cleanup()
{

    delete mallData;
}

ClassifierSplitModel *C45ModelSelection::selectModel(Instances *data)
{

    double minResult;
    std::vector<C45Split*> currentModel;
    C45Split *bestModel = nullptr;
    NoSplit *noSplitModel = nullptr;
    double averageInfoGain = 0;
    int validModels = 0;
    bool multiVal = true;
    Distribution *checkDistribution;
    Attribute *attribute;
    double sumOfWeights;
    int i;

    try
    {

        // Check if all Instances belong to one class or if not
        // enough Instances to split.
        checkDistribution = new Distribution(data);
        noSplitModel = new NoSplit(checkDistribution);
        if (Utils::sm(checkDistribution->total(), 2 * mminNoObj) || Utils::eq(checkDistribution->total(), checkDistribution->perClass(checkDistribution->maxClass())))
        {
            return noSplitModel;
        }

        // Check if all attributes are nominal and have a
        // lot of values.
        if (mallData != nullptr)
        {
            int totalattribute = data->numAttributes();
            for (int i = 0; i < totalattribute; i++)
            {
                attribute = data->attribute(i);
                if ((attribute->isNumeric()) || (Utils::sm(attribute->numValues(), (0.3 * mallData->numInstances()))))
                {
                    multiVal = false;
                    break;
                }
            }
        }

        currentModel = std::vector<C45Split*>(data->numAttributes());
        sumOfWeights = data->sumOfWeights();

        // For each attribute.
        for (i = 0; i < data->numAttributes(); i++)
        {

            // Apart from class attribute.
            if (i != (data)->classIndex())
            {

                // Get models for current attribute.
                currentModel[i] = new C45Split(i, mminNoObj, sumOfWeights, museMDLcorrection);
                // Check for Dataset assignment
                currentModel[i]->buildClassifier(*data);

                // Check if useful split for current attribute
                // exists and check for enumerated attributes with
                // a lot of values.
                if (currentModel[i]->checkModel())
                {
                    if (mallData != nullptr)
                    {
                        if ((data->attribute(i)->isNumeric()) || (multiVal || Utils::sm(data->attribute(i)->numValues(), (0.3 * mallData->numInstances()))))
                        {
                            averageInfoGain = averageInfoGain + currentModel[i]->infoGain();
                            validModels++;
                        }
                    }
                    else
                    {
                        averageInfoGain = averageInfoGain + currentModel[i]->infoGain();
                        validModels++;
                    }
                }
            }
            else
            {
                currentModel[i]= nullptr;
            }
        }

        // Check if any useful split was found.
        if (validModels == 0)
        {
            return noSplitModel;
        }
        averageInfoGain = averageInfoGain / validModels;

        // Find "best" attribute to split on.
        minResult = 0;
        for (i = 0; i < data->numAttributes(); i++)
        {
            if ((i != (data)->classIndex()) && (currentModel[i]->checkModel()))
            {
                // Use 1E-3 here to get a closer approximation to the original
                // implementation.
                if ((currentModel[i]->infoGain() >= (averageInfoGain - 1E-3)) && Utils::gr(currentModel[i]->gainRatio(), minResult))
                {
                    bestModel = currentModel[i];
                    minResult = currentModel[i]->gainRatio();
                }
            }
        }

        // Check if useful split was found.
        if (Utils::eq(minResult, 0))
        {
            return noSplitModel;
        }

        // Add all Instances with unknown values for the corresponding
        // attribute to the distribution for the model, so that
        // the complete distribution is stored with the model.
        bestModel->distribution()->addInstWithUnknown(data, bestModel->attIndex());

        // Set the split point analogue to C45 if attribute numeric.
        if ((mallData != nullptr) && (!mdoNotMakeSplitPointActualValue))
        {
            bestModel->setSplitPoint(mallData);
        }
        return bestModel;
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
    }
    return nullptr;
}

ClassifierSplitModel *C45ModelSelection::selectModel(Instances *train, Instances *test)
{

    return selectModel(train);
}


