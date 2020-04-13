#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the root of a function using a bisection search
// Time Complexity: O(iters * (cost to compute f(x)))
template <class T, class F> T bisectionSearch(T lo, T hi, F f, int iters) {
    for (int it = 0; it < iters; it++) {
        T mid = lo + (hi - lo) / 2;
        if (f(mid) < 0) lo = mid;
        else hi = mid;
    }
    return lo + (hi - lo) / 2;
}
