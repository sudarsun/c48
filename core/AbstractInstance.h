#ifndef _ABSTRACTINSTANCE_
#define _ABSTRACTINSTANCE_

#include "Instance.h"
#include "RevisionHandler.h"
#include <string>
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Instances;
class Attribute;
class StringBuffer;

/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *    AbstractInstance.java
 *    Copyright (C) 1999-2012 University of Waikato, Hamilton, New Zealand
 *
 */

namespace weka {
    namespace core {


        /// <summary>
        /// Abstract class providing common functionality for the original instance
        /// implementations.
        /// 
        /// @author Eibe Frank (eibe@cs.waikato.ac.nz)
        /// @version $Revision: 10649 $
        /// </summary>
        class AbstractInstance : public Instance, public Serializable, public RevisionHandler {

          /// <summary>
          /// for serialization </summary>
      public:
          static const long long serialVersionUID = 1482635194499365155LL;

          /// <summary>
          /// The dataset the instance has access to. Null if the instance doesn't have
          /// access to any dataset. Only if an instance has access to a dataset, it
          /// knows about the actual attribute types.
          /// </summary>
      protected:
          Instances *m_Dataset; // @spec_public@

          /// <summary>
          /// The instance's attribute values. </summary>
          std::vector<double> m_AttValues; // @spec_public non_null@

          /// <summary>
          /// The instance's weight. </summary>
          double m_Weight = 0;

          /// <summary>
          /// Default max number of digits after the decimal point for numeric values </summary>
      public:
          static int s_numericAfterDecimalPoint;

          /// <summary>
          /// Returns the attribute with the given index.
          /// </summary>
          /// <param name="index"> the attribute's index </param>
          /// <returns> the attribute at the given position </returns>
          /// <exception cref="UnassignedDatasetException"> if instance doesn't have access to a
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          virtual Attribute *attribute( int index ) override;

          /// <summary>
          /// Returns the attribute with the given index in the sparse representation.
          /// </summary>
          /// <param name="indexOfIndex"> the index of the attribute's index </param>
          /// <returns> the attribute at the given position </returns>
          /// <exception cref="UnassignedDatasetException"> if instance doesn't have access to a
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          virtual Attribute *attributeSparse( int indexOfIndex ) override;

          /// <summary>
          /// Returns class attribute.
          /// </summary>
          /// <returns> the class attribute </returns>
          /// <exception cref="UnassignedDatasetException"> if the class is not set or the instance
          ///           doesn't have access to a dataset </exception>
          // @ requires m_Dataset != null;
          virtual Attribute *classAttribute() override;

          /// <summary>
          /// Returns the class attribute's index.
          /// </summary>
          /// <returns> the class index as an integer </returns>
          /// <exception cref="UnassignedDatasetException"> if instance doesn't have access to a
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          // @ ensures \result == m_Dataset.classIndex();
          virtual int classIndex() override;

          /// <summary>
          /// Tests if an instance's class is missing.
          /// </summary>
          /// <returns> true if the instance's class is missing </returns>
          /// <exception cref="UnassignedClassException"> if the class is not set or the instance
          ///           doesn't have access to a dataset </exception>
          // @ requires classIndex() >= 0;
          virtual bool classIsMissing() override;

          /// <summary>
          /// Returns an instance's class value in internal format. (ie. as a
          /// floating-point number)
          /// </summary>
          /// <returns> the corresponding value as a double (If the corresponding attribute
          ///         is nominal (or a string) then it returns the value's index as a
          ///         double). </returns>
          /// <exception cref="UnassignedClassException"> if the class is not set or the instance
          ///           doesn't have access to a dataset </exception>
          // @ requires classIndex() >= 0;
          virtual double classValue() override;

          /// <summary>
          /// Returns the dataset this instance has access to. (ie. obtains information
          /// about attribute types from) Null if the instance doesn't have access to a
          /// dataset.
          /// </summary>
          /// <returns> the dataset the instance has accesss to </returns>
          // @ ensures \result == m_Dataset;
          virtual Instances *dataset() override;

          /// <summary>
          /// Deletes an attribute at the given position (0 to numAttributes() - 1). Only
          /// succeeds if the instance does not have access to any dataset because
          /// otherwise inconsistencies could be introduced.
          /// </summary>
          /// <param name="position"> the attribute's position </param>
          /// <exception cref="RuntimeException"> if the instance has access to a dataset </exception>
          // @ requires m_Dataset != null;
          virtual void deleteAttributeAt( int position ) override;

