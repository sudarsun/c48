#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_

#include <string>
#include <vector>
#include <stdexcept>
#include "core/Typedefs.h"

class Instances;
class ClassifierSplitModel;
class Instance;


/**
 * Class for handling a distribution of class values.
 *
 */
class Distribution
{

protected:

    /** Weight of instances per class per bag. */
    double_2D_array mperClassPerBag;

    /** Weight of instances per bag. */
    double_array mperBag;

    /** Weight of instances per class. */
    double_array mperClass;

    /** Total weight of instances. */
    double totaL = 0;

public:

    /**
     * Creates and initializes a new distribution.
     */
    Distribution(int numBags, int numClasses);

    /**
     * Creates and initializes a new distribution using the given
     * array. WARNING: it just copies a reference to this array.
     */
    Distribution(double_2D_array &table);

    /**
 * Creates a distribution with only one bag according
 * to instances in source.
 *
 * @exception Exception if something goes wrong
 */
    Distribution(Instances &source);

    /**
     * Creates a distribution according to given instances and
     * split model.
     *
     * @exception Exception if something goes wrong
     */
    Distribution(Instances &source, ClassifierSplitModel &modelToUse);

    /**
     * Creates distribution with only one bag by merging all
     * bags of given distribution.
     */
    Distribution(Distribution *toMerge);

    /**
     * Creates distribution with two bags by merging all bags apart of
     * the indicated one.
     */
    Distribution(Distribution *toMerge, int index);

    /**
     * Returns number of non-empty bags of distribution.
     */
    int actualNumBags() const;

    /**
     * Returns number of classes actually occuring in distribution.
     */
    int actualNumClasses() const;

    /**
     * Returns number of classes actually occuring in given bag.
     */
    int actualNumClasses(int bagIndex) const;

    /**
     * Adds given instance to given bag.
     *
     * @exception Exception if something goes wrong
     */
    void add(int bagIndex, Instance &instance);

    /**
     * Subtracts given instance from given bag.
     *
     * @exception Exception if something goes wrong
     */
    void sub(int bagIndex, Instance &instance);

    /**
     * Adds counts to given bag.
     */
    void add(int bagIndex, double_array &counts);

    /**
     * Adds all instances with unknown values for given attribute, weighted
     * according to frequency of instances in each bag.
     *
     * @exception Exception if something goes wrong
     */
    void addInstWithUnknown(Instances &source, int attIndex);

    /**
     * Adds all instances in given range to given bag.
     *
     * @exception Exception if something goes wrong
     */
    void addRange(int bagIndex, Instances &source, int startIndex, int lastPlusOne);

    /**
     * Adds given instance to all bags weighting it according to given weights.
     *
     * @exception Exception if something goes wrong
     */
    void addWeights(Instance &instance, double_array &weights);

    /**
     * Checks if at least two bags contain a minimum number of instances.
     */
    bool check(double minNoObj) const;

    /**
     * Deletes given instance from given bag.
     *
     * @exception Exception if something goes wrong
     */
    void del(int bagIndex, Instance &instance);

    /**
     * Deletes all instances in given range from given bag.
     *
     * @exception Exception if something goes wrong
     */
    void delRange(int bagIndex, Instances &source, int startIndex, int lastPlusOne);

    /**
     * Prints distribution.
     */
    string dumpDistribution() const;

    /**
     * Sets all counts to zero.
     */
    void initialize();

    /**
     * Returns matrix with distribution of class values.
     */
    double_2D_array matrix();

    /**
     * Returns index of bag containing maximum number of instances.
     */
    int maxBag() const;

    /**
     * Returns class with highest frequency over all bags.
     */
    int maxClass() const;

    /**
     * Returns class with highest frequency for given bag.
     */
    int maxClass(int index) const;

    /**
     * Returns number of bags.
     */
    int numBags() const;

    /**
     * Returns number of classes.
     */
    int numClasses() const;

    /**
     * Returns perClass(maxClass()).
     */
    double numCorrect() const;

    /**
     * Returns perClassPerBag(index,maxClass(index)).
     */
    double numCorrect(int index) const;

    /**
     * Returns total-numCorrect().
     */
    double numIncorrect() const;

    /**
     * Returns perBag(index)-numCorrect(index).
     */
    double numIncorrect(int index) const;

    /**
     * Returns number of (possibly fractional) instances of given class in
     * given bag.
     */
    double perClassPerBag(int bagIndex, int classIndex) const;

    /**
     * Returns number of (possibly fractional) instances in given bag.
     */
    double perBag(int bagIndex) const;

    /**
     * Returns number of (possibly fractional) instances of given class.
     */
    double perClass(int classIndex) const;

    /**
     * Returns relative frequency of class over all bags with
     * Laplace correction.
     */
    double laplaceProb(int classIndex) const;

    /**
     * Returns relative frequency of class for given bag.
     */
    double laplaceProb(int classIndex, int intIndex) const;

    /**
     * Returns relative frequency of class over all bags.
     */
    double prob(int classIndex) const;

    /**
     * Returns relative frequency of class for given bag.
     */
    double prob(int classIndex, int intIndex) const;

    /**
     * Subtracts the given distribution from this one. The results
     * has only one bag.
     */
    Distribution *subtract(Distribution *toSubstract) const;

    /**
     * Returns total number of (possibly fractional) instances.
     */
    double total() const;

    /**
     * Shifts given instance from one bag to another one.
     *
     * @exception Exception if something goes wrong
     */
    void shift(int from, int to, Instance &instance);

    /**
     * Shifts all instances in given range from one bag to another one.
     *
     * @exception Exception if something goes wrong
     */
    void shiftRange(int from, int to, Instances &source, int startIndex, int lastPlusOne);

};

#endif    // _DISTRIBUTION_
