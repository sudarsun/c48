#include "ThresholdCurve.h"
#include "TwoClassStats.h"
#include "core/Instances.h"
#include "core/DenseInstance.h"
#include <vector>
#include "NominalPrediction.h"

#include "core/Attribute.h"
#include "core/Utils.h"

const std::string ThresholdCurve::RELATION_NAME = "ThresholdCurve";
const std::string ThresholdCurve::TRUE_POS_NAME = "True Positives";
const std::string ThresholdCurve::FALSE_NEG_NAME = "False Negatives";
const std::string ThresholdCurve::FALSE_POS_NAME = "False Positives";
const std::string ThresholdCurve::TRUE_NEG_NAME = "True Negatives";
const std::string ThresholdCurve::FP_RATE_NAME = "False Positive Rate";
const std::string ThresholdCurve::TP_RATE_NAME = "True Positive Rate";
const std::string ThresholdCurve::PRECISION_NAME = "Precision";
const std::string ThresholdCurve::RECALL_NAME = "Recal";
const std::string ThresholdCurve::FALLOUT_NAME = "Fallout";
const std::string ThresholdCurve::FMEASURE_NAME = "FMeasure";
const std::string ThresholdCurve::SAMPLE_SIZE_NAME = "Sample Size";
const std::string ThresholdCurve::LIFT_NAME = "Lift";
const std::string ThresholdCurve::THRESHOLD_NAME = "Threshold";

Instances *ThresholdCurve::getCurve( std::vector<Prediction*> predictions ) {

  if( predictions.size() == 0 ) {
    return nullptr;
  }
  return getCurve( predictions, (static_cast<NominalPrediction*>( predictions[ 0 ] ) )->distribution().size() - 1 );
}

Instances *ThresholdCurve::getCurve( std::vector<Prediction*> predictions, int classIndex ) {

  if( ( predictions.size() == 0 ) || ((static_cast<NominalPrediction*>(predictions.at(0)))->distribution().size() <= classIndex) ) {
    return nullptr;
  }

  double totPos = 0, totNeg = 0;
  std::vector<double> probs = getProbabilities( predictions, classIndex );

  // Get distribution of positive/negatives
  for( int i = 0; i < probs.size(); i++ ) {
    NominalPrediction *pred = static_cast<NominalPrediction*>( predictions.at( i ) );
    if( pred->actual() == Prediction::MISSING_VALUE ) {
      std::cout << " Skipping prediction with missing class value";
      continue;
    }
    if( pred->weight() < 0 ) {
      std::cout <<" Skipping prediction with negative weight";
      continue;
    }
    if( pred->actual() == classIndex ) {
      totPos += pred->weight();
    } else {
      totNeg += pred->weight();
    }
  }

  Instances *insts = makeHeader();
  std::vector<int> sorted = Utils::sort( probs );
  TwoClassStats *tc = new TwoClassStats( totPos, totNeg, 0, 0 );
  double threshold = 0;
  double cumulativePos = 0;
  double cumulativeNeg = 0;
  for( int i = 0; i < sorted.size(); i++ ) {

    if( ( i == 0 ) || ( probs[sorted[i]] > threshold ) ) {
  tc->setTruePositive( tc->getTruePositive() - cumulativePos );
  tc->setFalseNegative( tc->getFalseNegative() + cumulativePos );
  tc->setFalsePositive( tc->getFalsePositive() - cumulativeNeg );
  tc->setTrueNegative( tc->getTrueNegative() + cumulativeNeg );
  threshold = probs[sorted[i]];
  insts->add( makeInstance( tc, threshold ) );
  cumulativePos = 0;
  cumulativeNeg = 0;
  if( i == sorted.size() - 1 ) {
    break;
  }
    }

    NominalPrediction *pred = static_cast<NominalPrediction*>( predictions.at( sorted[i] ) );

    if( pred->actual() == Prediction::MISSING_VALUE ) {
  std::cout <<" Skipping prediction with missing class value";
  continue;
    }
    if( pred->weight() < 0 ) {
  std::cout <<" Skipping prediction with negative weight";
  continue;
    }
    if( pred->actual() == classIndex ) {
  cumulativePos += pred->weight();
    } else {
  cumulativeNeg += pred->weight();
    }
  }

  // make sure a zero point gets into the curve
  if( tc->getFalseNegative() != totPos || tc->getTrueNegative() != totNeg ) {
    tc = new TwoClassStats( 0, 0, totNeg, totPos );
    threshold = probs[sorted[sorted.size() - 1]] + 10e-6;
    insts->add( makeInstance( tc, threshold ) );
  }

  return insts;
}

double ThresholdCurve::getNPointPrecision( Instances *tcurve, int n ) {

  if( RELATION_NAME != tcurve->getRelationName() || (tcurve->numInstances() == 0) ) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  int recallInd = tcurve->attribute( RECALL_NAME )->index();
  int precisInd = tcurve->attribute( PRECISION_NAME )->index();
  std::vector<double> recallVals = tcurve->attributeToDoubleArray( recallInd );
  std::vector<int> sorted = Utils::sort( recallVals );
  double isize = 1.0 / ( n - 1 );
  double psum = 0;
  for( int i = 0; i < n; i++ ) {
    int pos = binarySearch( sorted, recallVals, i * isize );
    double recall = recallVals[sorted[pos]];
    double precis = tcurve->instance( sorted[pos] )->value( precisInd );

    // interpolate figures for non-endpoints
    while( ( pos != 0 ) && ( pos < sorted.size() - 1 ) ) {
      pos++;
      double recall2 = recallVals[sorted[pos]];
      if( recall2 != recall ) {
        double precis2 = tcurve->instance( sorted[pos] )->value( precisInd );
        double slope = ( precis2 - precis ) / ( recall2 - recall );
        double offset = precis - recall * slope;
        precis = isize * i * slope + offset;
        
        break;
      }
    }
    psum += precis;
  }
  return psum / n;
}

