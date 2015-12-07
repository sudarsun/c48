#ifndef _UTILS_
#define _UTILS_

#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

/// <summary>
/// Class implementing some simple utility methods.
/// </summary>
class Utils
{

public:

	static const double SMALL;

	/// <summary>
	/// Computes entropy for an array of integers.
	/// </summary>
	/// <param name="counts"> array of counts </param>
	/// <returns> - a log2 a - b log2 b - c log2 c + (a+b+c) log2 (a+b+c) when given
	///         array [a b c] </returns>
	static double info(std::vector<int> &counts);

	/// <summary>
	/// Tests if a is smaller or equal to b.
	/// </summary>
	/// <param name="a"> a double </param>
	/// <param name="b"> a double </param>
	static bool smOrEq(double a, double b);

	/// <summary>
	/// Tests if a is greater or equal to b.
	/// </summary>
	/// <param name="a"> a double </param>
	/// <param name="b"> a double </param>
	static bool grOrEq(double a, double b);
	/// <summary>
	/// Tests if a is smaller than b.
	/// </summary>
	/// <param name="a"> a double </param>
	/// <param name="b"> a double </param>
	static bool sm(double a, double b);

	/// <summary>
	/// Tests if a is greater than b.
	/// </summary>
	/// <param name="a"> a double </param>
	/// <param name="b"> a double </param>
	static bool gr(double a, double b);

	/// <summary>
	/// Returns the kth-smallest value in the array.
	/// </summary>
	/// <param name="array"> the array of integers </param>
	/// <param name="k"> the value of k </param>
	/// <returns> the kth-smallest value </returns>
	static int kthSmallestValue(std::vector<int> &array_Renamed, int k);
	/// <summary>
	/// Returns the kth-smallest value in the array
	/// </summary>
	/// <param name="array"> the array of double </param>
	/// <param name="k"> the value of k </param>
	/// <returns> the kth-smallest value </returns>
	static double kthSmallestValue(std::vector<double> &array_Renamed, int k);
	/// <summary>
	/// Returns the logarithm of a for base 2.
	/// </summary>
	/// <param name="a"> a double </param>
	/// <returns> the logarithm for base 2 </returns>
	static double getLog2(double a);

	/// <summary>
	/// Returns index of maximum element in a given array of doubles. First maximum
	/// is returned.
	/// </summary>
	/// <param name="doubles"> the array of doubles </param>
	/// <returns> the index of the maximum element </returns>
	static int maxIndex(std::vector<double> &doubles);
	/// <summary>
	/// Returns index of maximum element in a given array of integers. First
	/// maximum is returned.
	/// </summary>
	/// <param name="ints"> the array of integers </param>
	/// <returns> the index of the maximum element </returns>
	static int maxIndex(std::vector<int> &ints);

	/// <summary>
	/// Computes the mean for an array of doubles.
	/// </summary>
	/// <param name="vector"> the array </param>
	/// <returns> the mean </returns>
	static double mean(std::vector<double> &vector);

	/// <summary>
	/// Returns index of minimum element in a given array of integers. First
	/// minimum is returned.
	/// </summary>
	/// <param name="ints"> the array of integers </param>
	/// <returns> the index of the minimum element </returns>
	static int minIndex(std::vector<int> &ints);

	/// <summary>
	/// Returns index of minimum element in a given array of doubles. First minimum
	/// is returned.
	/// </summary>
	/// <param name="doubles"> the array of doubles </param>
	/// <returns> the index of the minimum element </returns>
	static int minIndex(std::vector<double> &doubles);

	/// <summary>
	/// Tests if the given value codes "missing".
	/// </summary>
	/// <param name="val"> the value to be tested </param>
	/// <returns> true if val codes "missing" </returns>

	static bool isMissingValue(double val);

	/// <summary>
	/// Returns the value used to code a missing value. Note that equality tests on
	/// this value will always return false, so use isMissingValue(double val) for
	/// testing..
	/// </summary>
	/// <returns> the value used as missing value. </returns>
	static double missingValue();

