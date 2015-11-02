#include "AbstractClassifier.h"
#include "core/Instance.h"
#include "core/Instances.h"

#include "core/Consts.h"
#include "evaluation/BatchPredictor.h"
#include "core/Utils.h"

double AbstractClassifier::classifyInstance( Instance *instance ) {

  std::vector<double> dist = distributionForInstance( instance );
  if( dist.empty() ) {
    throw "Null distribution predicted" ;
  }
  switch( instance->classAttribute()->type() ) {
  case ATTRIBUTE::NOMINAL: {
    double max = 0;
    int maxIndex = 0;

    for( int i = 0; i < (int)dist.size(); i++ ) {
      if( dist[i] > max ) {
        maxIndex = i;
        max = dist[i];
      }
    }
    if( max > 0 ) {
      return maxIndex;
    } else {
      return Utils::missingValue();
    }
  }
  case ATTRIBUTE::NUMERIC:
  case ATTRIBUTE::DATE:
    return dist[0];
  default:
    return Utils::missingValue();
  }
}

std::vector<double> AbstractClassifier::distributionForInstance( Instance *instance ) {

  std::vector<double> dist( instance->numClasses() );
  switch( instance->classAttribute()->type() ) {
  case ATTRIBUTE::NOMINAL: {
    double classification = classifyInstance( instance );
    if( Utils::isMissingValue( classification ) ) {
      return dist;
    } else {
      dist[static_cast<int>( classification )] = 1.0;
    }
    return dist;
  }
  case ATTRIBUTE::NUMERIC:
  case ATTRIBUTE::DATE:
    dist[0] = classifyInstance( instance );
    return dist;
  default:
    return dist;
  }
}

bool AbstractClassifier::getDebug() {

  return m_Debug;
}

void AbstractClassifier::setDebug( bool debug ) {

  m_Debug = debug;
}

bool AbstractClassifier::getDoNotCheckCapabilities() {

  return m_DoNotCheckCapabilities;
}

void AbstractClassifier::setDoNotCheckCapabilities( bool doNotCheckCapabilities ) {

  m_DoNotCheckCapabilities = doNotCheckCapabilities;
}

int AbstractClassifier::getNumDecimalPlaces() {
  return m_numDecimalPlaces;
}

void AbstractClassifier::setNumDecimalPlaces( int num ) {
  m_numDecimalPlaces = num;
}

void AbstractClassifier::setBatchSize( const std::string &size ) {
  m_BatchSize = size;
}

std::string AbstractClassifier::getBatchSize() {
  return m_BatchSize;
}

bool AbstractClassifier::implementsMoreEfficientBatchPrediction() {
  return false;
}

std::vector<std::vector<double>> AbstractClassifier::distributionsForInstances( Instances *batch ) {
  std::vector<std::vector<double>> batchPreds( batch->numInstances() );
  for( int i = 0; i < batch->numInstances(); i++ ) {
    batchPreds[i] = distributionForInstance( batch->instance( i ) );
  }

  return batchPreds;
}
