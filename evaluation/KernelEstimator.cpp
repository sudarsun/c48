#include "KernelEstimator.h"
#include "core/Utils.h"
#include "c48/Stats.h"


double KernelEstimator::MAX_ERROR = 0.01;

int KernelEstimator::findNearestValue( double key ) {

  int low = 0;
  int high = m_NumValues;
  int middle = 0;
  while( low < high ) {
    middle = ( low + high ) / 2;
    double current = m_Values[middle];
    if( current == key ) {
  return middle;
    }
    if( current > key ) {
  high = middle;
    } else if( current < key ) {
  low = middle + 1;
    }
  }
  return low;
}

double KernelEstimator::round( double data ) {

  return std::rint( data / m_Precision ) * m_Precision;
}

KernelEstimator::KernelEstimator( double precision ) {

  m_Values = double_array( 50 );
  m_Weights = double_array( 50 );
  m_NumValues = 0;
  m_SumOfWeights = 0;
  m_AllWeightsOne = true;
  m_Precision = precision;
  // precision cannot be zero
  if( m_Precision < Utils::SMALL ) {
      m_Precision = Utils::SMALL;
  }
  //    m_StandardDev = 1e10 * m_Precision; // Set the standard deviation initially very wide
  m_StandardDev = m_Precision / ( 2 * 3 );
}

void KernelEstimator::addValue( double data, double weight ) {

  if( weight == 0 ) {
    return;
  }
  data = round( data );
  int insertIndex = findNearestValue( data );
  if( ( m_NumValues <= insertIndex ) || ( m_Values[insertIndex] != data ) ) {
    if( m_NumValues < m_Values.size() ) {
      int left = m_NumValues - insertIndex;
	  std::cout << "Please cross-check here tfor array copy";
	  std::copy(m_Values.begin() + insertIndex, m_Values.begin() +left, m_Values.begin()+ insertIndex + 1);
	  std::copy(m_Weights.begin() + insertIndex, m_Weights.begin() + left, m_Weights.begin() + insertIndex + 1);

     // std::copy( m_Values, insertIndex, m_Values, insertIndex + 1, left );
     // std::copy( m_Weights, insertIndex, m_Weights, insertIndex + 1, left );

      m_Values[insertIndex] = data;
      m_Weights[insertIndex] = weight;
      m_NumValues++;
    } else {
      double_array newValues( m_Values.size() * 2 );
      double_array newWeights( m_Values.size() * 2 );
      int left = m_NumValues - insertIndex;
	  std::copy(m_Values.begin(), m_Values.begin() + insertIndex, newValues.begin());
	  std::copy(m_Weights.begin(), m_Weights.begin() + insertIndex, newWeights.begin());
      //std::copy( m_Values, 0, newValues, 0, insertIndex );
      //std::copy( m_Weights, 0, newWeights, 0, insertIndex );
      newValues[insertIndex] = data;
      newWeights[insertIndex] = weight;
	  std::copy(m_Values.begin() + insertIndex, m_Values.begin()+left, newValues.begin() + insertIndex + 1);
	  std::copy(m_Weights.begin() + insertIndex, m_Weights.begin()+left, newWeights.begin() + insertIndex + 1);
	  //std::copy( m_Values, insertIndex, newValues, insertIndex + 1, left );
	  //std::copy( m_Weights, insertIndex, newWeights, insertIndex + 1, left );
      m_NumValues++;
      m_Values = newValues;
      m_Weights = newWeights;
    }
    if( weight != 1 ) {
      m_AllWeightsOne = false;
    }
  } else {
    m_Weights[insertIndex] += weight;
    m_AllWeightsOne = false;
  }
  m_SumOfWeights += weight;
  double range = m_Values[m_NumValues - 1] - m_Values[0];
  if( range > 0 ) {
    m_StandardDev = std::fmax( range / sqrt( m_SumOfWeights ), m_Precision / ( 2 * 3 ) );
        // allow at most 3 sds within one interval
  }
}

double KernelEstimator::getProbability( double data ) {

  double delta = 0, sum = 0, currentProb = 0;
  double zLower = 0, zUpper = 0;
  if( m_NumValues == 0 ) {
    zLower = ( data - ( m_Precision / 2 ) ) / m_StandardDev;
    zUpper = ( data + ( m_Precision / 2 ) ) / m_StandardDev;
    return (Stats::normalProbability( zUpper ) - Stats::normalProbability( zLower ) );
  }
  double weightSum = 0;
  int start = findNearestValue( data );
  for( int i = start; i < m_NumValues; i++ ) {
    delta = m_Values[i] - data;
    zLower = ( delta - ( m_Precision / 2 ) ) / m_StandardDev;
    zUpper = ( delta + ( m_Precision / 2 ) ) / m_StandardDev;
    currentProb = (Stats::normalProbability( zUpper ) - Stats::normalProbability( zLower ) );
    sum += currentProb * m_Weights[i];
    /*
    System.out.print("zL" + (i + 1) + ": " + zLower + " ");
    System.out.print("zU" + (i + 1) + ": " + zUpper + " ");
    System.out.print("P" + (i + 1) + ": " + currentProb + " ");
    System.out.println("total: " + (currentProb * m_Weights[i]) + " ");
    */
    weightSum += m_Weights[i];
    if( currentProb * ( m_SumOfWeights - weightSum ) < sum * MAX_ERROR ) {
  break;
    }
  }
  for( int i = start - 1; i >= 0; i-- ) {
    delta = m_Values[i] - data;
    zLower = ( delta - ( m_Precision / 2 ) ) / m_StandardDev;
    zUpper = ( delta + ( m_Precision / 2 ) ) / m_StandardDev;
    currentProb = (Stats::normalProbability( zUpper ) - Stats::normalProbability( zLower ) );
    sum += currentProb * m_Weights[i];
    weightSum += m_Weights[i];
    if( currentProb * ( m_SumOfWeights - weightSum ) < sum * MAX_ERROR ) {
  break;
    }
  }
  return sum / m_SumOfWeights;
}

string KernelEstimator::toString() {

  string result = std::to_string( m_NumValues ) + string(" Normal Kernels. \nStandardDev = " ) + Utils::doubleToString( m_StandardDev,6,4 ) + string( " Precision = " ) + std::to_string( m_Precision );
  if( m_NumValues == 0 ) {
    result += string( "  \nMean = 0" );
  } else {
    result += string( "  \nMeans =" );
    for( int i = 0; i < m_NumValues; i++ ) {
  result += string( " " ) + std::to_string( m_Values[i] );
    }
    if( !m_AllWeightsOne ) {
  result += string( "\nWeights = " );
  for( int i = 0; i < m_NumValues; i++ ) {
    result += string( " " ) + std::to_string( m_Weights[i] );
  }
    }
  }
  return result + string( "\n" );
}

int KernelEstimator::getNumKernels() {
  return m_NumValues;
}

double_array KernelEstimator::getMeans() {
  return m_Values;
}

double_array KernelEstimator::getWeights() {
  return m_Weights;
}

double KernelEstimator::getPrecision() {
  return m_Precision;
}

double KernelEstimator::getStdDev() {
  return m_StandardDev;
}
