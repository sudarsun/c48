#ifndef _INSTANCES_HPP
#define _INSTANCES_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include <fstream>

#include "Attribute.h"
#include "AttributeStats.h"
#include "Instance.h"

/**
 * Class for handling an ordered set of weighted instances.
 *
 */
class Instances
{

protected:

    /** The dataset's name. */
    string mRelationName;

    /** The attribute information. */
    std::vector<Attribute*> mAttributes;

    /** A map to quickly find attribute indices based on their names. */
    std::unordered_map<string, int> mNamesToAttributeIndices;

    /** The instances. */
    std::vector<Instance*> mInstances;

    /** The class attribute's index */
    int mClassIndex;

    /**
     * initializes with the header information of the given dataset and sets the
     * capacity of the set of instances.
     *
     * @param dataset the dataset to use as template
     * @param capacity the number of rows to reserve
     */
    void initialize(Instances *dataset, int capacity);

public:
    /**
     * Creates an empty set of instances. Uses the given attribute information.
     * Sets the capacity of the set of instances to 0 if its negative. Given
     * attribute information must not be changed after this constructor has been
     * used.
     *
     * @param name the name of the relation
     * @param attInfo the attribute information
     * @param capacity the capacity of the set
     */
    Instances(const string &name, std::vector<Attribute*> &attInfo, int capacity);

    /**
     * Constructor copying all instances and references to the header information
     * from the given set of instances.
     *
     * @param dataset the set to be copied
     */
    Instances(Instances *dataset);

    /**
     * Constructor creating an empty set of instances. Copies references to the
     * header information from the given set of instances. Sets the capacity of
     * the set of instances to 0 if its negative.
     *
     * @param dataset the instances from which the header information is to be
     *          taken
     * @param capacity the capacity of the new dataset
     */
    Instances(Instances *dataset, int capacity);

    /**
     * Returns an attribute.
     *
     * @param index the attribute's index (index starts with 0)
     * @return the attribute at the given position
     */
     // @ requires 0 <= index;
     // @ requires index < mAttributes.size();
     // @ ensures \result != null;  
    Attribute *attribute(int index);

    /**
     * Returns an attribute given its name. If there is more than one attribute
     * with the same name, it returns the first one. Returns null if the attribute
     * can't be found.
     *
     * @param name the attribute's name
     * @return the attribute with the given name, null if the attribute can't be
     *         found
     */
    Attribute *attribute(const string &name);

    /**
     * Returns the number of attributes.
     *
     * @return the number of attributes as an integer
     */
     // @ ensures \result == mAttributes.size();
    int numAttributes();

    /**
     * Returns the class attribute's index. Returns negative number if it's
     * undefined.
     *
     * @return the class index as an integer
     */
     // ensures \result == mClassIndex;
    int classIndex();

    /**
     * Adds one instance to the end of the set. Shallow copies instance before it
     * is added. Increases the size of the dataset if it is not large enough. Does
     * not check if the instance is compatible with the dataset. Note: String or
     * relational values are not transferred.
     *
     * @param instance the instance to be added
     */
    bool add(Instance *instance);

    /**
     * Adds one instance to the given index.
     *
     * @param index the index
     * @param instance the instance to be added
     */
    void add(int index, Instance *instance);

    /**
     * Sets the class index of the set. If the class index is negative there is
     * assumed to be no class. (ie. it is undefined)
     *
     * @param classIndex the new class index (index starts with 0)
     * @throws IllegalArgumentException if the class index is too big or < 0
     */
    void setClassIndex(int classIndex);

    /**
     * Returns the number of instances in the dataset.
     *
     * @return the number of instances in the dataset as an integer
     */
     // @ ensures \result == mInstances.size();
    int numInstances();

    /**
     * Copies instances from one set to the end of another one.
     *
     * @param from the position of the first instance to be copied
     * @param dest the destination for the instances
     * @param num the number of instances to be copied
     */
     // @ requires 0 <= from && from <= numInstances() - num;
     // @ requires 0 <= num;
    void copyInstances(int from, Instances *dest, int num);

    /**
     * Returns the instance at the given position.
     *
     * @param index the instance's index (index starts with 0)
     * @return the instance at the given position
     */
     // @ requires 0 <= index;
     // @ requires index < numInstances();
    Instance *instance(int index);

