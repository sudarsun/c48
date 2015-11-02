#ifndef _BINC45SPLIT_
#define _BINC45SPLIT_

#include "ClassifierSplitModel.h"
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

class InfoGainSplitCrit;
class GainRatioSplitCrit;
class Instances;
class Instance;
class Distribution;


/// Class implementing a binary C4.5-like split on an attribute.

class BinC45Split : public ClassifierSplitModel
{


    /// <summary>
    /// Attribute to split on. </summary>
protected:
    const int mattIndex;

    /// <summary>
    /// Minimum number of objects in a split. </summary>
    const int mminNoObj;

    /// <summary>
    /// Use MDL correction? </summary>
    const bool museMDLcorrection;

    /// <summary>
    /// Value of split point. </summary>
    double msplitPoint = 0;

    /// <summary>
    /// InfoGain of split. </summary>
    double minfoGain = 0;

    /// <summary>
    /// GainRatio of split. </summary>
    double mgainRatio = 0;

    /// <summary>
    /// The sum of the weights of the instances. </summary>
    const double msumOfWeights;

    /// <summary>
    /// Static reference to splitting criterion. </summary>
    static InfoGainSplitCrit *minfoGainCrit;

    /// <summary>
    /// Static reference to splitting criterion. </summary>
    static GainRatioSplitCrit *mgainRatioCrit;

    /// <summary>
    /// Initializes the split model.
    /// </summary>
public:
    BinC45Split( int attIndex, int minNoObj, double sumOfWeights, bool useMDLcorrection );

    /// <summary>
    /// Creates a C4.5-type split on the given data.
    /// </summary>
    /// <exception cref="Exception"> if something goes wrong </exception>
    virtual void buildClassifier( Instances *trainInstances );

    /// <summary>
    /// Returns index of attribute for which split was generated.
    /// </summary>
    int attIndex();

    /// <summary>
    /// Returns the split point (numeric attribute only).
    /// </summary>
    /// <returns> the split point used for a test on a numeric attribute </returns>
    virtual double splitPoint();

    /// <summary>
    /// Returns (C4.5-type) gain ratio for the generated split.
    /// </summary>
    double gainRatio();

    /// <summary>
    /// Gets class probability for instance.
    /// </summary>
    /// <exception cref="Exception"> if something goes wrong </exception>
    virtual double classProb( int classIndex, Instance *instance, int theSubset );

    /// <summary>
    /// Creates split on enumerated attribute.
    /// </summary>
    /// <exception cref="Exception"> if something goes wrong </exception>
private:
    void handleEnumeratedAttribute( Instances *trainInstances );

    /// <summary>
    /// Creates split on numeric attribute.
    /// </summary>
    /// <exception cref="Exception"> if something goes wrong </exception>
    void handleNumericAttribute( Instances *trainInstances );

    /// <summary>
    /// Returns (C4.5-type) information gain for the generated split.
    /// </summary>
public:
    double infoGain();

    /// <summary>
    /// Prints left side of condition.
    /// </summary>
    /// <param name="data"> the data to get the attribute name from. </param>
    /// <returns> the attribute name </returns>
    virtual std::string leftSide( Instances *data );

    /// <summary>
    /// Prints the condition satisfied by instances in a subset.
    /// </summary>
    /// <param name="index"> of subset and training set. </param>
    virtual std::string rightSide( int index, Instances *data );

    /// <summary>
    /// Returns a string containing java source code equivalent to the test made at
    /// this node. The instance being tested is called "i".
    /// </summary>
    /// <param name="index"> index of the nominal value tested </param>
    /// <param name="data"> the data containing instance structure info </param>
    /// <returns> a value of type 'String' </returns>
    virtual std::string sourceExpression( int index, Instances *data );

    /// <summary>
    /// Sets split point to greatest value in given data smaller or equal to old
    /// split point. (C4.5 does this for some strange reason).
    /// </summary>
    void setSplitPoint( Instances *allInstances );

    /// <summary>
    /// Sets distribution associated with model.
    /// </summary>
    virtual void resetDistribution( Instances *data );

    /// <summary>
    /// Returns weights if instance is assigned to more than one subset. Returns
    /// null if instance is only assigned to one subset.
    /// </summary>
    virtual std::vector<double> weights( Instance *instance );

    /// <summary>
    /// Returns index of subset instance is assigned to. Returns -1 if instance is
    /// assigned to more than one subset.
    /// </summary>
    /// <exception cref="Exception"> if something goes wrong </exception>

    virtual int whichSubset( Instance *instance ) final;

};


#endif	//#ifndef _BINC45SPLIT_
