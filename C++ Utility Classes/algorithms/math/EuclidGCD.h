#ifndef ALGORITHMS_MATH_EUCLIDGCD_H_
#define ALGORITHMS_MATH_EUCLIDGCD_H_

#include <bits/stdc++.h>
using namespace std;

// non-recursive implementation
/**
 * Returns the greatest common divisor of 2 integers.
 *
 * @param  p the first integer
 * @param  q the second integer
 * @return greatest common divisor of {@code p} and {@code q}
 */
int gcd(int p, int q) {
    while (q != 0) {
        int temp = q;
        q = p % q;
        p = temp;
    }
    return abs(p);
}
// recursive implementation
/**
 * Returns the greatest common divisor of 2 integers.
 *
 * @param  p the first integer
 * @param  q the second integer
 * @return greatest common divisor of {@code p} and {@code q}
 */
int gcdRec(int p, int q) {
    return q == 0 ? abs(p) : gcdRec(q, p % q);
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
int gcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int g = gcd(b, a % b, x1, y1);
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
int multInv(int a, int n) {
    int x, y;
    assert(gcd(a, n, x, y) == 1); // otherwise, there is no inverse
    return (x % n + n) % n;
}

#endif /* ALGORITHMS_MATH_EUCLIDGCD_H_ */