	/// <summary>
	/// Returns the correlation coefficient of two double vectors.
	/// </summary>
	/// <param name="y1"> double vector 1 </param>
	/// <param name="y2"> double vector 2 </param>
	/// <param name="n"> the length of two double vectors </param>
	/// <returns> the correlation coefficient </returns>
	static double correlation(std::vector<double> &y1, std::vector<double> &y2, int n);

	/// <summary>
	/// Removes all occurrences of a string from another string.
	/// </summary>
	/// <param name="inString"> the string to remove substrings from. </param>
	/// <param name="substring"> the substring to remove. </param>
	/// <returns> the input string with occurrences of substring removed. </returns>
	static std::string removeSubstring(const std::string &inString, const std::string &substring);

	/// <summary>
	/// Replaces with a new string, all occurrences of a string from another
	/// string.
	/// </summary>
	/// <param name="inString"> the string to replace substrings in. </param>
	/// <param name="subString"> the substring to replace. </param>
	/// <param name="replaceString"> the replacement substring </param>
	/// <returns> the input string with occurrences of substring replaced. </returns>
	static std::string replaceSubstring(const std::string &inString, const std::string &subString, const std::string &replaceString);

	/// <summary>
	/// Rounds a double and converts it into String.
	/// </summary>
	/// <param name="value"> the double value </param>
	/// <param name="afterDecimalPoint"> the (maximum) number of digits permitted after the
	///          decimal point </param>
	/// <returns> the double as a formatted string </returns>
	static std::string doubleToString(double value, int afterDecimalPoint);

	/// <summary>
	/// Rounds a double and converts it into a formatted decimal-justified String.
	/// Trailing 0's are replaced with spaces.
	/// </summary>
	/// <param name="value"> the double value </param>
	/// <param name="width"> the width of the string </param>
	/// <param name="afterDecimalPoint"> the number of digits after the decimal point </param>
	/// <returns> the double as a formatted string </returns>
	static std::string doubleToString(double value, int width, int afterDecimalPoint);

	/// <summary>
	/// Tests if a is equal to b.
	/// </summary>
	/// <param name="a"> a double </param>
	/// <param name="b"> a double </param>
	static bool eq(double a, double b);

	/// <summary>
	/// Converts carriage returns and new lines in a string into \r and \n.
	/// Backquotes the following characters: ` " \ \t and %
	/// </summary>
	/// <param name="string"> the string </param>
	/// <returns> the converted string </returns>
	/// <seealso cref= #unbackQuoteChars(String) </seealso>
	static std::string backQuoteChars(std::string inString);

	/// <summary>
	/// Normalizes the doubles in the array by their sum.
	/// </summary>
	/// <param name="doubles"> the array of double </param>
	/// <exception cref="IllegalArgumentException"> if sum is Zero or NaN </exception>
	static void normalize(std::vector<double> &doubles);

	/// <summary>
	/// Normalizes the doubles in the array using the given value.
	/// </summary>
	/// <param name="doubles"> the array of double </param>
	/// <param name="sum"> the value by which the doubles are to be normalized </param>
	/// <exception cref="IllegalArgumentException"> if sum is zero or NaN </exception>
	static void normalize(std::vector<double> &doubles, double sum);

	/// <summary>
	/// Converts an array containing the natural logarithms of probabilities stored
	/// in a vector back into probabilities. The probabilities are assumed to sum
	/// to one.
	/// </summary>
	/// <param name="a"> an array holding the natural logarithms of the probabilities </param>
	/// <returns> the converted array </returns>
	static std::vector<double> logs2probs(std::vector<double> &a);

	/// <summary>
	/// Returns the log-odds for a given probabilitiy.
	/// </summary>
	/// <param name="prob"> the probabilitiy
	/// </param>
	/// <returns> the log-odds after the probability has been mapped to [Utils.SMALL,
	///         1-Utils.SMALL] </returns>
	static double probToLogOdds(double prob);

	/// <summary>
	/// Rounds a double to the next nearest integer value. The JDK version of it
	/// doesn't work properly.
	/// </summary>
	/// <param name="value"> the double value </param>
	/// <returns> the resulting integer value </returns>
	static int round(double value);

