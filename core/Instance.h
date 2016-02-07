#ifndef _INSTANCE_
#define _INSTANCE_

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "Attribute.h"

class Instances;

/**
 * Class for handling an instance. All values (numeric, date, nominal, string
 * or relational) are internally stored as floating-point numbers. If an
 * attribute is nominal (or a string or relational), the stored value is the
 * index of the corresponding nominal (or string or relational) value in the
 * attribute's definition. We have chosen this approach in favor of a more
 * elegant object-oriented approach because it is much faster.
 *
 */
class Instance
{

public:
  /**
   * Constructor that copies the attribute values and the weight from
   * the given instance. Reference to the dataset is set to null.
   * (ie. the instance doesn't have access to information about the
   * attribute types)
   *
   * @param instance the instance from which the attribute
   * values and the weight are to be copied 
   */
    Instance(Instance *instance);
	
  /**
   * Constructor that inititalizes instance variable with given
   * values. Reference to the dataset is set to null. (ie. the instance
   * doesn't have access to information about the attribute types)
   *
   * @param weight the instance's weight
   * @param attValues a vector of attribute values 
   */
    Instance(const double weight, const double_array &attValues);

  /**
   * Returns the attribute with the given index.
   *
   * @param index the attribute's index
   * @return the attribute at the given position
   * @throws UnassignedDatasetException if instance doesn't have access to a
   * dataset
   */ 
  //@ requires mDataset != null;
    Attribute &attribute(const int index) const;
	
  /**
   * Returns the class attribute's index.
   *
   * @return the class index as an integer 
   * @throws UnassignedDatasetException if instance doesn't have access to a dataset 
   */
  //@ requires mDataset != null;
  //@ ensures  \result == mDataset.classIndex();
  int classIndex() const;
  
  /**
   * Returns the number of attributes.
   *
   * @return the number of attributes as an integer
   */
  //@ ensures \result == mAttValues.length;
  int numAttributes() const;

  /**
   * Sets the class value of an instance to be "missing". A deep copy of
   * the vector of attribute values is performed before the
   * value is set to be missing.
   *
   * @throws UnassignedClassException if the class is not set
   * @throws UnassignedDatasetException if the instance doesn't
   * have access to a dataset
   */
  //@ requires classIndex() >= 0;
  void setClassMissing();
  
  /**
   * Sets a specific value to be "missing". Performs a deep copy
   * of the vector of attribute values before the value is set to
   * be missing.
   *
   * @param attIndex the attribute's index
   */
   void setMissing(const int attIndex);
   
  /**
   * Sets a value of a nominal or string attribute to the given
   * value. Performs a deep copy of the vector of attribute values
   * before the value is set.
   *
   * @param attIndex the attribute's index
   * @param value the new attribute value (If the attribute
   * is a string attribute and the value can't be found,
   * the value is added to the attribute).
   * @throws UnassignedDatasetException if the dataset is not set
   * @throws IllegalArgumentException if the selected
   * attribute is not nominal or a string, or the supplied value couldn't 
   * be found for a nominal attribute 
   */
  //@ requires mDataset != null;	
  void setValue(const int attIndex, const string &value);
  
  /**
   * Sets a specific value in the instance to the given value 
   * (internal floating-point format). Performs a deep copy
   * of the vector of attribute values before the value is set.
   *
   * @param attIndex the attribute's index 
   * @param value the new attribute value (If the corresponding
   * attribute is nominal (or a string) then this is the new value's
   * index as a double).  
   */
   void setValue(const int attIndex, const double value);
   
  /**
   * Sets the reference to the dataset. Does not check if the instance
   * is compatible with the dataset. Note: the dataset does not know
   * about this instance. If the structure of the dataset's header
   * gets changed, this instance will not be adjusted automatically.
   *
   * @param instances the reference to the dataset 
   */
   void setDataset(Instances* data);
   
  /**
   * Returns the dataset this instance has access to. (ie. obtains
   * information about attribute types from) Null if the instance
   * doesn't have access to a dataset.
   *
   * @return the dataset the instance has accesss to
   */
  //@ ensures \result == mDataset;
  Instances* getDataset() const;
  
  /**
   * Returns an instance's class value in internal format. (ie. as a
   * floating-point number)
   *
   * @return the corresponding value as a double (If the 
   * corresponding attribute is nominal (or a string) then it returns the 
   * value's index as a double).
   * @throws UnassignedClassException if the class is not set or the instance doesn't
   * have access to a dataset 
   */
  //@ requires classIndex() >= 0;	
    double classValue() const;
  /**
   * Returns the instance's weight.
   *
   * @return the instance's weight as a double
   */
   double weight() const;
  /**
   * Returns an instance's attribute value in internal format.
   *
   * @param attIndex the attribute's index
   * @return the specified value as a double (If the corresponding
   * attribute is nominal (or a string) then it returns the value's index as a 
   * double).
   */
   double value(const int attIndex) const;

  /**
   * Returns the values of each attribute as an array of doubles.
   *
   * @return an array containing all the instance attribute values
   */
   double_array toDoubleArray() const;
   
  /**
   * Returns class attribute.
   *
   * @return the class attribute
   * @throws UnassignedDatasetException if the class is not set or the
   * instance doesn't have access to a dataset
   */
  //@ requires mDataset != null;
  Attribute &classAttribute() const;
  
  /**
   * Returns the number of class labels.
   *
   * @return the number of class labels as an integer if the 
   * class attribute is nominal, 1 otherwise.
   * @throws UnassignedDatasetException if instance doesn't have access to any
   * dataset
   */
  //@ requires mDataset != null;
  int numClasses() const;
  /**
   * Tests if a specific value is "missing".
   *
   * @param attIndex the attribute's index
   * @return true if the value is "missing"
   */
   bool isMissing(const int attIndex) const;
  /**
   * Sets the weight of an instance.
   *
   * @param weight the weight
   */
   void setWeight(const double weight);

  /**
   * Returns the double that codes "missing".
   *
   * @return the double that codes "missing"
   */
   double missingValue() const;
   /**
   * Tests if an instance's class is missing.
   *
   * @return true if the instance's class is missing
   * @throws UnassignedClassException if the class is not set or the instance doesn't
   * have access to a dataset
   */
  //@ requires classIndex() >= 0;
    bool classIsMissing() const;
  
  /**
   * Produces a shallow copy of this instance. The copy has
   * access to the same dataset. (if you want to make a copy
   * that doesn't have access to the dataset, use 
   * <code>new Instance(instance)</code>
   *
   * @return the shallow copy
   */
   Instance* copy();

protected:

	/** The instance's attribute values. */
    double_array mAttValues;

	/** The instance's weight. */
    double mWeight;
	/** 
	 * The dataset the instance has access to.  Null if the instance
	* doesn't have access to any dataset.  Only if an instance has
	* access to a dataset, it knows about the actual attribute types.  
	*/
    Instances *mDataset;

private:

  /**
   * Clones the attribute vector of the instance and
   * overwrites it with the clone.
   */
   void freshAttributeVector();

};

#endif    // _INSTANCE_
