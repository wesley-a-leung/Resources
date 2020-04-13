#pragma once
#include <bits/stdc++.h>
using namespace std;

// base ^ pow
// Time Complexity: O(log pow)
// If multiplication is an expensive operation, then y = y * y should only be computed when pow > 0
// Required: 0 <= pow
template <class T, class U> T pow2(T base, U pow) {
    T x = 1;
    for (; pow > 0; pow >>= 1, base = base * base) if (pow & 1) x = x * base;
    return x;
}
