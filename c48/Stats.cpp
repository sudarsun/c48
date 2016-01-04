#include "Stats.h"
#include "core/statistics.h"
#include <iostream>
#include <algorithm>
#define PI 3.14
#include <cmath>

const double Stats::SQRTH = 7.07106781186547524401E-1;
const double Stats::MAXLOG = 7.09782712893383996732E2;


/// <summary>
///***********************************************
/// COEFFICIENTS FOR METHOD normalInverse() *
///  ************************************************
/// </summary>
/* approximation for 0 <= |y - 0.5| <= 3/8 */
static double_array const P0 = { -5.99633501014107895267E1, 9.80010754185999661536E1, -5.66762857469070293439E1, 1.39312609387279679503E1, -1.23916583867381258016E0 };
static double_array const Q0 = { 1.95448858338141759834E0, 4.67627912898881538453E0, 8.63602421390890590575E1, -2.25462687854119370527E2, 2.00260212380060660359E2, -8.20372256168333339912E1, 1.59056225126211695515E1, -1.18331621121330003142E0 };

/*
* Approximation for interval z = sqrt(-2 log y ) between 2 and 8 i.e., y
* between exp(-2) = .135 and exp(-32) = 1.27e-14.
*/
static double_array const P1 = { 4.05544892305962419923E0, 3.15251094599893866154E1, 5.71628192246421288162E1, 4.40805073893200834700E1, 1.46849561928858024014E1, 2.18663306850790267539E0, -1.40256079171354495875E-1, -3.50424626827848203418E-2, -8.57456785154685413611E-4 };
static double_array const Q1 = { 1.57799883256466749731E1, 4.53907635128879210584E1, 4.13172038254672030440E1, 1.50425385692907503408E1, 2.50464946208309415979E0, -1.42182922854787788574E-1, -3.80806407691578277194E-2, -9.33259480895457427372E-4 };


/*
* Approximation for interval z = sqrt(-2 log y ) between 8 and 64 i.e., y
* between exp(-32) = 1.27e-14 and exp(-2048) = 3.67e-890.
*/
static double_array const P2 = { 3.23774891776946035970E0, 6.91522889068984211695E0, 3.93881025292474443415E0, 1.33303460815807542389E0, 2.01485389549179081538E-1, 1.23716634817820021358E-2, 3.01581553508235416007E-4, 2.65806974686737550832E-6, 6.23974539184983293730E-9 };
static double_array const Q2 = { 6.02427039364742014255E0, 3.67983563856160859403E0, 1.37702099489081330271E0, 2.16236993594496635890E-1, 1.34204006088543189037E-2, 3.28014464682127739104E-4, 2.89247864745380683936E-6, 6.79019408009981274425E-9 };

double Stats::addErrs(double N, double e, float CF) {

    // Ignore stupid values for CF
    if (CF > 0.5) {
        std::cout << "WARNING: confidence value for pruning too high. Error estimate not modified.";
        return 0;
    }

    // Check for extreme cases at the low end because the
    // normal approximation won't work
    if (e < 1) {

        // Base case (i.e. e == 0) from documenta Geigy Scientific
        // Tables, 6th edition, page 185
        double base = N * (1 - pow(CF, 1 / N));
        if (e == 0) {
            return base;
        }

        // Use linear interpolation between 0 and 1 like C4.5 does
        return base + e * (addErrs(N, 1, CF) - base);
    }

    // Use linear interpolation at the high end (i.e. between N - 0.5
    // and N) because of the continuity correction
    if (e + 0.5 >= N) {

        // Make sure that we never return anything smaller than zero
        return std::max(N - e, 0.0);
    }

    // Get z-score corresponding to CF
    double z = normalInverse(1 - CF);

    // Compute upper limit of confidence interval
    double f = (e + 0.5) / N;
    double r = (f + (z * z) / (2 * N) + z * sqrt((f / N) - (f * f / N) + (z * z / (4 * N * N)))) / (1 + (z * z) / N);

    return (r * N) - e;
}

double Stats::normalInverse(double y0) {

    double x, y, z, y2, x0, x1;
    int code;

    const double s2pi = sqrt(2.0 * PI);

    if (y0 <= 0.0) {
        throw "IllegalArgumentException()";
    }
    if (y0 >= 1.0) {
        throw "IllegalArgumentException()";
    }
    code = 1;
    y = y0;
    if (y > (1.0 - 0.13533528323661269189))
    { // 0.135... = exp(-2)
        y = 1.0 - y;
        code = 0;
    }

    if (y > 0.13533528323661269189) {
        y = y - 0.5;
        y2 = y * y;
        x = y + y * (y2 * polevl(y2, P0, 4) / p1evl(y2, Q0, 8));
        x = x * s2pi;
        return (x);
    }

    x = sqrt(-2.0 * log(y));
    x0 = x - log(x) / x;

    z = 1.0 / x;
    if (x < 8.0) {
        x1 = z * polevl(z, P1, 8) / p1evl(z, Q1, 8);
    }
    else {
        x1 = z * polevl(z, P2, 8) / p1evl(z, Q2, 8);
    }
    x = x0 - x1;
    if (code != 0) {
        x = -x;
    }
    return (x);
}

