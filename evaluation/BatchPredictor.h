#ifndef _BATCHPREDICTOR_
#define _BATCHPREDICTOR_

#include <string>
#include <vector>
#include <core/Typedefs.h>

class Instances;

/**
 * Interface to something that can produce predictions in a batch manner
 * when presented with a set of Instances.
 *
 */
class BatchPredictor {

public:
    /**
     * Set the batch size to use. The implementer will
     * prefer (but not necessarily expect) this many instances
     * to be passed in to distributionsForInstances().
     *
     * @param size the batch size to use
     */
    virtual void setBatchSize(const string &size) = 0;

    /**
     * Get the batch size to use. The implementer will prefer (but not
     * necessarily expect) this many instances to be passed in to
     * distributionsForInstances(). Allows the preferred batch size
     * to be encapsulated with the client.
     *
     * @return the batch size to use
     */
    virtual string getBatchSize() = 0;

    /**
     * Batch scoring method
     *
     * @param insts the instances to get predictions for
     * @return an array of probability distributions, one for each instance
     *
     * @exception Exception if a problem occurs
     */
    virtual double_2D_array distributionsForInstances(Instances *insts) = 0;

    /**
     * Returns true if this BatchPredictor can generate batch predictions
     * in an efficient manner.
     *
     * @return true if batch predictions can be generated efficiently
     */
    virtual bool implementsMoreEfficientBatchPrediction() = 0;
};
#endif    //#ifndef _BATCHPREDICTOR_
