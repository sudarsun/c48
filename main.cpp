#include "core/DataSource.h"
#include "core/Instances.h"
#include "evaluation/NominalPrediction.h"
#include "evaluation/NumericPrediction.h"
#include <iostream>

using namespace std;

int main()
{
    DataSource data("E:\\C4.5\\sleep.names");
    Instances *inst = nullptr;
    inst = data.getStructure();
    int num = inst->numAttributes();
    for (int i=0; i<num; i++)
        cout << "Attribute:" << inst->attribute(i)->name() <<endl;

    inst = data.getDataSet();
    cout << "Number of Attribute:" << inst->numAttributes() <<endl;
    Prediction *p = nullptr;
    cout << "";
    return 0;
}
