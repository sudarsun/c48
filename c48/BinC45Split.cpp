#include "BinC45Split.h"
#include "InfoGainSplitCrit.h"
#include "GainRatioSplitCrit.h"
#include "core/Instances.h"
#include "core/Instance.h"
#include "Distribution.h"
#include "core/Utils.h"


InfoGainSplitCrit *BinC45Split::minfoGainCrit = new InfoGainSplitCrit();
GainRatioSplitCrit *BinC45Split::mgainRatioCrit = new GainRatioSplitCrit();

BinC45Split::BinC45Split( int attIndex, int minNoObj, double sumOfWeights, bool useMDLcorrection ) : mattIndex( attIndex ), mminNoObj( minNoObj ), museMDLcorrection( useMDLcorrection ), msumOfWeights( sumOfWeights )
{

    // Get index of attribute to split on.

    // Set minimum number of objects.

    // Set sum of weights;

    // Whether to use the MDL correction for numeric attributes
}

void BinC45Split::buildClassifier( Instances *trainInstances )
{

    // Initialize the remaining instance variables.
    mnumSubsets = 0;
    msplitPoint = std::numeric_limits<double>::max();
    minfoGain = 0;
    mgainRatio = 0;

    // Different treatment for enumerated and numeric
    // attributes.
    if( trainInstances->attribute( mattIndex )->isNominal() )
    {
        handleEnumeratedAttribute( trainInstances );
    }
    else
    {
        trainInstances->sort( trainInstances->attribute( mattIndex ) );
        handleNumericAttribute( trainInstances );
    }
}

int BinC45Split::attIndex()
{

    return mattIndex;
}

double BinC45Split::splitPoint()
{
    return msplitPoint;
}

double BinC45Split::gainRatio()
{
    return mgainRatio;
}

double BinC45Split::classProb( int classIndex, Instance *instance, int theSubset )
{

    if( theSubset <= -1 )
    {
        std::vector<double> _weights = weights( instance );
        if( _weights.empty() )
        {
            return mdistribution->prob( classIndex );
        }
        else
        {
            double prob = 0;
            for( int i = 0; i < _weights.size(); i++ )
            {
                prob += _weights[i] * mdistribution->prob( classIndex, i );
            }
            return prob;
        }
    }
    else
    {
        if( Utils::gr( mdistribution->perBag( theSubset ), 0 ) )
        {
            return mdistribution->prob( classIndex, theSubset );
        }
        else
        {
            return mdistribution->prob( classIndex );
        }
    }
}

void BinC45Split::handleEnumeratedAttribute( Instances *trainInstances )
{

    Distribution *newDistribution, *secondDistribution;
    int numAttValues;
    double currIG, currGR;
    Instance *instance;
    int i;

    numAttValues = trainInstances->attribute( mattIndex )->numValues();
    newDistribution = new Distribution( numAttValues, trainInstances->numClasses() );

    // Only Instances with known values are relevant.
    java::util::Iterator<Instance*> enu = trainInstances->enumerateInstances();
    while( enu->hasMoreElements() )
    {
        instance = enu->nextElement();
        if( !instance->isMissing( mattIndex ) )
        {
            newDistribution->add( static_cast<int>( instance->value( mattIndex ) ), instance );
        }
    }
    mdistribution = newDistribution;

    // For all values
    for( i = 0; i < numAttValues; i++ )
    {

        if( Utils::grOrEq( newDistribution->perBag( i ), mminNoObj ) )
        {
            secondDistribution = new Distribution( newDistribution, i );

            // Check if minimum number of Instances in the two
            // subsets.
            if( secondDistribution->check( mminNoObj ) )
            {
                mnumSubsets = 2;
                currIG = minfoGainCrit->splitCritValue( secondDistribution, msumOfWeights );
                currGR = mgainRatioCrit->splitCritValue( secondDistribution, msumOfWeights, currIG );
                if( ( i == 0 ) || Utils::gr( currGR, mgainRatio ) )
                {
                    mgainRatio = currGR;
                    minfoGain = currIG;
                    msplitPoint = i;
                    mdistribution = secondDistribution;
                }
            }
        }
    }
}

