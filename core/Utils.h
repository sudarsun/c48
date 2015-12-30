#ifndef _UTILS_
#define _UTILS_

#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include "Typedefs.h"

/**
 * Class implementing some simple utility methods.
 *
 */
class Utils
{

public:

    /** The small deviation allowed in double comparisons. */
    static const double SMALL;

    /**
     * Computes entropy for an array of integers.
     *
     * @param counts array of counts
     * @return - a log2 a - b log2 b - c log2 c + (a+b+c) log2 (a+b+c) when given
     *         array [a b c]
     */
    static double info(int_array &counts);

    /**
     * Tests if a is smaller or equal to b.
     *
     * @param a a double
     * @param b a double
     */
    static bool smOrEq(double a, double b);

    /**
    * Tests if a is greater or equal to b.
    *
    * @param a a double
    * @param b a double
    */
    static bool grOrEq(double a, double b);

    /**
     * Tests if a is smaller than b.
     *
     * @param a a double
     * @param b a double
     */
    static bool sm(double a, double b);

    /**
     * Tests if a is greater than b.
     *
     * @param a a double
     * @param b a double
     */
    static bool gr(double a, double b);

    /**
     * Returns the kth-smallest value in the array.
     *
     * @param array the array of integers
     * @param k the value of k
     * @return the kth-smallest value
     */
    static int kthSmallestValue(int_array &array_Renamed, int k);

    /**
     * Returns the kth-smallest value in the array
     *
     * @param array the array of double
     * @param k the value of k
     * @return the kth-smallest value
     */
    static double kthSmallestValue(double_array &array_Renamed, int k);

    /**
     * Returns the logarithm of a for base 2.
     *
     * @param a a double
     * @return the logarithm for base 2
     */
    static double getLog2(double a);

    /**
     * Returns index of maximum element in a given array of doubles. First maximum
     * is returned.
     *
     * @param doubles the array of doubles
     * @return the index of the maximum element
     */
    static int maxIndex(double_array &doubles);

    /**
     * Returns index of maximum element in a given array of integers. First
     * maximum is returned.
     *
     * @param ints the array of integers
     * @return the index of the maximum element
     */
    static int maxIndex(int_array &ints);

    /**
    * Computes the mean for an array of doubles.
    *
    * @param vector the array
    * @return the mean
    */
    static double mean(double_array &vector);

    /**
     * Returns index of minimum element in a given array of integers. First
     * minimum is returned.
     *
     * @param ints the array of integers
     * @return the index of the minimum element
     */
    static int minIndex(int_array &ints);

    /**
     * Returns index of minimum element in a given array of doubles. First minimum
     * is returned.
     *
     * @param doubles the array of doubles
     * @return the index of the minimum element
     */
    static int minIndex(double_array &doubles);

    /**
     * Tests if the given value codes "missing".
     *
     * @param val the value to be tested
     * @return true if val codes "missing"
     */
    static bool isMissingValue(double val);

    /**
     * Returns the value used to code a missing value. Note that equality tests on
     * this value will always return false, so use isMissingValue(double val) for
     * testing.
     *
     * @return the value used as missing value.
     */
    static double missingValue();

    /**
     * Returns the correlation coefficient of two double vectors.
     *
     * @param y1 double vector 1
     * @param y2 double vector 2
     * @param n the length of two double vectors
     * @return the correlation coefficient
     */
    static double correlation(double_array &y1, double_array &y2, int n);

    /**
    * Removes all occurrences of a string from another string.
    *
    * @param inString the string to remove substrings from.
    * @param substring the substring to remove.
    * @return the input string with occurrences of substring removed.
    */
    static string removeSubstring(const string &inString, const string &substring);

    /**
    * Replaces with a new string, all occurrences of a string from another
    * string.
    *
    * @param inString the string to replace substrings in.
    * @param subString the substring to replace.
    * @param replaceString the replacement substring
    * @return the input string with occurrences of substring replaced.
    */
    static string replaceSubstring(const string &inString, const string &subString, const string &replaceString);

    /**
    * Rounds a double and converts it into String.
    *
    * @param value the double value
    * @param afterDecimalPoint the (maximum) number of digits permitted after the
    *          decimal point
    * @return the double as a formatted string
    */
    static string doubleToString(double value, int afterDecimalPoint);