	/// <summary>
	/// Replaces all "missing values" in the given array of double values with
	/// MAX_VALUE.
	/// </summary>
	/// <param name="array"> the array to be modified. </param>
	static void replaceMissingWithMAX_VALUE(std::vector<double> &array_Renamed);

	/// <summary>
	/// Rounds a double to the given number of decimal places.
	/// </summary>
	/// <param name="value"> the double value </param>
	/// <param name="afterDecimalPoint"> the number of digits after the decimal point </param>
	/// <returns> the double rounded to the given precision </returns>
	static double roundDouble(double value, int afterDecimalPoint);

	/// <summary>
	/// Sorts a given array of integers in ascending order and returns an array of
	/// integers with the positions of the elements of the original array in the
	/// sorted array. The sort is stable. (Equal elements remain in their original
	/// order.)
	/// </summary>
	/// <param name="array"> this array is not changed by the method! </param>
	/// <returns> an array of integers with the positions in the sorted array. </returns>
	static std::vector<int> Sort(std::vector<int> &array_Renamed);

	/// <summary>
	/// Sorts a given array of doubles in ascending order and returns an array of
	/// integers with the positions of the elements of the original array in the
	/// sorted array. NOTE THESE CHANGES: the sort is no longer stable and it
	/// doesn't use safe floating-point comparisons anymore. Occurrences of
	/// Double.NaN are treated as Double.MAX_VALUE.
	/// </summary>
	/// <param name="array"> this array is not changed by the method! </param>
	/// <returns> an array of integers with the positions in the sorted array. </returns>
	static std::vector<int> Sort(std::vector<double> &array_Renamed);

	/// <summary>
	/// Sorts a given array of doubles in ascending order and returns an array of
	/// integers with the positions of the elements of the original array in the
	/// sorted array. Missing values in the given array are replaced by
	/// Double.MAX_VALUE, so the array is modified in that case!
	/// </summary>
	/// <param name="array"> the array to be sorted, which is modified if it has missing
	///          values </param>
	/// <returns> an array of integers with the positions in the sorted array. </returns>
	static std::vector<int> sortWithNoMissingValues(std::vector<double> &array_Renamed);

	/// <summary>
	/// Sorts a given array of doubles in ascending order and returns an array of
	/// integers with the positions of the elements of the original array in the
	/// sorted array. The sort is stable (Equal elements remain in their original
	/// order.) Occurrences of Double.NaN are treated as Double.MAX_VALUE
	/// </summary>
	/// <param name="array"> this array is not changed by the method! </param>
	/// <returns> an array of integers with the positions in the sorted array. </returns>
	static std::vector<int> stableSort(std::vector<double> &array_Renamed);

	/// <summary>
	/// Computes the variance for an array of doubles.
	/// </summary>
	/// <param name="vector"> the array </param>
	/// <returns> the variance </returns>
	static double variance(std::vector<double> &vector);

	/// <summary>
	/// Computes the sum of the elements of an array of doubles.
	/// </summary>
	/// <param name="doubles"> the array of double </param>
	/// <returns> the sum of the elements </returns>
	static double sum(std::vector<double> &doubles);

	/// <summary>
	/// Computes the sum of the elements of an array of integers.
	/// </summary>
	/// <param name="ints"> the array of integers </param>
	/// <returns> the sum of the elements </returns>
	static int sum(std::vector<int> &ints);

	/// <summary>
	/// Returns c*log2(c) for a given integer value c.
	/// </summary>
	/// <param name="c"> an integer value </param>
	/// <returns> c*log2(c) (but is careful to return 0 if c is 0) </returns>
	static double xlogx(int c);
	/// <summary>
	/// Breaks up the string, if wider than "columns" characters.
	/// </summary>
	/// <param name="s"> the string to process </param>
	/// <param name="columns"> the width in columns </param>
	/// <returns> the processed string </returns>
	static void trim(std::string &outString);

	/// <summary>
	/// Initial index, filled with values from 0 to size - 1.
	/// </summary>
private:
	static std::vector<int> initialIndex(int size);

	/// <summary>
	/// Sorts left, right, and center elements only, returns resulting center as
	/// pivot.
	/// </summary>
	static int sortLeftRightAndCenter(std::vector<double> &array_Renamed, std::vector<int> &index, int l, int r);

