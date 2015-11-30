#ifndef _STATS_H_
#define _STATS_H_
#include <vector>

/// <summary>
/// Class implementing a statistical routine needed by C48 to
/// compute its error estimate.
/// </summary>
class Stats {

	/// <summary>
	/// Computes estimated extra error for given total number of instances
	/// and error using normal approximation to binomial distribution
	/// (and continuity correction).
	/// </summary>
	/// <param name="N"> number of instances </param>
	/// <param name="e"> observed error </param>
	/// <param name="CF"> confidence value </param>
public:
	static double addErrs(double N, double e, float CF);

	/// <summary>
	/// Returns the value, <tt>x</tt>, for which the area under the Normal
	/// (Gaussian) probability density function (integrated from minus infinity to
	/// <tt>x</tt>) is equal to the argument <tt>y</tt> (assumes mean is zero,
	/// variance is one).
	/// <para>
	/// For small arguments <tt>0 < y < exp(-2)</tt>, the program computes
	/// <tt>z = sqrt( -2.0 * log(y) )</tt>; then the approximation is
	/// <tt>x = z - log(z)/z  - (1/z) P(1/z) / Q(1/z)</tt>. There are two rational
	/// functions P/Q, one for <tt>0 < y < exp(-32)</tt> and the other for
	/// <tt>y</tt> up to <tt>exp(-2)</tt>. For larger arguments,
	/// <tt>w = y - 0.5</tt>, and <tt>x/sqrt(2pi) = w + w**3 R(w**2)/S(w**2))</tt>.
	///
	/// </para>
	/// </summary>
	/// <param name="y0"> the area under the normal pdf </param>
	/// <returns> the z-value </returns>

	static double normalInverse(double y0);
	static double p1evl(double x, const std::vector<double> &coef, int N);
	static double polevl(double x, const std::vector<double> &coef, int N);

	/// <summary>
	/// Returns the area under the Normal (Gaussian) probability density
	/// function, integrated from minus infinity to <tt>x</tt>
	/// (assumes mean is zero, variance is one).
	/// <pre>
	///                            x
	///                             -
	///                   1        | |          2
	///  normal(x)  = ---------    |    exp( - t /2 ) dt
	///               sqrt(2pi)  | |
	///                           -
	///                          -inf.
	///   
	///             =  ( 1 + erf(z) ) / 2
	///             =  erfc(z) / 2
	/// </pre>
	/// where <tt>z = x/sqrt(2)</tt>.
	/// Computation is via the functions <tt>errorFunction</tt> and <tt>errorFunctionComplement</tt>.
	/// </summary>
	/// <param name="a"> the z-value </param>
	/// <returns> the probability of the z value according to the normal pdf </returns>
	static double normalProbability(double a);


	/*
	Returns the error function of the normal distribution.
	*/
	static double errorFunction(double x);
	/*
	Returns the complementary Error function of the normal distribution.
	*/
	static double errorFunctionComplemented(double a);

protected:
	static const double SQRTH;
	static const double MAXLOG;
};


#endif	// _STATS_H_
