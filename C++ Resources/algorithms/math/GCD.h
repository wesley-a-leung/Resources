#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(log max(a, b))

// Computes gcd(a, b)
template <typename T> T gcd(T a, T b) {
    while (b != 0) { T temp = b; b = a % b; a = temp; }
    return abs(a);
}

// Computes gcd(a b)
template <typename T> T gcdRec(T a, T b) { return b == 0 ? abs(a) : gcdRec(b, a % b); }

// Extended Euclidean Algorithm to compute x and y, where ax + by = gcd(a, b)
template <typename T> T EEA(T a, T b, T &x, T &y) {
    if (b == 0) { x = 1; y = 0; return abs(a); }
    T x1, y1, g = EEA(b, a % b, x1, y1); y = x1 - (a / b) * y1; x = y1; return g;
}

// Computes the multiplicative inverse of a in Zn
template <typename T> T multInv(T a, T n) {
    T x, y; assert(EEA(a, n, x, y) == 1); // otherwise, there is no inverse
    return (x % n + n) % n;
}

// Solves the linear congruence ax = c mod m
// return the value of x, and the modulus of the answer
template <typename T> pair<T, T> solveCongruence(T a, T c, T m) {
    T x, y, g = EEA(a, m, x, y); assert(c % g == 0); // otherwise there is no solution
    x = (x % m + m) % m; x = (x * c / g) % (m / g); return make_pair(x, m / g); 
}
