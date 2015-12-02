#include "TwoClassStats.h"
#include "ConfusionMatrix.h"

std::vector<std::string> TwoClassStats::CATEGORY_NAMES = { "negative", "positive" };

TwoClassStats::TwoClassStats( double tp, double fp, double tn, double fn ) {

  setTruePositive( tp );
  setFalsePositive( fp );
  setTrueNegative( tn );
  setFalseNegative( fn );
}

void TwoClassStats::setTruePositive( double tp ) {
    mTruePos = tp;
}

void TwoClassStats::setFalsePositive( double fp ) {
    mFalsePos = fp;
}

void TwoClassStats::setTrueNegative( double tn ) {
    mTrueNeg = tn;
}

void TwoClassStats::setFalseNegative( double fn ) {
    mFalseNeg = fn;
}

double TwoClassStats::getTruePositive() {
    return mTruePos;
}

double TwoClassStats::getFalsePositive() {
    return mFalsePos;
}

double TwoClassStats::getTrueNegative() {
    return mTrueNeg;
}

double TwoClassStats::getFalseNegative() {
    return mFalseNeg;
}

double TwoClassStats::getTruePositiveRate() {
  if( 0 == ( mTruePos + mFalseNeg ) ) {
    return 0;
  } else {
    return mTruePos / ( mTruePos + mFalseNeg );
  }
}

double TwoClassStats::getFalsePositiveRate() {
  if( 0 == ( mFalsePos + mTrueNeg ) ) {
    return 0;
  } else {
    return mFalsePos / ( mFalsePos + mTrueNeg );
  }
}

double TwoClassStats::getPrecision() {
  if( 0 == ( mTruePos + mFalsePos ) ) {
    return 0;
  } else {
    return mTruePos / ( mTruePos + mFalsePos );
  }
}

double TwoClassStats::getRecall() {
    return getTruePositiveRate();
}

double TwoClassStats::getFMeasure() {

  double precision = getPrecision();
  double recall = getRecall();
  if( ( precision + recall ) == 0 ) {
    return 0;
  }
  return 2 * precision * recall / ( precision + recall );
}

double TwoClassStats::getFallout() {
  if( 0 == ( mTruePos + mFalsePos ) ) {
    return 0;
  } else {
    return mFalsePos / ( mTruePos + mFalsePos );
  }
}

ConfusionMatrix *TwoClassStats::getConfusionMatrix() {

  ConfusionMatrix *cm = new ConfusionMatrix( CATEGORY_NAMES );
  cm->set( 0, 0, mTrueNeg );
  cm->set( 0, 1, mFalsePos );
  cm->set( 1, 0, mFalseNeg );
  cm->set( 1, 1, mTruePos );
  return cm;
}

std::string TwoClassStats::toString() {

  std::string res = "";
  res.append( std::to_string(getTruePositive())).append(" ");
  res.append( std::to_string(getFalseNegative())).append(" ");
  res.append( std::to_string(getTrueNegative())).append(" ");
  res.append( std::to_string(getFalsePositive())).append(" ");
  res.append( std::to_string(getFalsePositiveRate())).append(" ");
  res.append( std::to_string(getTruePositiveRate())).append(" ");
  res.append( std::to_string(getPrecision())).append(" ");
  res.append( std::to_string(getRecall())).append(" ");
  res.append( std::to_string(getFMeasure())).append(" ");
  res.append( std::to_string(getFallout())).append(" ");

  return res;
}
