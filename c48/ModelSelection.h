#ifndef _MODELSELECTION_
#define _MODELSELECTION_

#include <stdexcept>

class ClassifierSplitModel;
class Instances;


class ModelSelection
{

    /// <summary>
    /// Selects a model for the given dataset.
    /// </summary>
    /// <exception cref="Exception"> if model can't be selected </exception>
public:
    virtual ClassifierSplitModel *selectModel( Instances *data ) = 0;

    /// <summary>
    /// Selects a model for the given train data using the given test data
    /// </summary>
    /// <exception cref="Exception"> if model can't be selected </exception>
    virtual ClassifierSplitModel *selectModel( Instances *train, Instances *test );
};

#endif	//#ifndef _MODELSELECTION_