    /**
     * Rounds a double and converts it into a formatted decimal-justified String.
     * Trailing 0's are replaced with spaces.
     *
     * @param value the double value
     * @param width the width of the string
     * @param afterDecimalPoint the number of digits after the decimal point
     * @return the double as a formatted string
     */
    static string doubleToString(double value, int width, int afterDecimalPoint);

    /**
     * Tests if a is equal to b.
     *
     * @param a a double
     * @param b a double
     */
    static bool eq(double a, double b);

    /**
    * Converts carriage returns and new lines in a string into \r and \n.
    * Backquotes the following characters: ` " \ \t and %
    *
    * @param string the string
    * @return the converted string
    * @see #unbackQuoteChars(String)
    */
    static string backQuoteChars(string inString);

    /**
    * Normalizes the doubles in the array by their sum.
    *
    * @param doubles the array of double
    * @exception IllegalArgumentException if sum is Zero or NaN
    */
    static void normalize(double_array &doubles);

    /**
    * Normalizes the doubles in the array using the given value.
    *
    * @param doubles the array of double
    * @param sum the value by which the doubles are to be normalized
    * @exception IllegalArgumentException if sum is zero or NaN
    */
    static void normalize(double_array &doubles, double sum);

    /**
    * Converts an array containing the natural logarithms of probabilities stored
    * in a vector back into probabilities. The probabilities are assumed to sum
    * to one.
    *
    * @param a an array holding the natural logarithms of the probabilities
    * @return the converted array
    */
    static double_array logs2probs(double_array &a);

    /**
    * Returns the log-odds for a given probabilitiy.
    *
    * @param prob the probabilitiy
    *
    * @return the log-odds after the probability has been mapped to [Utils.SMALL,
    *         1-Utils.SMALL]
    */
    static double probToLogOdds(double prob);

    /**
    * Rounds a double to the next nearest integer value. The JDK version of it
    * doesn't work properly.
    *
    * @param value the double value
    * @return the resulting integer value
    */
    static int round(double value);

    /**
     * Replaces all "missing values" in the given array of double values with
     * MAX_VALUE.
     *
     * @param array the array to be modified.
     */
    static void replaceMissingWithMAX_VALUE(double_array &array_Renamed);

    /**
     * Rounds a double to the given number of decimal places.
     *
     * @param value the double value
     * @param afterDecimalPoint the number of digits after the decimal point
     * @return the double rounded to the given precision
     */
    static double roundDouble(double value, int afterDecimalPoint);

    /**
     * Sorts a given array of integers in ascending order and returns an array of
     * integers with the positions of the elements of the original array in the
     * sorted array. The sort is stable. (Equal elements remain in their original
     * order.)
     *
     * @param array this array is not changed by the method!
     * @return an array of integers with the positions in the sorted array.
     */
    static int_array Sort(int_array &array_Renamed);

    /**
     * Sorts a given array of doubles in ascending order and returns an array of
     * integers with the positions of the elements of the original array in the
     * sorted array. NOTE THESE CHANGES: the sort is no longer stable and it
     * doesn't use safe floating-point comparisons anymore. Occurrences of
     * Double.NaN are treated as Double.MAX_VALUE.
     *
     * @param array this array is not changed by the method!
     * @return an array of integers with the positions in the sorted array.
     */
    static int_array Sort(double_array &array_Renamed);

    /**
    * Sorts a given array of doubles in ascending order and returns an array of
    * integers with the positions of the elements of the original array in the
    * sorted array. Missing values in the given array are replaced by
    * Double.MAX_VALUE, so the array is modified in that case!
    *
    * @param array the array to be sorted, which is modified if it has missing
    *          values
    * @return an array of integers with the positions in the sorted array.
    */
    static int_array sortWithNoMissingValues(double_array &array_Renamed);

    /**
     * Sorts a given array of doubles in ascending order and returns an array of
     * integers with the positions of the elements of the original array in the
     * sorted array. The sort is stable (Equal elements remain in their original
     * order.) Occurrences of Double.NaN are treated as Double.MAX_VALUE
     *
     * @param array this array is not changed by the method!
     * @return an array of integers with the positions in the sorted array.
     */
    static int_array stableSort(double_array &array_Renamed);

    /**
    * Computes the variance for an array of doubles.
    *
    * @param vector the array
    * @return the variance
    */
    static double variance(double_array &vector);

