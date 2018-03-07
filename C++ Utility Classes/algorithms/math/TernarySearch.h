#ifndef ALGORITHMS_MATH_TERNARYSEARCH_H_
#define ALGORITHMS_MATH_TERNARYSEARCH_H_

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-6;

/**
 * Performs ternary search to find the minimum of a function.
 *
 * @param f the function
 * @param lo the starting x value
 * @param hi the ending x value
 * @returns a pair containing the x value at the minimum of the function, and the minimum value of the function.
 */
pair<double, double> ternary_search(double (*f)(double), double lo, double hi) {
    double m1, m2;
    do {
        m1 = lo + (hi - lo) / 3;
        m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) hi = m2; // change to f(m1) < f(m2) for minimum, f(m1) > f(m2) for maximum
        else lo = m1;
    } while (abs(hi - lo) >= EPS);
    m1 = lo + (hi - lo) / 2;
    if (abs(m1) < EPS) m1 = 0.0; // correcting -0.0 to 0.0
    return make_pair(m1, f(m1));
}

/**
 * Performs ternary search to find the minimum of a function at integral values.
 *
 * @param f the function
 * @param lo the starting x value
 * @param hi the ending x value
 * @returns a pair containing the x value at the minimum of the function, and the minimum value of the function.
 */
pair<long long, long long> ternary_search(long long (*f)(long long), long long lo, long long hi) {
    long long mid;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (f(mid) < f(mid + 1)) hi = mid; // change to f(mid) < f(mid + 1) for minimum, f(mid) > f(mid + 1) for maximum
        else lo = mid + 1;
    }
    return make_pair(lo, f(lo));
}

#endif /* ALGORITHMS_MATH_TERNARYSEARCH_H_ */
