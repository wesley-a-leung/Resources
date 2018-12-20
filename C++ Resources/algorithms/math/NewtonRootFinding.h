#pragma once
#include <bits/stdc++.h>
using namespace std;

// Find the root of a function using Newton's Method, given an initial guess x0
// Time Complexity: O(-log(EPS)) * cost to compute f(x), faster in practice
template <class T, class F> T newton(F f, F df, T x0, T EPS) {
    T cur = x0, next = x0;
    do { cur = next; next = cur - f(cur) / df(cur); } while (abs(next - cur) > EPS);
    return next;
}