/// <summary>
/// Evaluates the given polynomial of degree <tt>N</tt> at <tt>x</tt>.
/// Evaluates polynomial when coefficient of N is 1.0. Otherwise same as
/// <tt>polevl()</tt>.
///
/// <pre>
///                     2          N
/// y  =  C  + C x + C x  +...+ C x
///        0    1     2          N
///
/// Coefficients are stored in reverse order:
///
/// coef[0] = C  , ..., coef[N] = C  .
///            N                   0
/// </pre>
///
/// The function <tt>p1evl()</tt> assumes that <tt>coef[N] = 1.0</tt> and is
/// omitted from the array. Its calling arguments are otherwise the same as
/// <tt>polevl()</tt>.
/// <para>
/// In the interest of speed, there are no checks for out of bounds arithmetic.
///
/// </para>
/// </summary>
/// <param name="x"> argument to the polynomial. </param>
/// <param name="coef"> the coefficients of the polynomial. </param>
/// <param name="N"> the degree of the polynomial. </param>
double Stats::p1evl(double x, const double_array &coef, int N) {

    double ans;
    ans = x + coef[0];

    for (int i = 1; i < N; i++) {
        ans = ans * x + coef[i];
    }

    return ans;
}

/// <summary>
/// Evaluates the given polynomial of degree <tt>N</tt> at <tt>x</tt>.
///
/// <pre>
///                     2          N
/// y  =  C  + C x + C x  +...+ C x
///        0    1     2          N
///
/// Coefficients are stored in reverse order:
///
/// coef[0] = C  , ..., coef[N] = C  .
///            N                   0
/// </pre>
///
/// In the interest of speed, there are no checks for out of bounds arithmetic.
/// </summary>
/// <param name="x"> argument to the polynomial. </param>
/// <param name="coef"> the coefficients of the polynomial. </param>
/// <param name="N"> the degree of the polynomial. </param>
double Stats::polevl(double x, const double_array &coef, int N) {

    double ans;
    ans = coef[0];

    for (int i = 1; i <= N; i++) {
        ans = ans * x + coef[i];
    }

    return ans;
}

double Stats::normalProbability(double a) {

    double x, y, z;

    x = a * SQRTH;
    z = abs(x);

    if (z < SQRTH) {
        y = 0.5 + 0.5 * errorFunction(x);
    }
    else {
        y = 0.5 * errorFunctionComplemented(z);
        if (x > 0) {
            y = 1.0 - y;
        }
    }
    return y;
}


double Stats::errorFunction(double x) {
    double y, z;
    const double_array T = { 9.60497373987051638749E0, 9.00260197203842689217E1, 2.23200534594684319226E3, 7.00332514112805075473E3, 5.55923013010394962768E4 };
    const double_array U = { 3.35617141647503099647E1, 5.21357949780152679795E2, 4.59432382970980127987E3, 2.26290000613890934246E4, 4.92673942608635921086E4 };

    if (abs(x) > 1.0) {
        return (1.0 - errorFunctionComplemented(x));
    }
    z = x * x;
    y = x * polevl(z, T, 4) / p1evl(z, U, 5);
    return y;
}

double Stats::errorFunctionComplemented(double a) {
    double x, y, z, p, q;

    double_array P = { 2.46196981473530512524E-10, 5.64189564831068821977E-1, 7.46321056442269912687E0, 4.86371970985681366614E1, 1.96520832956077098242E2, 5.26445194995477358631E2, 9.34528527171957607540E2, 1.02755188689515710272E3, 5.57535335369399327526E2 };
    double_array Q = { 1.32281951154744992508E1, 8.67072140885989742329E1, 3.54937778887819891062E2, 9.75708501743205489753E2, 1.82390916687909736289E3, 2.24633760818710981792E3, 1.65666309194161350182E3, 5.57535340817727675546E2 };

    double_array R = { 5.64189583547755073984E-1, 1.27536670759978104416E0, 5.01905042251180477414E0, 6.16021097993053585195E0, 7.40974269950448939160E0, 2.97886665372100240670E0 };
    double_array S = { 2.26052863220117276590E0, 9.39603524938001434673E0, 1.20489539808096656605E1, 1.70814450747565897222E1, 9.60896809063285878198E0, 3.36907645100081516050E0 };

    if (a < 0.0) {
        x = -a;
    }
    else {
        x = a;
    }

    if (x < 1.0) {
        return 1.0 - errorFunction(a);
    }

    z = -a * a;

    if (z < -MAXLOG) {
        if (a < 0) {
            return (2.0);
        }
        else {
            return (0.0);
        }
    }

    z = exp(z);

    if (x < 8.0) {
        p = polevl(x, P, 8);
        q = p1evl(x, Q, 8);
    }
    else {
        p = polevl(x, R, 5);
        q = p1evl(x, S, 6);
    }

    y = (z * p) / q;

    if (a < 0) {
        y = 2.0 - y;
    }

    if (y == 0.0) {
        if (a < 0) {
            return 2.0;
        }
        else {
            return (0.0);
        }
    }
    return y;
}