void BinC45Split::handleNumericAttribute( Instances *trainInstances )
{

    int firstMiss;
    int next = 1;
    int last = 0;
    int index = 0;
    int splitIndex = -1;
    double currentInfoGain;
    double defaultEnt;
    double minSplit;
    Instance *instance;
    int i;

    // Current attribute is a numeric attribute.
    mdistribution = new Distribution( 2, trainInstances->numClasses() );

    // Only Instances with known values are relevant.
    java::util::Iterator<Instance*> enu = trainInstances->enumerateInstances();
    i = 0;
    while( enu->hasMoreElements() )
    {
        instance = enu->nextElement();
        if( instance->isMissing( mattIndex ) )
        {
            break;
        }
        mdistribution->add( 1, instance );
        i++;
    }
    firstMiss = i;

    // Compute minimum number of Instances required in each
    // subset.
    minSplit = 0.1 * ( mdistribution->total() ) / (trainInstances->numClasses());
    if( Utils::smOrEq( minSplit, mminNoObj ) )
    {
        minSplit = mminNoObj;
    }
    else if( Utils::gr( minSplit, 25 ) )
    {
        minSplit = 25;
    }

    // Enough Instances with known values?
    if( Utils::sm( firstMiss, 2 * minSplit ) )
    {
        return;
    }

    // Compute values of criteria for all possible split
    // indices.
    defaultEnt = minfoGainCrit->oldEnt( mdistribution );
    while( next < firstMiss )
    {

        if( trainInstances->instance( next - 1 )->value( mattIndex ) + 1e-5 < trainInstances->instance( next )->value( mattIndex ) )
        {

            // Move class values for all Instances up to next
            // possible split point.
            mdistribution->shiftRange( 1, 0, trainInstances, last, next );

            // Check if enough Instances in each subset and compute
            // values for criteria.
            if( Utils::grOrEq( mdistribution->perBag( 0 ), minSplit ) && Utils::grOrEq( mdistribution->perBag( 1 ), minSplit ) )
            {
                currentInfoGain = minfoGainCrit->splitCritValue( mdistribution, msumOfWeights, defaultEnt );
                if( Utils::gr( currentInfoGain, minfoGain ) )
                {
                    minfoGain = currentInfoGain;
                    splitIndex = next - 1;
                }
                index++;
            }
            last = next;
        }
        next++;
    }

    // Was there any useful split?
    if( index == 0 )
    {
        return;
    }

    // Compute modified information gain for best split.
    if( museMDLcorrection )
    {
        minfoGain = minfoGain - ( Utils::log2( index ) / msumOfWeights );
    }
    if( Utils::smOrEq( minfoGain, 0 ) )
    {
        return;
    }

    // Set instance variables' values to values for
    // best split.
    mnumSubsets = 2;
    msplitPoint = ( trainInstances->instance( splitIndex + 1 )->value( mattIndex ) + trainInstances->instance( splitIndex )->value( mattIndex ) ) / 2;

    // In case we have a numerical precision problem we need to choose the
    // smaller value
    if( msplitPoint == trainInstances->instance( splitIndex + 1 )->value( mattIndex ) )
    {
        msplitPoint = trainInstances->instance( splitIndex )->value( mattIndex );
    }

    // Restore distributioN for best split.
    mdistribution = new Distribution( 2, trainInstances->numClasses() );
    mdistribution->addRange( 0, trainInstances, 0, splitIndex + 1 );
    mdistribution->addRange( 1, trainInstances, splitIndex + 1, firstMiss );

    // Compute modified gain ratio for best split.
    mgainRatio = mgainRatioCrit->splitCritValue( mdistribution, msumOfWeights, minfoGain );
}

double BinC45Split::infoGain()
{

    return minfoGain;
}