double ThresholdCurve::getROCArea( Instances *tcurve ) {

  const int n = tcurve->numInstances();
  if( RELATION_NAME != tcurve->getRelationName() || (n == 0) ) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  const int tpInd = tcurve->attribute( TRUE_POS_NAME )->index();
  const int fpInd = tcurve->attribute( FALSE_POS_NAME )->index();
  const std::vector<double> tpVals = tcurve->attributeToDoubleArray( tpInd );
  const std::vector<double> fpVals = tcurve->attributeToDoubleArray( fpInd );

  double area = 0.0, cumNeg = 0.0;
  const double totalPos = tpVals[0];
  const double totalNeg = fpVals[0];
  for( int i = 0; i < n; i++ ) {
  double cip, cin;
  if( i < n - 1 ) {
      cip = tpVals[i] - tpVals[i + 1];
      cin = fpVals[i] - fpVals[i + 1];
  } else {
      cip = tpVals[n - 1];
      cin = fpVals[n - 1];
  }
  area += cip * ( cumNeg + ( 0.5 * cin ) );
  cumNeg += cin;
  }
  area /= ( totalNeg * totalPos );

  return area;
}

int ThresholdCurve::getThresholdInstance( Instances *tcurve, double threshold ) {

  if( RELATION_NAME != tcurve->getRelationName() || (tcurve->numInstances() == 0) || (threshold < 0) || (threshold > 1.0) ) {
    return -1;
  }
  if( tcurve->numInstances() == 1 ) {
    return 0;
  }
  std::vector<double> tvals = tcurve->attributeToDoubleArray( tcurve->numAttributes() - 1 );
  std::vector<int> sorted = Utils::sort( tvals );
  return binarySearch( sorted, tvals, threshold );
}

int ThresholdCurve::binarySearch( std::vector<int> &index, std::vector<double> &vals, double target ) {

  int lo = 0, hi = (int)index.size() - 1;
  while( hi - lo > 1 ) {
    int mid = lo + ( hi - lo ) / 2;
    double midval = vals[index[mid]];
    if( target > midval ) {
      lo = mid;
    } else if( target < midval ) {
      hi = mid;
    } else {
      while( ( mid > 0 ) && ( vals[index[mid - 1]] == target ) ) {
        mid--;
      }
      return mid;
    }
  }
  return lo;
}

std::vector<double> ThresholdCurve::getProbabilities( std::vector<Prediction*> predictions, int classIndex ) {

  // sort by predicted probability of the desired class.
  std::vector<double> probs(predictions.size() );
  for( int i = 0; i < probs.size(); i++ ) {
    NominalPrediction *pred = static_cast<NominalPrediction*>( predictions[i] );
    probs[i] = pred->distribution()[classIndex];
  }
  return probs;
}

Instances *ThresholdCurve::makeHeader() {

  std::vector<Attribute*> fv;
  fv.push_back( new Attribute( TRUE_POS_NAME ) );
  fv.push_back( new Attribute( FALSE_NEG_NAME ) );
  fv.push_back( new Attribute( FALSE_POS_NAME ) );
  fv.push_back( new Attribute( TRUE_NEG_NAME ) );
  fv.push_back( new Attribute( FP_RATE_NAME ) );
  fv.push_back( new Attribute( TP_RATE_NAME ) );
  fv.push_back( new Attribute( PRECISION_NAME ) );
  fv.push_back( new Attribute( RECALL_NAME ) );
  fv.push_back( new Attribute( FALLOUT_NAME ) );
  fv.push_back( new Attribute( FMEASURE_NAME ) );
  fv.push_back( new Attribute( SAMPLE_SIZE_NAME ) );
  fv.push_back( new Attribute( LIFT_NAME ) );
  fv.push_back( new Attribute( THRESHOLD_NAME ) );
  return new Instances( RELATION_NAME, fv, 0 );
}

Instance *ThresholdCurve::makeInstance( TwoClassStats *tc, double prob ) {

  int count = 0;
  std::vector<double> vals( 13 );
  vals[count++] = tc->getTruePositive();
  vals[count++] = tc->getFalseNegative();
  vals[count++] = tc->getFalsePositive();
  vals[count++] = tc->getTrueNegative();
  vals[count++] = tc->getFalsePositiveRate();
  vals[count++] = tc->getTruePositiveRate();
  vals[count++] = tc->getPrecision();
  vals[count++] = tc->getRecall();
  vals[count++] = tc->getFallout();
  vals[count++] = tc->getFMeasure();
    double ss = ( tc->getTruePositive() + tc->getFalsePositive() ) / (tc->getTruePositive() + tc->getFalsePositive() + tc->getTrueNegative() + tc->getFalseNegative());
  vals[count++] = ss;
  double expectedByChance = ( ss * ( tc->getTruePositive() + tc->getFalseNegative() ) );
  if( expectedByChance < 1 ) {
    vals[count++] = Utils::missingValue();
  } else {
  vals[count++] = tc->getTruePositive() / expectedByChance;

  }
  vals[count++] = prob;
  return new DenseInstance( 1.0, vals );
}
