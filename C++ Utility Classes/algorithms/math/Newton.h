#ifndef ALGORITHMS_MATH_NEWTON_H_
#define ALGORITHMS_MATH_NEWTON_H_

#include <bits/stdc++.h>

using namespace std;

const double EPS = 1e-9;

// Finds a root of function f with derivative d based on initial guess x0
double newton(double (*f)(double), double (*df)(double), double x0) {
    double cur = x0, next = x0;
    do {
        cur = next;
        next = cur - f(cur) / df(cur);
    } while (abs(next - cur) > EPS);
    return next;
}

// Finds a root of function f with derivative d based on initial guess x0 by
// repeating newton's approximation a specified number of times
double newton(double (*f)(double), double (*df)(double), double x0, int iterations) {
    double cur = x0, next = x0;
    while (iterations--) {
        cur = next;
        next = cur - f(cur) / df(cur);
    }
    return next;
}

#endif /* ALGORITHMS_MATH_NEWTON_H_ */
