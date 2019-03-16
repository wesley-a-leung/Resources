#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(log max(a, b))

template <class T> T gcd(T a, T b) { return b == 0 ? abs(a) : gcd(b, a % b); }

template <class T> T lcm(T a, T b) { return a == 0 && b == 0 ? 0 : abs(a / gcd(a, b) * b); }

// Extended Euclidean Algorithm to compute x and y, where ax + by = gcd(a, b)
template <class T> T EEA(T a, T b, T &x, T &y) {
    T g = b, r = a; x = 0, y = 1;
    while (r != 0) { T q = g / r; g %= r; swap(g, r); x -= q * y; swap(x, y); }
    return g;
}

// Computes the multiplicative inverse of a in Zm
// Require: 0 <= a, a < m
template <class T> T mulInv(T a, T m) {
    T x, y;
    if (EEA(a, m, x, y) != 1) return -1; // no inverse
    x %= m; return x < 0 ? x + m : x;
}

// Solves the linear congruence ax = c mod m
// Return the value of x, and the modulus of the answer
// Required: 0 <= a < m, 0 <= c < m
template <class T> pair<T, T> solveCongruence(T a, T c, T m) {
    T x, y, g = EEA(a, m, x, y);
    if (c % g != 0) return make_pair(-1, m / g); // no solution
    x = (x % m + m) % m; x = (x * c / g) % (m / g); return make_pair(x, m / g); 
}

// Solves the Linear Diophantine Equation ax + by = c
// Returns true if there is a solution, false otherwise
// The solutions are stored in the pairs x and y
// All pairs of integers (s, t) where s = x.first + k * x.second and t = y.first + k * y.second
// for all integers k are solutions
// Edge cases:
//   1. a == 0 && b == 0 is satisfied by all integer pairs (s, t) if c == 0, no solutions otherwise
//   2. a == 0 is satisfied by all integer pairs (s, t) with t = c / b if c is divisible by b, no solutions otherwise
//   3. b == 0 is satisfied by all integer pairs (s, t) with s = c / a if c is divisible by a, no solutions otherwise
template <class T> bool LDE(T a, T b, T c, pair<T, T> &x, pair<T, T> &y) {
    assert(a != 0 && b != 0); T xg, yg, g = EEA(a, b, xg, yg);
    if (c % g != 0) return false;
    x = make_pair(xg * (c / g), b / g); y = make_pair(yg * (c / g), -a / g); return true; 
}

// Generalized Chinese Remainder Theorem to find the solution to x mod lcm(a.second, b.second)
// given x = a.first mod a.second and x = b.first mod b.second
// Returns the pair {x, lcm(a.second, b.second)}
// Required: 0 <= a.first < a.second, 0 <= b.first < b.second
template <class T> pair<T, T> CRT(pair<T, T> a, pair<T, T> b) {
    T g = gcd(a.second, b.second), l = a.second / g * b.second;
    if ((b.first - a.first) % g != 0) return make_pair(-1, l); // no solution
    T A = a.second / g, B = b.second / g, mul = (b.first - a.first) / g * mulInv(A % B, B) % B;
    return make_pair(((mul * a.second + a.first) % l + l) % l, l);
}
