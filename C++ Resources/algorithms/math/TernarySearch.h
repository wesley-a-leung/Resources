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

// Ternary search to find the minimum of a function at integral values
// in the range [lo, hi), f(hi) must be defined
// Time Complexity: O(log(hi - lo)) * (cost to compute f(x))
template <class T, class F> T ternary_search(T lo, T hi, F f) {
    static_assert(is_integral<T>::value, "T must be an integral type"); T mid;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (f(mid) < f(mid + 1)) hi = mid; // f(mid) < f(mid + 1) for minimum, f(mid) > f(mid + 1) for maximum
        else lo = mid + 1;
    }
    return lo;
}
