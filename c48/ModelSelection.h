#ifndef _MODELSELECTION_
#define _MODELSELECTION_

#include <stdexcept>

class ClassifierSplitModel;
class Instances;

/**
 * Abstract class for model selection criteria.
 *
 */
class ModelSelection
{

public:
  /**
   * Selects a model for the given dataset.
   *
   * @exception Exception if model can't be selected
   */
	virtual ClassifierSplitModel *selectModel(Instances *data) = 0;

  /**
   * Selects a model for the given train data using the given test data
   *
   * @exception Exception if model can't be selected
   */
	virtual ClassifierSplitModel *selectModel(Instances *train, Instances *test);
};

#endif	//#ifndef _MODELSELECTION_