    /**
     * Returns the class attribute.
     *
     * @return the class attribute
     * @throws UnassignedClassException if the class is not set
     */
     // @ requires classIndex() >= 0;	
    Attribute *classAttribute();

    /**
     * Returns the number of class labels.
     *
     * @return the number of class labels as an integer if the class attribute is
     *         nominal, 1 otherwise.
     * @throws UnassignedClassException if the class is not set
     */
     // @ requires classIndex() >= 0;
    int numClasses();

    /**
     * Returns the last instance in the set.
     *
     * @return the last instance in the set
     */
     // @ requires numInstances() > 0;
    Instance *lastInstance();

    /**
     * Computes the sum of all the instances' weights.
     *
     * @return the sum of all the instances' weights as a double
     */
    double sumOfWeights();

    /**
     * Sorts the instances based on an attribute. For numeric attributes,
     * instances are sorted in ascending order. For nominal attributes, instances
     * are sorted based on the attribute label ordering specified in the header.
     * Instances with missing values for the attribute are placed at the end of
     * the dataset.
     *
     * @param attIndex the attribute's index (index starts with 0)
     */
    void Sort(int attIndex);

    /**
     * Sorts the instances based on an attribute. For numeric attributes,
     * instances are sorted into ascending order. For nominal attributes,
     * instances are sorted based on the attribute label ordering specified in the
     * header. Instances with missing values for the attribute are placed at the
     * end of the dataset.
     *
     * @param att the attribute
     */
    void Sort(Attribute *att);

    /**
     * Sorts a nominal attribute (stable, linear-time sort). Instances
     * are sorted based on the attribute label ordering specified in the header.
     *
     * @param attIndex the attribute's index (index starts with 0)
     */
    void sortBasedOnNominalAttribute(int attIndex);

    /**
     * Removes all instances with a missing class value from the dataset.
     *
     * @throws UnassignedClassException if class is not set
     */
    void deleteWithMissingClass();

    /**
     * Removes all instances with missing values for a particular attribute from
     * the dataset.
     *
     * @param att the attribute
     */
    void deleteWithMissing(Attribute *att);

    /**
     * Removes all instances with missing values for a particular attribute from
     * the dataset.
     *
     * @param attIndex the attribute's index (index starts with 0)
     */
     // @ requires 0 <= attIndex && attIndex < numAttributes();
    void deleteWithMissing(int attIndex);

    /**
     * Creates the training set for one fold of a cross-validation on the dataset.
     *
     * @param numFolds the number of folds in the cross-validation. Must be
     *          greater than 1.
     * @param numFold 0 for the first fold, 1 for the second, ...
     * @return the training set
     * @throws IllegalArgumentException if the number of folds is less than 2 or
     *           greater than the number of instances.
     */
     // @ requires 2 <= numFolds && numFolds < numInstances();
     // @ requires 0 <= numFold && numFold < numFolds;
    Instances *trainCV(int numFolds, int numFold);

    /**
     * Creates the test set for one fold of a cross-validation on the dataset.
     *
     * @param numFolds the number of folds in the cross-validation. Must be
     *          greater than 1.
     * @param numFold 0 for the first fold, 1 for the second, ...
     * @return the test set as a set of weighted instances
     * @throws IllegalArgumentException if the number of folds is less than 2 or
     *           greater than the number of instances.
     */
     // @ requires 2 <= numFolds && numFolds < numInstances();
     // @ requires 0 <= numFold && numFold < numFolds;
    Instances *testCV(int numFolds, int numFold);

    /**
     * Returns the relation's name.
     *
     * @return the relation's name as a string
     */
     // @ ensures \result == mRelationName;
    string getRelationName();

    /**
     * Sets the relation's name.
     *
     * @param newName the new relation name.
     */
    void setRelationName(const string name);

    /**
     * Gets the value of all instances in this dataset for a particular attribute.
     * Useful in conjunction with Utils.sort to allow iterating through the
     * dataset in sorted order for some attribute.
     *
     * @param index the index of the attribute.
     * @return an array containing the value of the desired attribute for each
     *         instance in the dataset.
     */
     // @ requires 0 <= index && index < numAttributes();
    double_array attributeToDoubleArray(int index);

};

#endif // _INSTANCES_HPP