          /// <summary>
          /// Returns an enumeration of all the attributes.
          /// </summary>
          /// <returns> enumeration of all the attributes </returns>
          /// <exception cref="UnassignedDatasetException"> if the instance doesn't have access to a
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          virtual java::util::Iterator<Attribute*> *enumerateAttributes() override;

          /// <summary>
          /// Tests if the headers of two instances are equivalent.
          /// </summary>
          /// <param name="inst"> another instance </param>
          /// <returns> true if the header of the given instance is equivalent to this
          ///         instance's header </returns>
          /// <exception cref="UnassignedDatasetException"> if instance doesn't have access to any
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          virtual bool equalHeaders( Instance *inst ) override;

          /// <summary>
          /// Checks if the headers of two instances are equivalent. If not, then returns
          /// a message why they differ.
          /// </summary>
          /// <param name="dataset"> another instance </param>
          /// <returns> null if the header of the given instance is equivalent to this
          ///         instance's header, otherwise a message with details on why they
          ///         differ </returns>
          virtual std::wstring equalHeadersMsg( Instance *inst ) override;

          /// <summary>
          /// Tests whether an instance has a missing value. Skips the class attribute if
          /// set.
          /// </summary>
          /// <returns> true if instance has a missing value. </returns>
          /// <exception cref="UnassignedDatasetException"> if instance doesn't have access to any
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          virtual bool hasMissingValue() override;

          /// <summary>
          /// Inserts an attribute at the given position (0 to numAttributes()). Only
          /// succeeds if the instance does not have access to any dataset because
          /// otherwise inconsistencies could be introduced.
          /// </summary>
          /// <param name="position"> the attribute's position </param>
          /// <exception cref="RuntimeException"> if the instance has accesss to a dataset </exception>
          /// <exception cref="IllegalArgumentException"> if the position is out of range </exception>
          // @ requires m_Dataset == null;
          // @ requires 0 <= position && position <= numAttributes();
          virtual void insertAttributeAt( int position ) override;

          /// <summary>
          /// Tests if a specific value is "missing".
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          /// <returns> true if the value is "missing" </returns>
          virtual bool isMissing( int attIndex ) override;

          /// <summary>
          /// Tests if a specific value is "missing", given an index in the sparse
          /// representation.
          /// </summary>
          /// <param name="indexOfIndex"> the index of the attribute's index </param>
          /// <returns> true if the value is "missing" </returns>
          virtual bool isMissingSparse( int indexOfIndex ) override;

          /// <summary>
          /// Tests if a specific value is "missing". The given attribute has to belong
          /// to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <returns> true if the value is "missing" </returns>
          virtual bool isMissing( Attribute *att ) override;

          /// <summary>
          /// Returns the number of class labels.
          /// </summary>
          /// <returns> the number of class labels as an integer if the class attribute is
          ///         nominal, 1 otherwise. </returns>
          /// <exception cref="UnassignedDatasetException"> if instance doesn't have access to any
          ///           dataset </exception>
          // @ requires m_Dataset != null;
          virtual int numClasses() override;

          /// <summary>
          /// Sets the class value of an instance to be "missing". A deep copy of the
          /// vector of attribute values is performed before the value is set to be
          /// missing.
          /// </summary>
          /// <exception cref="UnassignedClassException"> if the class is not set </exception>
          /// <exception cref="UnassignedDatasetException"> if the instance doesn't have access to a
          ///           dataset </exception>
          // @ requires classIndex() >= 0;
          virtual void setClassMissing() override;

          /// <summary>
          /// Sets the class value of an instance to the given value (internal
          /// floating-point format). A deep copy of the vector of attribute values is
          /// performed before the value is set.
          /// </summary>
          /// <param name="value"> the new attribute value (If the corresponding attribute is
          ///          nominal (or a string) then this is the new value's index as a
          ///          double). </param>
          /// <exception cref="UnassignedClassException"> if the class is not set </exception>
          /// <exception cref="UnaddignedDatasetException"> if the instance doesn't have access to a
          ///           dataset </exception>
          // @ requires classIndex() >= 0;
          virtual void setClassValue( double value ) override;

          /// <summary>
          /// Sets the class value of an instance to the given value. A deep copy of the
          /// vector of attribute values is performed before the value is set.
          /// </summary>
          /// <param name="value"> the new class value (If the class is a string attribute and
          ///          the value can't be found, the value is added to the attribute). </param>
          /// <exception cref="UnassignedClassException"> if the class is not set </exception>
          /// <exception cref="UnassignedDatasetException"> if the dataset is not set </exception>
          /// <exception cref="IllegalArgumentException"> if the attribute is not nominal or a
          ///           string, or the value couldn't be found for a nominal attribute </exception>
          // @ requires classIndex() >= 0;
          void setClassValue( const std::wstring &value ) override;