	/// <summary>
	/// Swaps two elements in the given integer array.
	/// </summary>
	static void swap(std::vector<int> &index, int l, int r);

	/// <summary>
	/// Conditional swap for quick sort.
	/// </summary>
	static void conditionalSwap(std::vector<double> &array_Renamed, std::vector<int> &index, int left, int right);

	/// <summary>
	/// Partitions the instances around a pivot. Used by quicksort and
	/// kthSmallestValue.
	/// </summary>
	/// <param name="array"> the array of doubles to be sorted </param>
	/// <param name="index"> the index into the array of doubles </param>
	/// <param name="l"> the first index of the subset </param>
	/// <param name="r"> the last index of the subset
	/// </param>
	/// <returns> the index of the middle element </returns>
	static int partition(std::vector<double> &array_Renamed, std::vector<int> &index, int l, int r, double pivot);

	/// <summary>
	/// Partitions the instances around a pivot. Used by quicksort and
	/// kthSmallestValue.
	/// </summary>
	/// <param name="array"> the array of integers to be sorted </param>
	/// <param name="index"> the index into the array of integers </param>
	/// <param name="l"> the first index of the subset </param>
	/// <param name="r"> the last index of the subset
	/// </param>
	/// <returns> the index of the middle element </returns>
	static int partition(std::vector<int> &array_Renamed, std::vector<int> &index, int l, int r);

	/// <summary>
	/// Implements quicksort with median-of-three method and explicit sort for
	/// problems of size three or less.
	/// </summary>
	/// <param name="array"> the array of doubles to be sorted </param>
	/// <param name="index"> the index into the array of doubles </param>
	/// <param name="left"> the first index of the subset to be sorted </param>
	/// <param name="right"> the last index of the subset to be sorted </param>
	// @ requires 0 <= first && first <= right && right < array.length;
	// @ requires (\forall int i; 0 <= i && i < index.length; 0 <= index[i] &&
	// index[i] < array.length);
	// @ requires array != index;
	// assignable index;
	static void quickSort(std::vector<double> &array_Renamed, std::vector<int> &index, int left, int right);

	/// <summary>
	/// Implements quicksort according to Manber's "Introduction to Algorithms".
	/// </summary>
	/// <param name="array"> the array of integers to be sorted </param>
	/// <param name="index"> the index into the array of integers </param>
	/// <param name="left"> the first index of the subset to be sorted </param>
	/// <param name="right"> the last index of the subset to be sorted </param>
	// @ requires 0 <= first && first <= right && right < array.length;
	// @ requires (\forall int i; 0 <= i && i < index.length; 0 <= index[i] &&
	// index[i] < array.length);
	// @ requires array != index;
	// assignable index;
	static void quickSort(std::vector<int> &array_Renamed, std::vector<int> &index, int left, int right);

	/// <summary>
	/// Implements computation of the kth-smallest element according to Manber's
	/// "Introduction to Algorithms".
	/// </summary>
	/// <param name="array"> the array of double </param>
	/// <param name="index"> the index into the array of doubles </param>
	/// <param name="left"> the first index of the subset </param>
	/// <param name="right"> the last index of the subset </param>
	/// <param name="k"> the value of k
	/// </param>
	/// <returns> the index of the kth-smallest element </returns>
	// @ requires 0 <= first && first <= right && right < array.length;
	static int select(std::vector<double> &array_Renamed, std::vector<int> &index, int left, int right, int k);

	/// <summary>
	/// Implements computation of the kth-smallest element according to Manber's
	/// "Introduction to Algorithms".
	/// </summary>
	/// <param name="array"> the array of integers </param>
	/// <param name="index"> the index into the array of integers </param>
	/// <param name="left"> the first index of the subset </param>
	/// <param name="right"> the last index of the subset </param>
	/// <param name="k"> the value of k
	/// </param>
	/// <returns> the index of the kth-smallest element </returns>
	// @ requires 0 <= first && first <= right && right < array.length;
	static int select(std::vector<int> &array_Renamed, std::vector<int> &index, int left, int right, int k);
};

#endif	//#ifndef _UTILS_
