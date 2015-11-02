#ifndef _STATISTICS_
#define _STATISTICS_

#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <limits>


  /// <summary>
    /// A class to store simple statistics.<para>
    ///
    /// </para>
    /// Upon initialization the variables take the following values:<para>
    /// <code>
    /// <seealso cref="#count"/> = <seealso cref="#sum"/> = <seealso cref="#sumSq"/> = 0 <br />
    /// <seealso cref="#mean"/> = <seealso cref="#stdDev"/> = <seealso cref="#min"/> = <seealso cref="#max"/> = Double.NaN
    /// </para>
    /// </code><para>
    /// </para>
    /// This is called the initial state. <para>
    ///
    /// For signaling that a Stats object has been provided with values that hint
    /// that something is either wrong with the data used or the algorithm used there
    /// </para>
    /// is also the invalid state where the variables take the following values: <para>
    /// <code>
    /// <seealso cref="#count"/> = <seealso cref="#sum"/> = <seealso cref="#sumSq"/> = <seealso cref="#mean"/> =
    /// <seealso cref="#stdDev"/> = <seealso cref="#min"/> = <seealso cref="#max"/> = Double.NaN
    /// </para>
    /// <code><para>
    /// Once a Stats object goes into the invalid state it can't change its state
    /// </para>
    /// anymore. <para>
    ///
    /// A Stats object assumes that only values are subtracted (by using the
    /// <seealso cref="#subtract(double)"/> or <seealso cref="#subtract(double, double)"/> methods)
    /// that have previously been added (by using the <seealso cref="#add(double)"/> or
    /// <seealso cref="#add(double, double)"/> methods) and the weights must be the same
    /// too.<br />
    /// </para>
    /// Otherwise the Stats object's fields' values are implementation defined.<para>
    ///
    /// If the implementation detects a problem then the Stats object goes into the
    /// </para>
    /// invalid state.<para>
    ///
    /// The fields <seealso cref="#count"/>, <seealso cref="#sum"/>, <seealso cref="#sumSq"/>, <seealso cref="#min"/> and
    /// <seealso cref="#max"/> are always updated whereas the field <seealso cref="#mean"/> and
    /// <seealso cref="#stdDev"/> are only guaranteed to be updated after a call to
    /// </para>
    /// <seealso cref="#calculateDerived()"/>.<para>
    ///
    /// </para>
    /// For the fields <seealso cref="#min"/> and <seealso cref="#max"/> the following rules apply:<para>
    /// <code>
    /// min(values_added \ values_subtracted) >= <seealso cref="#min"/> >= min(values_added)<br>
    /// max(values_added \ values_subtracted) <= <seealso cref="#max"/> <= max(values_added)
    /// </para>
    /// </code><para>
    /// </para>
    /// Where \ is the set difference.<para>
    ///
    /// </para>
    /// For the field <seealso cref="#stdDev"/> the following rules apply:<para>
    /// <ol>
    /// <li>If count <= 1 then
    ///     <seealso cref="#stdDev"/>=Double.NaN.</li>
    /// <li>Otherwise <seealso cref="#stdDev"/> >= 0 and it should take on the value by best
    ///     effort of the implementation.</li>
    /// </ol>
    ///
    /// For the methods <seealso cref="#add(double)"/>, <seealso cref="#add(double, double)"/>,
    /// <seealso cref="#subtract(double)"/> and <seealso cref="#subtract(double, double)"/> the following
    /// </para>
    /// rules apply:<para>
    ///
    /// <ol>
    /// <li>if weight < 0 then <seealso cref="#subtract(double, double)"/> is used instead of
    ///     <seealso cref="#add(double, double)"/> with weight = -weight and vice versa.</li>
    /// <li>if weight = +-inf or weight = NaN then the Stats object goes into the
    ///     invalid state.</li>
    /// <li>if value = +-inf or value = NaN then the Stats object goes into the
    ///     invalid state.</li>
    /// <li>if weight = 0 then the value gets ignored.</li>
    /// <li>Otherwise the fields get updated by the implementation's best effort.</li>
    /// </ol>
    ///
    /// </para>
    /// For <seealso cref="#count"/> the following rules apply<para>
    ///
    /// <ol>
    /// <li>If <seealso cref="#count"/> goes below zero then all fields are set to
    /// <code>Double.NaN</code> except the <seealso cref="#count"/> field which gets tracked
    /// normally.</li>
    /// <li>If <seealso cref="#count"/> = 0 then the Stats object goes into the initial state.
    /// </li>
    /// <li>If <seealso cref="#count"/> > 0 for the first time, then the Stats object goes into
    ///     initial state and gets updated with the corresponding value and weight.
    /// </li>
    /// </ol>
    ///
    /// @author Len Trigg (trigg@cs.waikato.ac.nz)
    /// @version $Revision$
    /// </para>
    /// </summary>
    class Statistics {

      /// <summary>
      /// The number of values seen </summary>
  public:
      double count = 0;

      /// <summary>
      /// The sum of values seen </summary>
      double sum = 0;

      /// <summary>
      /// The sum of values squared seen </summary>
      double sumSq = 0;

      /// <summary>
      /// The std deviation of values at the last calculateDerived() call </summary>
      double stdDev = std::numeric_limits<double>::quiet_NaN();

      /// <summary>
      /// The mean of values at the last calculateDerived() call </summary>
      double mean = std::numeric_limits<double>::quiet_NaN();

      /// <summary>
      /// The minimum value seen, or Double.NaN if no values seen </summary>
      double min = std::numeric_limits<double>::quiet_NaN();

      /// <summary>
      /// The maximum value seen, or Double.NaN if no values seen </summary>
      double max = std::numeric_limits<double>::quiet_NaN();

      /// <summary>
      /// an important factor to calculate the standard deviation incrementally </summary>
  private:
      double stdDevFactor = 0;

      void reset();

      void negativeCount();

      void goInvalid();

      bool isInvalid();

      /// <summary>
      /// Adds a value to the observed values<para>
      ///
      /// </para>
      /// It's equivalent to <code>add(value, 1)</code><para>
      ///
      /// </para>
      /// </summary>
      /// <param name="value"> the observed value </param>
  public:
      virtual void add( double value );

      /// <summary>
      /// Adds a weighted value to the observed values
      /// </summary>
      /// <param name="value"> the observed value </param>
      /// <param name="weight"> the weight of the observed value </param>
      virtual void add( double value, double weight );

      /// <summary>
      /// Removes a value to the observed values (no checking is done
      /// that the value being removed was actually added).<para>
      ///
      /// </para>
      /// It's equivalent to <code>subtract(value, 1)</code><para>
      ///
      /// </para>
      /// </summary>
      /// <param name="value"> the observed value </param>
      virtual void subtract( double value );

      /// <summary>
      /// Subtracts a weighted value from the observed values
      /// </summary>
      /// <param name="value"> the observed value </param>
      /// <param name="weight"> the weight of the observed value </param>
      virtual void subtract( double value, double weight );

      /// <summary>
      /// Tells the object to calculate any statistics that don't have their
      /// values automatically updated during add. Currently updates the mean
      /// and standard deviation.
      /// </summary>
      virtual void calculateDerived();

      /// <summary>
      /// Returns a string summarising the stats so far.
      /// </summary>
      /// <returns> the summary string </returns>
      virtual std::string toString();

    }; // Stats


#endif	// _STATISTICS_