          /// <summary>
          /// Sets the reference to the dataset. Does not check if the instance is
          /// compatible with the dataset. Note: the dataset does not know about this
          /// instance. If the structure of the dataset's header gets changed, this
          /// instance will not be adjusted automatically.
          /// </summary>
          /// <param name="instances"> the reference to the dataset </param>
          void setDataset( Instances *instances ) override;

          /// <summary>
          /// Sets a specific value to be "missing". Performs a deep copy of the vector
          /// of attribute values before the value is set to be missing.
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          void setMissing( int attIndex ) override;

          /// <summary>
          /// Sets a specific value to be "missing". Performs a deep copy of the vector
          /// of attribute values before the value is set to be missing. The given
          /// attribute has to belong to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          void setMissing( Attribute *att ) override;

          /// <summary>
          /// Sets a value of a nominal or string attribute to the given value. Performs
          /// a deep copy of the vector of attribute values before the value is set.
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          /// <param name="value"> the new attribute value (If the attribute is a string
          ///          attribute and the value can't be found, the value is added to the
          ///          attribute). </param>
          /// <exception cref="UnassignedDatasetException"> if the dataset is not set </exception>
          /// <exception cref="IllegalArgumentException"> if the selected attribute is not nominal
          ///           or a string, or the supplied value couldn't be found for a
          ///           nominal attribute </exception>
          // @ requires m_Dataset != null;
          void setValue( int attIndex, const std::wstring &value ) override;

          /// <summary>
          /// Sets a specific value in the instance to the given value (internal
          /// floating-point format). Performs a deep copy of the vector of attribute
          /// values before the value is set, so if you are planning on calling setValue
          /// many times it may be faster to create a new instance using toDoubleArray.
          /// The given attribute has to belong to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <param name="value"> the new attribute value (If the corresponding attribute is
          ///          nominal (or a string) then this is the new value's index as a
          ///          double). </param>
          void setValue( Attribute *att, double value ) override;

          /// <summary>
          /// Sets a value of an nominal or string attribute to the given value. Performs
          /// a deep copy of the vector of attribute values before the value is set, so
          /// if you are planning on calling setValue many times it may be faster to
          /// create a new instance using toDoubleArray. The given attribute has to
          /// belong to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <param name="value"> the new attribute value (If the attribute is a string
          ///          attribute and the value can't be found, the value is added to the
          ///          attribute). </param>
          /// <exception cref="IllegalArgumentException"> if the the attribute is not nominal or a
          ///           string, or the value couldn't be found for a nominal attribute </exception>
          void setValue( Attribute *att, const std::wstring &value ) override;

          /// <summary>
          /// Sets the weight of an instance.
          /// </summary>
          /// <param name="weight"> the weight </param>
          void setWeight( double weight ) override;

          /// <summary>
          /// Returns the relational value of a relational attribute.
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          /// <returns> the corresponding relation as an Instances object </returns>
          /// <exception cref="IllegalArgumentException"> if the attribute is not a relation-valued
          ///           attribute </exception>
          /// <exception cref="UnassignedDatasetException"> if the instance doesn't belong to a
          ///           dataset. </exception>
          // @ requires m_Dataset != null;
          Instances *relationalValue( int attIndex ) override;

          /// <summary>
          /// Returns the relational value of a relational attribute.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <returns> the corresponding relation as an Instances object, null if missing </returns>
          /// <exception cref="IllegalArgumentException"> if the attribute is not a relation-valued
          ///           attribute </exception>
          /// <exception cref="UnassignedDatasetException"> if the instance doesn't belong to a
          ///           dataset. </exception>
          Instances *relationalValue( Attribute *att ) override;

          /// <summary>
          /// Returns the value of a nominal, string, date, or relational attribute for
          /// the instance as a string.
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          /// <returns> the value as a string </returns>
          /// <exception cref="IllegalArgumentException"> if the attribute is not a nominal, string,
          ///           date, or relation-valued attribute. </exception>
          /// <exception cref="UnassignedDatasetException"> if the instance doesn't belong to a
          ///           dataset. </exception>
          // @ requires m_Dataset != null;
          std::wstring stringValue( int attIndex ) override;

