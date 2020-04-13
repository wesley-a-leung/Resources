#pragma once
#include <bits/stdc++.h>
using namespace std;

// returns the first value in the range [lo, hi) where f(x) == key
// if there is no value in [lo, hi) where f(x) == key, then it returns -1
// assumes that f(x) is uniform and non decreasing in the range [lo, hi)
// Time Complexity: O(log log (hi - lo)) * (cost to compute f(x))
template <class T, class F> T interpolationSearch(T lo, T hi, F f, T key) {
    hi--;
    while (lo < hi && f(lo) != f(hi) && f(lo) <= key && key <= f(hi)) {
        T guess = ((key - f(lo)) / (f(hi) - f(lo))) * (hi - lo) + lo;
        auto fguess = f(guess);
        if (fguess < key) lo = guess + 1;
        else if (fguess > key) hi = guess - 1;
        else return guess;
    }
    if (key == f(lo)) return lo;
    return -1;
}
