#ifndef _BINC45MODELSELECTION_
#define _BINC45MODELSELECTION_

#include "ModelSelection.h"
#include <string>
#include <vector>
#include <stdexcept>

class BinC45Split;
class Instances;
class ClassifierSplitModel;
class NoSplit;
class Distribution;
class Attribute;



                /// <summary>
                /// Class for selecting a C4.5-like binary (!) split for a given dataset.
                /// 
                /// @author Eibe Frank (eibe@cs.waikato.ac.nz)
                /// @version $Revision$
                /// </summary>
                class BinC45ModelSelection : public ModelSelection {


                  /// <summary>
                  /// Minimum number of instances in interval. </summary>
              protected:
                  const int mminNoObj;

                  /// <summary>
                  /// Use MDL correction? </summary>
                  const bool museMDLcorrection;

                  /// <summary>
                  /// The FULL training dataset. </summary>
                  Instances *mallData;

                  /// <summary>
                  /// Do not relocate split point to actual data value </summary>
                  const bool mdoNotMakeSplitPointActualValue;

                  /// <summary>
                  /// Initializes the split selection method with the given parameters.
                  /// </summary>
                  /// <param name="minNoObj"> minimum number of instances that have to occur in at least
                  ///          two subsets induced by split </param>
                  /// <param name="allData"> FULL training dataset (necessary for selection of split
                  ///          points). </param>
                  /// <param name="useMDLcorrection"> whether to use MDL adjustement when finding splits
                  ///          on numeric attributes </param>
              public:
                  BinC45ModelSelection( int minNoObj, Instances *allData, bool useMDLcorrection, bool doNotMakeSplitPointActualValue );

                  /// <summary>
                  /// Sets reference to training data to null.
                  /// </summary>
                  virtual void cleanup();

                  /// <summary>
                  /// Selects C4.5-type split for the given dataset.
                  /// </summary>
                  ClassifierSplitModel *selectModel( Instances *data );

                  /// <summary>
                  /// Selects C4.5-type split for the given dataset.
                  /// </summary>
                  ClassifierSplitModel *selectModel( Instances *train, Instances *test );

                };


#endif	//#ifndef _BINC45MODELSELECTION_
