#ifndef _SPLITCRITERION_
#define _SPLITCRITERION_


// Forward class declarations:
class Distribution;

/**
 * Abstract class for computing splitting criteria
 * with respect to distributions of class values.
 *
 */
class SplitCriterion {

public:
    /**
     * Computes result of splitting criterion for given distribution.
     *
     * @return value of splitting criterion. 0 by default
     */
    virtual double splitCritValue(Distribution &bags) const;

    /**
     * Computes result of splitting criterion for given training and
     * test distributions.
     *
     * @return value of splitting criterion. 0 by default
     */
    virtual double splitCritValue(Distribution &train, Distribution &test) const;

    /**
     * Computes result of splitting criterion for given training and
     * test distributions and given number of classes.
     *
     * @return value of splitting criterion. 0 by default
     */
    virtual double splitCritValue(Distribution &train, Distribution &test, int noClassesDefault) const;

    /**
     * Computes result of splitting criterion for given training and
     * test distributions and given default distribution.
     *
     * @return value of splitting criterion. 0 by default
     */
    virtual double splitCritValue(Distribution &train, Distribution &test, Distribution &defC) const;
};


#endif    //#ifndef _SPLITCRITERION_
