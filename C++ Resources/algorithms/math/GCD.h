#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(log max(a, b))

template <class T> T gcd(T a, T b) { return b == 0 ? abs(a) : gcd(b, a % b); }

template <class T> T lcm(T a, T b) { return abs(a / gcd(a, b) * b); }

// Extended Euclidean Algorithm to compute x and y, where ax + by = gcd(a, b)
template <class T> T EEA(T a, T b, T &x, T &y) {
    if (b == 0) { x = 1; y = 0; return abs(a); }
    T x1, y1, g = EEA(b, a % b, x1, y1); y = x1 - (a / b) * y1; x = y1; return g;
}

// Computes the multiplicative inverse of a in Zn
template <class T> T multInv(T a, T n) {
    T x, y;
    if (EEA(a, n, x, y) != 1) return -1; // no inverse
    return (x % n + n) % n;
}

// Solves the linear congruence ax = c mod m
// Return the value of x, and the modulus of the answer
template <class T> pair<T, T> solveCongruence(T a, T c, T m) {
    T x, y, g = EEA(a, m, x, y);
    if (c % g != 0) return make_pair(-1, -1); // no solution
    x = (x % m + m) % m; x = (x * c / g) % (m / g); return make_pair(x, m / g); 
}

// Generalized Chinese Remainder Theorem to find the solution to x mod lcm(a.second, b.second)
// given x = a.first mod a.second and x = b.first mod b.second
// Returns the pair {x, lcm(a.second, b.second)}
template <class T> pair<T, T> CRT(pair<T, T> a, pair<T, T> b) {
    T g = gcd(a.second, b.second), l = a.second / g * b.second;
    if ((b.first - a.first) % g != 0) return make_pair(-1, -1); // no solution
    T A = a.second / g, B = b.second / g, mul = (b.first - a.first) / g * multInv(A % B, B) % B;
    return make_pair(((mul * a.second + a.first) % l + l) % l, l);
}
