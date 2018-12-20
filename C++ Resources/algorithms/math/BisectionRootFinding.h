#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the root of a function using a bisection search
// Time Complexity: O(log((hi - lo) / EPS)) * (cost to compute f(x))
template <class T, class F> T bisection_root_finding(F f, T lo, T hi, T EPS) {
    T mid;
    do {
        mid = lo + (hi - lo) / 2;
        if (f(mid) < 0) lo = mid;
        else hi = mid;
    } while (hi - lo >= EPS);
    return mid;
}
