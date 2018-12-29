#pragma once
#include <bits/stdc++.h>
using namespace std;

// Ternary search to find the minimum of a function
// Time Complexity: O(log((hi - lo) / EPS)) * (cost to compute f(x))
template <class T, class F> T ternary_search(F f, T lo, T hi, T EPS) {
    T m1, m2;
    do {
        m1 = lo + (hi - lo) / 3; m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) hi = m2; // f(m1) < f(m2) for minimum, f(m1) > f(m2) for maximum
        else lo = m1;
    } while (abs(hi - lo) >= EPS);
    return lo + (hi - lo) / 2;
}

// Ternary search to find the minimum of a function at integral values
// Time Complexity: O(log(hi - lo)) * (cost to compute f(x))
template <class T, class F> T ternary_search(F f, T lo, T hi) {
    static_assert(is_integral<T>::value, "T must be an integral type"); T mid;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (f(mid) < f(mid + 1)) hi = mid; // f(mid) < f(mid + 1) for minimum, f(mid) > f(mid + 1) for maximum
        else lo = mid + 1;
    }
    return lo;
}