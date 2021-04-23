#pragma once
#include <bits/stdc++.h>
using namespace std;

// Type templated greatest common divisor function
// Always returns a non negative integer
// Template Arguments:
//   T: the type of a and b
// Function Arguments:
//   a: the first value
//   b: the second value
// Return Value: the greatest common divisor of a and b
// In practice, has a small constant
// Time Complexity: O(log(max(a, b)))
// Memory Complexity: O(1)
// Tested:
//   https://hackerrank.com/contests/projecteuler/challenges/euler005/problem
template <class T> T gcd(T a, T b) { return b == 0 ? abs(a) : gcd(b, a % b); }

// Type templated lowest common multiple function
// Always returns a non negative integer
// Template Arguments:
//   T: the type of a and b
// Function Arguments:
//   a: the first value
//   b: the second value
// Return Value: the lowest common multiple of a and b
// In practice, has a small constant
// Time Complexity: O(log(max(a, b)))
// Memory Complexity: O(1)
// Tested:
//   https://hackerrank.com/contests/projecteuler/challenges/euler005/problem
template <class T> T lcm(T a, T b) {
  return a == 0 && b == 0 ? 0 : abs(a / gcd(a, b) * b);
}

// Extended Euclidean Algorithm to compute x and y, where ax + by = gcd(a, b)
// Template Arguments:
//   T: the type of a and b
// Function Arguments:
//   a: the first value
//   b: the second value
//   x: a reference to a variable to store x
//   y: a reference to a variable to store y
// Return Value: the greatest common divisor of a and b
// In practice, has a small constant
// Time Complexity: O(log(max(a, b)))
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/modinv
//   https://www.spoj.com/problems/CEQU/
//   https://codeforces.com/problemsets/acmsguru/problem/99999/106
template <class T> T EEA(T a, T b, T &x, T &y) {
  T xx = y = 0, yy = x = 1; while (b != 0) {
    T q = a / b; a %= b; swap(a, b);
    x -= q * xx; swap(x, xx); y -= q * yy; swap(y, yy);
  }
  if (a < 0) { a = -a; x = -x; y = -y; }
  return a;
}

// Computes the multiplicative inverse of a in Zm
// Inverse only exists if gcd(a, m) == 1
// Template Arguments:
//   T: the type of a and m
// Function Arguments:
//   a: the value to find the inverse of, must be in the range [0, m)
//   m: the mod, must be positive
// Return Value: the multiplicative inverse of a in Zm, -1 if no inverse
// In practice, has a small constant
// Time Complexity: O(log m)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/modinv
template <class T> T mulInv(T a, T m) {
  if (a == 0) return -1;
  T x, y; if (EEA(a, m, x, y) != 1) return -1;
  x %= m; return x < 0 ? x + m : x;
}

// Solves the linear congruence ax = c mod m
// Template Arguments:
//   T: the type of a, c, and m
// Function Arguments:
//   a: must be in the range [0, m)
//   c: must be in the range [0, m)
//   m: the mod, must be positive
// Return Value: a pair containing x, and the modulus of the answer (equal to
//   m / gcd(a, m)), x is -1 if there is no solution
// In practice, has a small constant
// Time Complexity: O(log m)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> pair<T, T> solveCongruence(T a, T c, T m) {
  T x, y, g = EEA(a, m, x, y);
  if (c % g != 0) return make_pair(-1, m / g);
  x = (x % m + m) % m; x = (x * c / g) % (m / g); return make_pair(x, m / g); 
}

// Solves the Linear Diophantine Equation ax + by = c
// All pairs of integers (s, t) where s = x.first + k * x.second
//   and t = y.first + k * y.second for all integers k are solutions
// Edge cases:
//   1. a == 0 && b == 0 is satisfied by all integer pairs (s, t) if c == 0,
//     no solutions otherwise
//   2. a == 0 is satisfied by all integer pairs (s, t) with t = c / b if c is
//     divisible by b, no solutions otherwise
//   3. b == 0 is satisfied by all integer pairs (s, t) with s = c / a if c is
//     divisible by a, no solutions otherwise
// Template Arguments:
//   T: the type of a, b, c
// Function Arguments:
//   a: the first value
//   b: the second value
//   c: the value of ax + by
//   x: a reference to a pair storing x and its mod (equal to b / gcd(a, b))
//   y: a reference to a pair storing y and its mod (equal to -a / gcd(a, b))
// Return Value: true if there is a solution, false otherwise
// In practice, has a small constant
// Time Complexity: O(log(max(a, b)))
// Memory Complexity: O(1)
// Tested:
//   https://www.spoj.com/problems/CEQU/
//   https://codeforces.com/problemsets/acmsguru/problem/99999/106
template <class T> bool LDE(T a, T b, T c, pair<T, T> &x, pair<T, T> &y) {
  assert(a != 0 && b != 0); T xg, yg, g = EEA(a, b, xg, yg);
  if (c % g != 0) return false;
  x = make_pair(xg * (c / g), b / g); y = make_pair(yg * (c / g), -a / g);
  return true; 
}

// Generalized Chinese Remainder Theorem to find the solution to
//   x mod lcm(a.second, b.second) given x = a.first mod a.second
//   and x = b.first mod b.second
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   a: the first value and its associated mod (0 <= a.first < a.second)
//   b: the second value and its associated mod (0 <= b.first < b.second)
// Return Value: the pair x and lcm(a.second, b.second) where
//   0 <= x < lcm(a.second, b.second), x is -1 if there is no solution
// In practice, has a small constant
// Time Complexity: O(log(max(a.second, b.second)))
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/generalchineseremainder
template <class T> pair<T, T> CRT(pair<T, T> a, pair<T, T> b) {
  T g = gcd(a.second, b.second), l = a.second / g * b.second;
  if ((b.first - a.first) % g != 0) return make_pair(-1, l);
  T A = a.second / g, B = b.second / g;
  T mul = (b.first - a.first) / g * mulInv(A % B, B) % B;
  return make_pair(((mul * a.second + a.first) % l + l) % l, l);
}
