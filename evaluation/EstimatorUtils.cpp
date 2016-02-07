#include "EstimatorUtils.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "Estimator.h"
#include <limits>


double EstimatorUtils::findMinDistance(Instances &inst, const int attrIndex) {
    double min = std::numeric_limits<double>::max();
    int numInst = inst.numInstances();
    double diff;
    if (numInst < 2) {
        return min;
    }
    int begin = -1;
    Instance *instance = nullptr;
    do {
        begin++;
        if (begin < numInst) {
            instance = &(inst.instance(begin));
        }
    } while (begin < numInst && instance->isMissing(attrIndex));

    double secondValue = inst.instance(begin).value(attrIndex);
    for (int i = begin; i < numInst && !inst.instance(i).isMissing(attrIndex); i++) {
        double firstValue = secondValue;
        secondValue = inst.instance(i).value(attrIndex);
        if (secondValue != firstValue) {
            diff = secondValue - firstValue;
            if (diff < min && diff > 0.0) {
                min = diff;
            }
        }
    }
    return min;
}

int EstimatorUtils::getMinMax(Instances &inst, const int attrIndex, double_array &minMax) {
    double min = std::numeric_limits<double>::quiet_NaN();;
    double max = std::numeric_limits<double>::quiet_NaN();;
    Instance *instance = nullptr;
    int numNotMissing = 0;
    if ((minMax.empty()) || (minMax.size() < 2)) {
        throw "Error in Program, privat method getMinMax";
    }

    int i = 0, totInst = inst.numInstances();
    if (totInst != 0)
    {
        do {
            instance = static_cast<Instance*>(&inst.instance(i++));
        } while (instance->isMissing(attrIndex) && i < totInst);

        // add values if not  missing
        if (!instance->isMissing(attrIndex)) {
            numNotMissing++;
            min = instance->value(attrIndex);
            max = instance->value(attrIndex);
        }
        while (i < totInst) {
            instance = static_cast<Instance*>(&inst.instance(i));
            if (!instance->isMissing(attrIndex)) {
                numNotMissing++;
                if (instance->value(attrIndex) < min) {
                    min = (instance->value(attrIndex));
                }
                else {
                    if (instance->value(attrIndex) > max) {
                        max = (instance->value(attrIndex));
                    }
                }
            }
        }
    }
    minMax[0] = min;
    minMax[1] = max;
    return numNotMissing;
}

//std::vector EstimatorUtils::getInstancesFromClass( Instances *data, int attrIndex, int classIndex, double classValue, Instances *workData ) {
//  
//  std::vector dataPlusInfo = std::vector( 0 );
//  int num = 0;
//  int numClassValue = 0;
//  //workData = new Instances(data, 0);
//  for( int i = 0; i < data->numInstances(); i++ ) {
//    if( !data->instance( i )->isMissing( attrIndex ) ) {
//  num++;
//  if( data->instance( i )->value( classIndex ) == classValue ) {
//    workData->add( data->instance( i ) );
//    numClassValue++;
//  }
//    }
//  }
//
//  double alphaFactor = double( static_cast<double>( numClassValue ) / static_cast<double>( num ) );
//  dataPlusInfo.push_back( workData );
//  dataPlusInfo.push_back( alphaFactor );
//  return dataPlusInfo;
//}

Instances *EstimatorUtils::getInstancesFromClass(Instances &data, int classIndex, double classValue) {
    Instances *workData = new Instances(&data, 0);
    for (int i = 0; i < data.numInstances(); i++) {
        if (data.instance(i).value(classIndex) == classValue) {
            workData->add(data.instance(i));
        }

    }
    return workData;
}

Instances *EstimatorUtils::getInstancesFromValue(Instances &data, const int index, const double v) {
    Instances *workData = new Instances(&data, 0);
    for (int i = 0; i < data.numInstances(); i++) {
        if (data.instance(i).value(index) == v) {
            workData->add(data.instance(i));
        }
    }
    return workData;
}

string EstimatorUtils::cutpointsToString(const double_array &cutPoints, const bool_array &cutAndLeft) {
    string text = "";
    if (cutPoints.empty()) {
        text.append("\n# no cutpoints found - attribute \n");
    }
    else {
        text.append(string("\n#* ") + std::to_string(cutPoints.size()) + string(" cutpoint(s) -\n"));
        for (int i = 0; i < cutPoints.size(); i++) {
            text.append(string("# ") + std::to_string(cutPoints[i]) + string(" "));

            text.append(string("") + std::to_string(cutAndLeft[i]) + string("\n"));
        }
        text.append("# end\n");
    }
    return text;
}
