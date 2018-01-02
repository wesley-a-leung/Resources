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

#endif /* ALGORITHMS_MATH_EUCLIDGCD_H_ */
