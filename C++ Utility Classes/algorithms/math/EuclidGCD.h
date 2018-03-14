#ifndef ALGORITHMS_MATH_EUCLIDGCD_H_
#define ALGORITHMS_MATH_EUCLIDGCD_H_

#include <bits/stdc++.h>
using namespace std;

// non-recursive implementation
/**
 * Returns the greatest common divisor of 2 integers.
 *
 * @param  a the first integer
 * @param  b the second integer
 * @return greatest common divisor of {@code a} and {@code b}
 */
template <typename T>
T gcd(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return abs(a);
}

// recursive implementation
/**
 * Returns the greatest common divisor of 2 integers.
 *
 * @param  a the first integer
 * @param  b the second integer
 * @return greatest common divisor of {@code a} and {@codebq}
 */
template <typename T>
T gcdRec(T a, T b) {
    return b == 0 ? abs(a) : gcdRec(b, a % b);
}

/**
 * Extended Euclidean Algorithm
 *
 * Returns the greatest common divisor of 2 integers and also determine
 * the values x and y such that ax + by = gcd(a, b).
 *
 * @param  a the first integer
 * @param  b the second integer
 * @param  x the reference to the multiplier of a
 * @param  y the reference to the multiplier of b
 * @return greatest common divisor of {@code a} and {@code b}
 */
template <typename T>
T gcd(T a, T b, T &x, T &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return abs(a);
    }
    T x1, y1;
    T g = gcd(b, a % b, x1, y1);
    y = x1 - (a / b) * y1;
    x = y1;
    return g;
}

/**
 * Returns the multiplicative inverse of a in Zn.
 *
 * @param a the number to determine the multiplicative inverse.
 * @param n the integer class
 * @return the multiplicative inverse of a in Zn.
 */
template <typename T>
T multInv(T a, T n) {
    T x, y;
    assert(gcd(a, n, x, y) == 1); // otherwise, there is no inverse
    return (x % n + n) % n;
}

/**
 * Solves the linear congruence ax = c mod m.
 *
 * @param a the linear coefficient
 * @param c the constant term
 * @param m the modulus of the system
 * @return a pair containing the integer x and the modulus of the answer.
 */
template <typename T>
pair<T, T> solveCongruence(T a, T c, T m) {
    T x, y;
    T g = gcd(a, m, x, y);
    assert(c % g == 0); // otherwise there is no solution
    x = (x % m + m) % m;
    x = (x * c / g) % (m / g);
    return make_pair(x, m / g);
}

#endif /* ALGORITHMS_MATH_EUCLIDGCD_H_ */
