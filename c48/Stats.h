#ifndef _STATS_H_
#define _STATS_H_
#include <vector>
#include "core/Typedefs.h"

/**
 * Class implementing a statistical routine needed by C48 to
 * compute its error estimate.
 */
class Stats {

public:
    /**
      * Computes estimated extra error for given total number of instances
      * and error using normal approximation to binomial distribution
      * (and continuity correction).
      *
      * @param N number of instances
      * @param e observed error
      * @param CF confidence value
      */
    static double addErrs(double N, double e, float CF);

    /**
     * Returns the value, x, for which the area under the Normal
     * (Gaussian) probability density function (integrated from minus infinity to
     * x) is equal to the argument y (assumes mean is zero,
     * variance is one).
     * For small arguments 0 < y < exp(-2), the program computes
     * z = sqrt( -2.0 * log(y) ); then the approximation is
     * x = z - log(z)/z  - (1/z) P(1/z) / Q(1/z). There are two rational
     * functions P/Q, one for 0 < y < exp(-32) and the other for
     * y up to exp(-2). For larger arguments,
     * w = y - 0.5, and x/sqrt(2pi) = w + w**3 R(w**2)/S(w**2)).
     *
     * @param y0 the area under the normal pdf
     * @return the z-value
     */
    static double normalInverse(double y0);

    /**
    * Evaluates the given polynomial of degree N at x.
    * Evaluates polynomial when coefficient of N is 1.0.
    * Otherwise same as polevl().
    *
    *                     2          N
    * y  =  C  + C x + C x  +...+ C x
    *        0    1     2          N
    *
    * Coefficients are stored in reverse order:
    *
    * coef[0] = C  , ..., coef[N] = C  .
    *            N                   0
    *
    * The function p1evl() assumes that coef[N] = 1.0 and is
    * omitted from the array.  Its calling arguments are
    * otherwise the same as polevl().
    *
    * In the interest of speed, there are no checks for out of bounds arithmetic.
    *
    * @param x argument to the polynomial.
    * @param coef the coefficients of the polynomial.
    * @param N the degree of the polynomial.
    */
    static double p1evl(double x, const double_array &coef, int N);

    /**
    * Evaluates the given polynomial of degree N at x.
    *
    *                     2          N
    * y  =  C  + C x + C x  +...+ C x
    *        0    1     2          N
    *
    * Coefficients are stored in reverse order:
    *
    * coef[0] = C  , ..., coef[N] = C  .
    *            N                   0
    *
    * In the interest of speed, there are no checks for out of bounds arithmetic.
    *
    * @param x argument to the polynomial.
    * @param coef the coefficients of the polynomial.
    * @param N the degree of the polynomial.
    */
    static double polevl(double x, const double_array &coef, int N);

    /**
     * Returns the area under the Normal (Gaussian) probability density
     * function, integrated from minus infinity to x
     * (assumes mean is zero, variance is one).
     *
     *                            x
     *                             -
     *                   1        | |          2
     *  normal(x)  = ---------    |    exp( - t /2 ) dt
     *               sqrt(2pi)  | |
     *                           -
     *                          -inf.
     *
     *             =  ( 1 + erf(z) ) / 2
     *             =  erfc(z) / 2
     *
     * where z = x/sqrt(2).
     * Computation is via the functions errorFunction and errorFunctionComplement.
     *
     * @param a the z-value
     * @return the probability of the z value according to the normal pdf
     */
    static double normalProbability(double a);

    /**
     * Returns the error function of the normal distribution.
     *
     * @param x argument to the error function.
     */
    static double errorFunction(double x);

    /**
     * Returns the complementary Error function of the normal distribution.
     *
     * @param a argument to the error function.
     */
    static double errorFunctionComplemented(double a);

protected:

    /** Some constants */
    static const double SQRTH;
    static const double MAXLOG;
};

#endif    // _STATS_H_
