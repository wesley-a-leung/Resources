#pragma once
#include <bits/stdc++.h>
using namespace std;

// returns the first value in the range [lo, hi) where f(x) is true
// if no value in [lo, hi) satisfies f(x), then it returns hi
// assumes that all values where f(x) is true are greater than all values where f(x) is false
// Time Complexity: O(log (hi - lo)) * (cost to compute f(x))
template <class T, class F> T getFirst(T lo, T hi, F f) {
    while (lo < hi) {
        T mid = lo + (hi - lo) / 2;
        if (f(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

// returns the last value in the range [lo, hi) where f(x) is true
// if no value in [lo, hi) satisfies f(x), then it returns lo - 1
// assumes that all values where f(x) is true are less than all values where f(x) is false
// Time Complexity: O(log (hi - lo)) * (cost to compute f(x))
template <class T, class F> T getLast(T lo, T hi, F f) {
    hi--;
    while (lo <= hi) {
        T mid = lo + (hi - lo) / 2;
        if (f(mid)) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}