          /// <summary>
          /// Returns the value of a nominal, string, date, or relational attribute for
          /// the instance as a string.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <returns> the value as a string </returns>
          /// <exception cref="IllegalArgumentException"> if the attribute is not a nominal, string,
          ///           date, or relation-valued attribute. </exception>
          /// <exception cref="UnassignedDatasetException"> if the instance doesn't belong to a
          ///           dataset. </exception>
          std::wstring stringValue( Attribute *att ) override;

          /// <summary>
          /// Returns the description of one instance with any numeric values printed at
          /// the supplied maximum number of decimal places. If the instance doesn't have
          /// access to a dataset, it returns the internal floating-point values. Quotes
          /// string values that contain whitespace characters.
          /// </summary>
          /// <param name="afterDecimalPoint"> the maximum number of digits permitted after the
          ///          decimal point for a numeric value
          /// </param>
          /// <returns> the instance's description as a string </returns>
          std::wstring toStringMaxDecimalDigits( int afterDecimalPoint ) override;

          /// <summary>
          /// Returns the description of one instance. If the instance doesn't have
          /// access to a dataset, it returns the internal floating-point values. Quotes
          /// string values that contain whitespace characters.
          /// </summary>
          /// <returns> the instance's description as a string </returns>
          virtual std::wstring toString() override;

          /// <summary>
          /// Returns the description of one value of the instance as a string. If the
          /// instance doesn't have access to a dataset, it returns the internal
          /// floating-point value. Quotes string values that contain whitespace
          /// characters, or if they are a question mark.
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          /// <returns> the value's description as a string </returns>
          std::wstring toString( int attIndex ) override;

          /// <summary>
          /// Returns the description of one value of the instance as a string. If the
          /// instance doesn't have access to a dataset, it returns the internal
          /// floating-point value. Quotes string values that contain whitespace
          /// characters, or if they are a question mark.
          /// </summary>
          /// <param name="attIndex"> the attribute's index </param>
          /// <param name="afterDecimalPoint"> the maximum number of digits permitted after the
          ///          decimal point for numeric values </param>
          /// <returns> the value's description as a string </returns>
          std::wstring toString( int attIndex, int afterDecimalPoint ) override;

          /// <summary>
          /// Returns the description of one value of the instance as a string. If the
          /// instance doesn't have access to a dataset it returns the internal
          /// floating-point value. Quotes string values that contain whitespace
          /// characters, or if they are a question mark. The given attribute has to
          /// belong to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <returns> the value's description as a string </returns>
          std::wstring toString( Attribute *att ) override;

          /// <summary>
          /// Returns the description of one value of the instance as a string. If the
          /// instance doesn't have access to a dataset it returns the internal
          /// floating-point value. Quotes string values that contain whitespace
          /// characters, or if they are a question mark. The given attribute has to
          /// belong to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <param name="afterDecimalPoint"> the maximum number of decimal places to print </param>
          /// <returns> the value's description as a string </returns>
          std::wstring toString( Attribute *att, int afterDecimalPoint ) override;

          /// <summary>
          /// Returns an instance's attribute value in internal format. The given
          /// attribute has to belong to a dataset.
          /// </summary>
          /// <param name="att"> the attribute </param>
          /// <returns> the specified value as a double (If the corresponding attribute is
          ///         nominal (or a string) then it returns the value's index as a
          ///         double). </returns>
          virtual double value( Attribute *att ) override;

          /// <summary>
          /// Returns an instance's attribute value in internal format, given an index in
          /// the sparse representation.
          /// </summary>
          /// <param name="indexOfIndex"> the index of the attribute's index </param>
          /// <returns> the specified value as a double (If the corresponding attribute is
          ///         nominal (or a string) then it returns the value's index as a
          ///         double). </returns>
          virtual double valueSparse( int indexOfIndex ) override;

          /// <summary>
          /// Returns the instance's weight.
          /// </summary>
          /// <returns> the instance's weight as a double </returns>
          double weight() override;

          /// <summary>
          /// Returns the revision string.
          /// </summary>
          /// <returns> the revision </returns>
          virtual std::wstring getRevision() override;

          /// <summary>
          /// Deletes an attribute at the given position (0 to numAttributes() - 1).
          /// </summary>
          /// <param name="position"> the attribute's position </param>
      protected:
          virtual void forceDeleteAttributeAt( int position ) = 0;

          /// <summary>
          /// Inserts an attribute at the given position (0 to numAttributes()) and sets
          /// its value to be missing.
          /// </summary>
          /// <param name="position"> the attribute's position </param>
          virtual void forceInsertAttributeAt( int position ) = 0;
        };

    }
}


#endif	//#ifndef _ABSTRACTINSTANCE_
