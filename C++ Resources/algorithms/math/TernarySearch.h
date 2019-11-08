#pragma once
#include <bits/stdc++.h>
using namespace std;

// Ternary search to find the minimum of a function
// Time Complexity: O(iters * (cost to compute f(x)))
template <class T, class F> T ternary_search(T lo, T hi, F f, int iters) {
    T m1, m2;
    for (int it = 0; it < iters; it++) {
        m1 = lo + (hi - lo) / 3; m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) hi = m2; // f(m1) < f(m2) for minimum, f(m1) > f(m2) for maximum
        else lo = m1;
    };
    return lo + (hi - lo) / 2;
}
