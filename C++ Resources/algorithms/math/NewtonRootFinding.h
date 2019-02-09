#pragma once
#include <bits/stdc++.h>
using namespace std;

// Find the root of a function using Newton's Method, given an initial guess x0
// Time Complexity: iters * cost to compute f(x)
template <class T, class F> T newton(T x0, F f, F df, int iters) {
    T cur = x0, next = x0;
    for (int it = 0; it < iters; it++) { cur = next; next = cur - f(cur) / df(cur); }
    return next;
}
