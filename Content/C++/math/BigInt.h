#pragma once
#include <bits/stdc++.h>
#include "FFT.h"
using namespace std;

// Big Integer
// Constructor Arguments:
//   s: a string of digits 
//   v: a long long
// Functions:
//   read(s): reads a BigInt from a string
//   write(s): write a BigInt to a string
//   >>, <<: input and output operators
//   <, <=, >, >=, ==, !=: comparison operators
//   isZero(): returns true if the big integer is 0
//   abs(): returns the absolute value
//   ++, --, +, +=, -, -=, *, *=, /, /=, %, %=: standard arithmetic operators
//   divMod(a1, b1): returns a pair with the quotient and remainder of
//     a1 divided by b1
// In practice, multiplication has a moderate constant, all other functions
//   have a small constant
// Time Complexity:
//   constructor: O(N)
//   isZero: O(1)
//   read, write, >>, <<, <, <=, >, >=, ==, !=, abs, ++, --, +, +=, -, -=: O(N)
//   *, *=: O(N log N)
//   /, /=, % (long long variants): O(N)
//   /, /=, %, %= (BigInt variants): O(N^2)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/aplusb2
//   https://dmoj.ca/problem/atimesb
//   https://dmoj.ca/problem/ccc96s2
//   https://dmoj.ca/problem/ccc97s5
struct BigInt {
  using T = long long; static constexpr const int DIG = 4;
  static constexpr const T BASE = pow(10, DIG);
  vector<T> a; int sign;
  void trim() {
    while (!a.empty() && a.back() == 0) a.pop_back();
    if (a.empty()) sign = 1;
  }
  void read(const string &s) {
    sign = 1; a.clear(); int pos = 0;
    for (; pos < int(s.size()) && (s[pos] == '-' || s[pos] == '+'); ++pos)
      if (s[pos] == '-') sign = -sign;
    for (int i = int(s.size()) - 1; i >= pos; i -= DIG) {
      T x = 0;
      for (int j = max(pos, i - DIG + 1); j <= i; j++) x = x * 10 + s[j] - '0';
      a.push_back(x);
    }
    trim();
  }
  friend istream& operator >> (istream &stream, BigInt &v) {
    string s; stream >> s; v.read(s); return stream;
  }
  string write() const {
    string ret = ""; if (sign == -1) ret.push_back('-');
    for (char c : to_string(a.empty() ? 0 : a.back())) ret.push_back(c);
    for (int i = int(a.size()) - 2; i >= 0; i--) {
      string s = to_string(a[i]);
      for (int j = int(s.size()); j < DIG; j++) ret.push_back('0');
      for (char c : s) ret.push_back(c);
    }
    return ret;
  }
  friend ostream& operator << (ostream &stream, const BigInt &v) {
    stream << v.write(); return stream;
  }
  bool operator < (const BigInt &v) const {
    if (sign != v.sign) return sign < v.sign;
    if (int(a.size()) != int(v.a.size()))
      return int(a.size()) * sign < int(v.a.size()) * v.sign;
    for (int i = int(a.size()) - 1; i >= 0; i--) if (a[i] != v.a[i])
      return a[i] * sign < v.a[i] * sign;
    return false;
  }
  bool operator <= (const BigInt &v) const { return !(v < *this); }
  bool operator > (const BigInt &v) const { return v < *this; }
  bool operator >= (const BigInt &v) const { return !(*this < v); }
  bool operator == (const BigInt &v) const {
    return !(*this < v) && !(v < *this);
  }
  bool operator != (const BigInt &v) const { return *this < v || v < *this; }
  BigInt() : sign(1) {}
  BigInt(const string &s) { read(s); }
  BigInt(T v) : BigInt(to_string(v)) {}
  bool isZero() const { return a.empty() || (int(a.size()) == 1 && !a[0]); }
  BigInt operator + () const { return *this; }
  BigInt operator - () const {
    BigInt res = *this; res.sign = -sign; return res;
  }
  BigInt abs() const { BigInt res = *this; res.sign *= res.sign; return res; }
  T value() const { 
    T res = 0;
    for (int i = int(a.size()) - 1; i >= 0; i--) res = res * BASE + a[i];
    return res * sign;
  }
  BigInt operator ++ () { return *this += BigInt(1); }
  BigInt operator ++ (int) {
    BigInt ret = *this; *this += BigInt(1); return ret;
  }
  BigInt operator -- () { return *this -= BigInt(1); }
  BigInt operator -- (int) {
    BigInt ret = *this; *this -= BigInt(1); return ret;
  }
  BigInt operator + (const BigInt &v) const {
    if (sign == v.sign) {
      BigInt res = v; T carry = 0;
      for (int i = 0; i < max(int(a.size()), int(v.a.size())) || carry; i++) {
        if (i == int(res.a.size())) res.a.push_back(0);
        res.a[i] += carry + (i < int(a.size()) ? a[i] : 0);
        carry = res.a[i] >= BASE; if (carry) res.a[i] -= BASE;
      }
      return res;
    }
    return *this - (-v);
  }
  BigInt &operator += (const BigInt &v) { return *this = *this + v; }
  BigInt operator - (const BigInt &v) const {
    if (sign == v.sign) {
      if (abs() >= v.abs()) {
        BigInt res = *this; T carry = 0;
        for (int i = 0; i < int(v.a.size()) || carry; i++) {
          res.a[i] -= carry + (i < int(v.a.size()) ? v.a[i] : 0);
          carry = res.a[i] < 0; if (carry) res.a[i] += BASE;
        }
        res.trim(); return res;
      }
      return -(v - *this);
    }
    return *this + (-v);
  }
  BigInt &operator -= (const BigInt &v) { return *this = *this - v; }
  BigInt operator * (const BigInt &v) const {
    BigInt res; res.a = mulIntFFT(a, v.a, BASE); res.sign = sign * v.sign;
    res.trim(); return res;
  }
  BigInt &operator *= (const BigInt &v) { return *this = *this * v; }
  friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
    T norm = BASE / (b1.a.back() + 1);
    BigInt a = a1.abs() * norm, b = b1.abs() * norm, q, r;
    q.a.resize(int(a.a.size()));
    for (int i = int(a.a.size()) - 1; i >= 0; i--) {
      r *= BASE; r += a.a[i];
      T s1 = int(r.a.size()) <= int(b.a.size()) ? 0 : r.a[int(b.a.size())];
      T s2 = int(r.a.size()) <= int(b.a.size()) - 1 ? 0
                                                    : r.a[int(b.a.size()) - 1];
      T d = (BASE * s1 + s2) / b.a.back(); r -= b * d;
      for (; r < 0; r += b) --d;
      q.a[i] = d;
    }
    q.sign = a1.sign * b1.sign; r.sign = a1.sign; q.trim(); r.trim();
    return make_pair(q, r / norm);
  }
  BigInt operator / (T v) const { BigInt res = *this; res /= v; return res; }
  BigInt &operator /= (T v) {
    if (v < 0) { sign = -sign; v = -v; }
    for (int i = int(a.size()) - 1, rem = 0; i >= 0; i--) {
      T cur = a[i] + rem * BASE; a[i] = cur / v; rem = cur % v;
    }
    trim(); return *this;
  }
  T operator % (T v) const {
    if (v < 0) v = -v;
    T m = 0;
    for (int i = int(a.size()) - 1; i >= 0; i--) m = (a[i] + m * BASE) % v;
    return m * sign;
  }
  BigInt operator / (const BigInt &v) const { return divmod(*this, v).first; }
  BigInt operator % (const BigInt &v) const { return divmod(*this, v).second; }
  BigInt &operator /= (const BigInt &v) { return *this = *this / v; }
  BigInt &operator %= (const BigInt &v) { return *this = *this % v; }
};