std::string BinC45Split::leftSide( Instances *data )
{

    return data->attribute( mattIndex ).name();
}

std::string BinC45Split::rightSide( int index, Instances *data )
{

    std::string text;

    text = "";
    if( data->attribute( mattIndex ).isNominal() )
    {
        if( index == 0 )
        {
            text.append( std::string( " = " ) + data->attribute( mattIndex ).value( static_cast<int>( msplitPoint ) ) );
        }
        else
        {
            text.append( std::string( " != " ) + data->attribute( mattIndex ).value( static_cast<int>( msplitPoint ) ) );
        }
    }
    else if( index == 0 )
    {
        text.append( std::string( " <= " ) + std::to_wstring( msplitPoint ) );
    }
    else
    {
        text.append( std::string( " > " ) + std::to_string( msplitPoint ) );
    }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    return text
}

std::string BinC45Split::sourceExpression( int index, Instances *data )
{

    std::string expr = "";
    if( index < 0 )
    {
        return std::string( "i[" ) + std::to_string(mattIndex) + std::string( "] == nul" );
    }
    if( data->attribute( mattIndex )->isNominal() )
    {
        if( index == 0 )
        {
            expr =  "i[" ;
        }
        else
        {
            expr = "!i[" ;
        }
        expr.append( mattIndex ).append( "]" );
        expr.append( ".equals(\"" ).append( data->attribute( mattIndex )->value( static_cast<int>( msplitPoint ) ) ).append( "\")" );
    }
    else
    {
        expr =  "((Double) i[" ;
        expr.append( mattIndex )->append( "])" );
        if( index == 0 )
        {
            expr.append( ".doubleValue() <= " ).append( msplitPoint );
        }
        else
        {
            expr.append( ".doubleValue() > " ).append( msplitPoint );
        }
    }

    return expr;
}

void BinC45Split::setSplitPoint( Instances *allInstances )
{

    double newSplitPoint = -std::numeric_limits<double>::max();
    double tempValue;
    Instance *instance;

    if( ( !allInstances->attribute( mattIndex ).isNominal() ) && (mnumSubsets > 1) )
    {
        java::util::Iterator<Instance*> enu = allInstances->enumerateInstances();
        while( enu->hasMoreElements() )
        {
            instance = enu->nextElement();
            if( !instance->isMissing( mattIndex ) )
            {
                tempValue = instance->value( mattIndex );
                if( Utils::gr( tempValue, newSplitPoint ) && Utils::smOrEq( tempValue, msplitPoint ) )
                {
                    newSplitPoint = tempValue;
                }
            }
        }
        msplitPoint = newSplitPoint;
    }
}

void BinC45Split::resetDistribution( Instances *data )
{

    Instances *insts = new Instances( data, data->numInstances() );
    for( int i = 0; i < data->numInstances(); i++ )
    {
        if( whichSubset( data->instance( i ) ) > -1 )
        {
            insts->add( data->instance( i ) );
        }
    }
    Distribution *newD = new Distribution( insts, this );
    newD->addInstWithUnknown( data, mattIndex );
    mdistribution = newD;
}

std::vector<double> BinC45Split::weights( Instance *instance )
{

    std::vector<double> weights;
    int i;

    if( instance->isMissing( mattIndex ) )
    {
        weights = std::vector<double>( mnumSubsets );
        for( i = 0; i < mnumSubsets; i++ )
        {
            weights[i] = mdistribution->perBag( i ) / mdistribution->total();
        }
        return weights;
    }
    else
    {
        return nullptr;
    }
}

int BinC45Split::whichSubset( Instance *instance )
{

    if( instance->isMissing( mattIndex ) )
    {
        return -1;
    }
    else
    {
        if( instance->attribute( mattIndex ).isNominal() )
        {
            if( static_cast<int>( msplitPoint ) == static_cast<int>( instance->value( mattIndex ) ) )
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if( Utils::smOrEq( instance->value( mattIndex ), msplitPoint ) )
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}
