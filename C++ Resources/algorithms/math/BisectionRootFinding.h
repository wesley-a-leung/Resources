#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the root of a function using a bisection search
// Time Complexity: O(log(hi - lo)) * (cost to compute f(x)) * iters
template <class T, class F> T bisectionSearch(T lo, T hi, F f, int iters) {
    T mid;
    for (int it = 0; it < iters; it++) {
        mid = lo + (hi - lo) / 2;
        if (f(mid) < 0) lo = mid;
        else hi = mid;
    }
    return mid;
}