    /**
    * Computes the sum of the elements of an array of doubles.
    *
    * @param doubles the array of double
    * @return the sum of the elements
    */
    static double sum(double_array &doubles);

    /**
     * Computes the sum of the elements of an array of integers.
     *
     * @param ints the array of integers
     * @return the sum of the elements
     */
    static int sum(int_array &ints);

    /**
    * Returns c*log2(c) for a given integer value c.
    *
    * @param c an integer value
    * @return c*log2(c) (but is careful to return 0 if c is 0)
    */
    static double xlogx(int c);
    /**
     * Breaks up the string, if wider than "columns" characters.
     *
     * @param s the string to process
     * @param columns the width in columns
     * @return the processed string
     */
    static void trim(string &outString);

private:
    /**
      * Initial index, filled with values from 0 to size - 1.
      */
    static int_array initialIndex(int size);

    /**
     * Sorts left, right, and center elements only, returns resulting center as
     * pivot.
     */
    static int sortLeftRightAndCenter(double_array &array_Renamed, int_array &index, int l, int r);

    /**
     * Swaps two elements in the given integer array.
     */
    static void swap(int_array &index, int l, int r);

    /**
     * Conditional swap for quick sort.
     */
    static void conditionalSwap(double_array &array_Renamed, int_array &index, int left, int right);

    /**
    * Partitions the instances around a pivot. Used by quicksort and
    * kthSmallestValue.
    *
    * @param array the array of doubles to be sorted
    * @param index the index into the array of doubles
    * @param l the first index of the subset
    * @param r the last index of the subset
    *
    * @return the index of the middle element
    */
    static int partition(double_array &array_Renamed, int_array &index, int l, int r, double pivot);

    /**
    * Partitions the instances around a pivot. Used by quicksort and
    * kthSmallestValue.
    *
    * @param array the array of integers to be sorted
    * @param index the index into the array of integers
    * @param l the first index of the subset
    * @param r the last index of the subset
    *
    * @return the index of the middle element
    */
    static int partition(int_array &array_Renamed, int_array &index, int l, int r);

    /**
     * Implements quicksort with median-of-three method and explicit sort for
     * problems of size three or less.
     *
     * @param array the array of doubles to be sorted
     * @param index the index into the array of doubles
     * @param left the first index of the subset to be sorted
     * @param right the last index of the subset to be sorted
     */
     // @ requires 0 <= first && first <= right && right < array.length;
     // @ requires (\forall int i; 0 <= i && i < index.length; 0 <= index[i] &&
     // index[i] < array.length);
     // @ requires array != index;
     // assignable index;
    static void quickSort(double_array &array_Renamed, int_array &index, int left, int right);

    /**
     * Implements quicksort according to Manber's "Introduction to Algorithms".
     *
     * @param array the array of integers to be sorted
     * @param index the index into the array of integers
     * @param left the first index of the subset to be sorted
     * @param right the last index of the subset to be sorted
     */
     // @ requires 0 <= first && first <= right && right < array.length;
     // @ requires (\forall int i; 0 <= i && i < index.length; 0 <= index[i] &&
     // index[i] < array.length);
     // @ requires array != index;
     // assignable index;
    static void quickSort(int_array &array_Renamed, int_array &index, int left, int right);

    /**
     * Implements computation of the kth-smallest element according to Manber's
     * "Introduction to Algorithms".
     *
     * @param array the array of double
     * @param index the index into the array of doubles
     * @param left the first index of the subset
     * @param right the last index of the subset
     * @param k the value of k
     *
     * @return the index of the kth-smallest element
     */
     // @ requires 0 <= first && first <= right && right < array.length;
    static int select(double_array &array_Renamed, int_array &index, int left, int right, int k);

    /**
     * Implements computation of the kth-smallest element according to Manber's
     * "Introduction to Algorithms".
     *
     * @param array the array of integers
     * @param index the index into the array of doubles
     * @param left the first index of the subset
     * @param right the last index of the subset
     * @param k the value of k
     *
     * @return the index of the kth-smallest element
     */
     // @ requires 0 <= first && first <= right && right < array.length;
    static int select(int_array &array_Renamed, int_array &index, int left, int right, int k);
};

#endif    //#ifndef _UTILS_
