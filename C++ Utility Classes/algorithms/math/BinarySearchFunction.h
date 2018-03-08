#ifndef ALGORITHMS_MATH_BINARYSEARCHFUNCTION_H_
#define ALGORITHMS_MATH_BINARYSEARCHFUNCTION_H_

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-6;

/**
 * Performs binary search to find the root of a function.
 *
 * @param f the function
 * @param lo the starting x value
 * @param hi the ending x value
 * @returns the x value of the root of the function.
 */
double binary_search(double (*f)(double), double lo, double hi) {
    double mid;
    do {
        mid = lo + (hi - lo) / 2;
        if (f(mid) < 0) lo = mid;
        else hi = mid;
    } while (hi - lo >= EPS);
    return mid;
}

#endif /* ALGORITHMS_MATH_BINARYSEARCHFUNCTION_H_ */
