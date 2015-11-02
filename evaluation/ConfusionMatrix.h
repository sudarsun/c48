#ifndef _CONFUSIONMATRIX_
#define _CONFUSIONMATRIX_

#include "Matrix.h"
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class CostMatrix;
class TwoClassStats;
class NominalPrediction;
class Prediction;
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
 *    NominalPrediction.java
 *    Copyright (C) 2002-2012 University of Waikato, Hamilton, New Zealand
 *
 */

namespace weka {
    namespace classifiers {
        namespace evaluation {


            using weka::classifiers::CostMatrix;
            using weka::core::matrix::Matrix;

            /// <summary>
            /// Cells of this matrix correspond to counts of the number (or weight) of
            /// predictions for each actual value / predicted value combination.
            /// 
            /// @author Len Trigg (len@reeltwo.com)
            /// @version $Revision: 10169 $
            /// </summary>
            class ConfusionMatrix : public Matrix {

              /// <summary>
              /// for serialization </summary>
          private:
              static const long long serialVersionUID = -181789981401504090LL;

              /// <summary>
              /// Stores the names of the classes </summary>
          protected:
              std::vector<std::wstring> m_ClassNames;

              /// <summary>
              /// Creates the confusion matrix with the given class names.
              /// </summary>
              /// <param name="classNames"> an array containing the names the classes. </param>
          public:
              ConfusionMatrix( std::vector<std::wstring> &classNames );

              /// <summary>
              /// Makes a copy of this ConfusionMatrix after applying the supplied CostMatrix
              /// to the cells. The resulting ConfusionMatrix can be used to get
              /// cost-weighted statistics.
              /// </summary>
              /// <param name="costs"> the CostMatrix. </param>
              /// <returns> a ConfusionMatrix that has had costs applied. </returns>
              /// <exception cref="Exception"> if the CostMatrix is not of the same size as this
              ///              ConfusionMatrix. </exception>
              virtual ConfusionMatrix *makeWeighted( CostMatrix *costs ) throw( std::exception );

              /// <summary>
              /// Creates and returns a clone of this object.
              /// </summary>
              /// <returns> a clone of this instance. </returns>
              virtual void *clone();

              /// <summary>
              /// Gets the number of classes.
              /// </summary>
              /// <returns> the number of classes </returns>
              virtual int size();

              /// <summary>
              /// Gets the name of one of the classes.
              /// </summary>
              /// <param name="index"> the index of the class. </param>
              /// <returns> the class name. </returns>
              virtual std::wstring className( int index );

              /// <summary>
              /// Includes a prediction in the confusion matrix.
              /// </summary>
              /// <param name="pred"> the NominalPrediction to include </param>
              /// <exception cref="Exception"> if no valid prediction was made (i.e. unclassified). </exception>
              virtual void addPrediction( NominalPrediction *pred ) throw( std::exception );

              /// <summary>
              /// Includes a whole bunch of predictions in the confusion matrix.
              /// </summary>
              /// <param name="predictions"> a FastVector containing the NominalPredictions to
              ///          include </param>
              /// <exception cref="Exception"> if no valid prediction was made (i.e. unclassified). </exception>
              virtual void addPredictions( std::vector<Prediction*> &predictions ) throw( std::exception );

              /// <summary>
              /// Gets the performance with respect to one of the classes as a TwoClassStats
              /// object.
              /// </summary>
              /// <param name="classIndex"> the index of the class of interest. </param>
              /// <returns> the generated TwoClassStats object. </returns>
              virtual TwoClassStats *getTwoClassStats( int classIndex );

              /// <summary>
              /// Gets the number of correct classifications (that is, for which a correct
              /// prediction was made). (Actually the sum of the weights of these
              /// classifications)
              /// </summary>
              /// <returns> the number of correct classifications </returns>
              virtual double correct();

              /// <summary>
              /// Gets the number of incorrect classifications (that is, for which an
              /// incorrect prediction was made). (Actually the sum of the weights of these
              /// classifications)
              /// </summary>
              /// <returns> the number of incorrect classifications </returns>
              virtual double incorrect();

              /// <summary>
              /// Gets the number of predictions that were made (actually the sum of the
              /// weights of predictions where the class value was known).
              /// </summary>
              /// <returns> the number of predictions with known class </returns>
              virtual double total();

              /// <summary>
              /// Returns the estimated error rate.
              /// </summary>
              /// <returns> the estimated error rate (between 0 and 1). </returns>
              virtual double errorRate();

              /// <summary>
              /// Calls toString() with a default title.
              /// </summary>
              /// <returns> the confusion matrix as a string </returns>
              virtual std::wstring toString();

              /// <summary>
              /// Outputs the performance statistics as a classification confusion matrix.
              /// For each class value, shows the distribution of predicted class values.
              /// </summary>
              /// <param name="title"> the title for the confusion matrix </param>
              /// <returns> the confusion matrix as a String </returns>
              virtual std::wstring toString( const std::wstring &title );

              /// <summary>
              /// Method for generating indices for the confusion matrix.
              /// </summary>
              /// <param name="num"> integer to format </param>
              /// <returns> the formatted integer as a string </returns>
          private:
              static std::wstring num2ShortID( int num, std::vector<wchar_t> &IDChars, int IDWidth );

              /// <summary>
              /// Returns the revision string.
              /// </summary>
              /// <returns> the revision </returns>
          public:
              virtual std::wstring getRevision();
            };

        }
    }
}


#endif	//#ifndef _CONFUSIONMATRIX_
