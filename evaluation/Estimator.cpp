#include "Estimator.h"
#include "core/Instances.h"
#include "core/Utils.h"
#include "EstimatorUtils.h"

void Estimator::addValue(double data, double weight) {
    try {
        throw "Method to add single value is not implemented!\n Estimator should implement IncrementalEstimator.";
    }
    catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
}

void Estimator::addValues(Instances *data, int attrIndex) {
    double_array minMax(2);

    try {
        EstimatorUtils::getMinMax(data, attrIndex, minMax);
    }
    catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }

    double min = minMax[0];
    double max = minMax[1];

    // factor is 1.0, data set has not been reduced
    addValues(data, attrIndex, min, max, 1.0);
}

void Estimator::addValues(Instances *data, int attrIndex, double min, double max, double factor) {
    // no handling of factor, would have to be overridden

    // no handling of min and max, would have to be overridden

    int numInst = data->numInstances();
    for (int i = 1; i < numInst; i++) {
        addValue(data->instance(i)->value(attrIndex), 1.0);
    }
}

void Estimator::addValues(Instances *data, int attrIndex, int classIndex, int classValue) {
    // can estimator handle the data?
    mNoClass = false;

    // find the minimal and the maximal value
    double_array minMax(2);

    try {
        EstimatorUtils::getMinMax(data, attrIndex, minMax);
    }
    catch (std::exception &ex) {

        std::wcout << ex.what() << std::endl;
    }

    double min = minMax[0];
    double max = minMax[1];

    // extract the instances with the given class value
    Instances *workData = new Instances(data, 0);
    double factor = getInstancesFromClass(data, attrIndex, classIndex, static_cast<double>(classValue), workData);

    // if no data return
    if (workData->numInstances() == 0) {
        return;
    }

    addValues(data, attrIndex, min, max, factor);
}

void Estimator::addValues(Instances *data, int attrIndex, int classIndex, int classValue, double min, double max) {

    // extract the instances with the given class value
    Instances *workData = new Instances(data, 0);
    double factor = getInstancesFromClass(data, attrIndex, classIndex, static_cast<double>(classValue), workData);

    // if no data return
    if (workData->numInstances() == 0) {
        return;
    }

    addValues(data, attrIndex, min, max, factor);
}

double Estimator::getInstancesFromClass(Instances *data, int attrIndex, int classIndex, double classValue, Instances *workData) {

    int num = 0;
    int numClassValue = 0;
    for (int i = 0; i < data->numInstances(); i++) {
        if (!data->instance(i)->isMissing(attrIndex)) {
            num++;
            if (data->instance(i)->value(classIndex) == classValue) {
                workData->add(data->instance(i));
                numClassValue++;
            }
        }
    }

    double alphaFactor = double(static_cast<double>(numClassValue) / static_cast<double>(num));
    return alphaFactor;
}

void Estimator::setDebug(bool debug) {

    mDebug = debug;
}

bool Estimator::getDebug() {

    return mDebug;
}
