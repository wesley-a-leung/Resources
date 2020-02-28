#pragma once
#include <bits/stdc++.h>
using namespace std;

// Ternary search to find the maximum of a function based on the comparator
// comparator convention is same as priority_queue in STL
// Time Complexity: O(iters * (cost to compute f(x)))

template <class T, class F, class Cmp = less<T>> T ternary_search(T lo, T hi, F f, int iters, Cmp cmp = less<T>()) {
    T m1, m2;
    for (int it = 0; it < iters; it++) {
        m1 = lo + (hi - lo) / 3; m2 = hi - (hi - lo) / 3;
        if (cmp(f(m1), f(m2))) lo = m1;
        else hi = m2;
    };
    return lo + (hi - lo) / 2;
}
