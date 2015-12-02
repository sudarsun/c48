#ifndef _TWOCLASSSTATS_
#define _TWOCLASSSTATS_

#include <string>
#include <vector>

// Forward class declarations:
class ConfusionMatrix;

/// <summary>
/// Encapsulates performance functions for two-class problems.
/// 
/// </summary>
class TwoClassStats {

  /// <summary>
  /// The names used when converting this object to a confusion matrix </summary>
  private:
  static std::vector<std::string> CATEGORY_NAMES;

  /// <summary>
  /// Pos predicted as pos </summary>
  double mTruePos = 0;

  /// <summary>
  /// Neg predicted as pos </summary>
  double mFalsePos = 0;

  /// <summary>
  /// Neg predicted as neg </summary>
  double mTrueNeg = 0;

  /// <summary>
  /// Pos predicted as neg </summary>
  double mFalseNeg = 0;

  /// <summary>
  /// Creates the TwoClassStats with the given initial performance values.
  /// </summary>
  /// <param name="tp"> the number of correctly classified positives </param>
  /// <param name="fp"> the number of incorrectly classified negatives </param>
  /// <param name="tn"> the number of correctly classified negatives </param>
  /// <param name="fn"> the number of incorrectly classified positives </param>
  public:
  TwoClassStats( double tp, double fp, double tn, double fn );

  /// <summary>
  /// Sets the number of positive instances predicted as positive </summary>
  virtual void setTruePositive( double tp );

  /// <summary>
  /// Sets the number of negative instances predicted as positive </summary>
  virtual void setFalsePositive( double fp );

  /// <summary>
  /// Sets the number of negative instances predicted as negative </summary>
  virtual void setTrueNegative( double tn );

  /// <summary>
  /// Sets the number of positive instances predicted as negative </summary>
  virtual void setFalseNegative( double fn );

  /// <summary>
  /// Gets the number of positive instances predicted as positive </summary>
  virtual double getTruePositive();

  /// <summary>
  /// Gets the number of negative instances predicted as positive </summary>
  virtual double getFalsePositive();

  /// <summary>
  /// Gets the number of negative instances predicted as negative </summary>
  virtual double getTrueNegative();

  /// <summary>
  /// Gets the number of positive instances predicted as negative </summary>
  virtual double getFalseNegative();

  /// <summary>
  /// Calculate the true positive rate. 
  /// This is defined as<para>
  /// <pre>
  /// correctly classified positives
  /// ------------------------------
  ///       total positives
  /// </pre>
  /// 
  /// </para>
  /// </summary>
  /// <returns> the true positive rate </returns>
  virtual double getTruePositiveRate();

  /// <summary>
  /// Calculate the false positive rate. 
  /// This is defined as<para>
  /// <pre>
  /// incorrectly classified negatives
  /// --------------------------------
  ///        total negatives
  /// </pre>
  /// 
  /// </para>
  /// </summary>
  /// <returns> the false positive rate </returns>
  virtual double getFalsePositiveRate();

  /// <summary>
  /// Calculate the precision. 
  /// This is defined as<para>
  /// <pre>
  /// correctly classified positives
  /// ------------------------------
  ///  total predicted as positive
  /// </pre>
  /// 
  /// </para>
  /// </summary>
  /// <returns> the precision </returns>
  virtual double getPrecision();

  /// <summary>
  /// Calculate the recall. 
  /// This is defined as<para>
  /// <pre>
  /// correctly classified positives
  /// ------------------------------
  ///       total positives
  /// </para>
  /// </pre><para>
  /// (Which is also the same as the truePositiveRate.)
  /// 
  /// </para>
  /// </summary>
  /// <returns> the recall </returns>
  virtual double getRecall();

  /// <summary>
  /// Calculate the F-Measure. 
  /// This is defined as<para>
  /// <pre>
  /// 2 * recall * precision
  /// ----------------------
  ///   recall + precision
  /// </pre>
  /// 
  /// </para>
  /// </summary>
  /// <returns> the F-Measure </returns>
  virtual double getFMeasure();

  /// <summary>
  /// Calculate the fallout. 
  /// This is defined as<para>
  /// <pre>
  /// incorrectly classified negatives
  /// --------------------------------
  ///   total predicted as positive
  /// </pre>
  /// 
  /// </para>
  /// </summary>
  /// <returns> the fallout </returns>
  virtual double getFallout();

  /// <summary>
  /// Generates a <code>ConfusionMatrix</code> representing the current
  /// two-class statistics, using class names "negative" and "positive".
  /// </summary>
  /// <returns> a <code>ConfusionMatrix</code>. </returns>
  virtual ConfusionMatrix *getConfusionMatrix();

  /// <summary>
  /// Returns a string containing the various performance measures
  /// for the current object 
  /// </summary>
  virtual std::string toString();
};


#endif	//#ifndef _TWOCLASSSTATS_
