#pragma once
#include <bits/stdc++.h>
using namespace std;

// Functions for __int128_t
__int128_t abs(__int128_t a) { return a >= 0 ? a : -a; }
#define FUN(name) \
  long double name(__int128_t a) { return name((long double)(a)); }
FUN(sqrt) FUN(sin) FUN(cos) FUN(tan) FUN(asin) FUN(acos) FUN(atan)
#undef FUN
long double atan2(__int128_t y, __int128_t x) {
  return atan2((long double)(y), (long double)(x));
}

// 128-bit integers IO operations
__uint128_t stoui128(const string &s) {
  __uint128_t x = 0;
  for (int i = 0; i < int(s.length()); i++) x = x * 10 + (s[i] - '0');
  return x;
}

__int128_t stoi128(const string &s) {
  __int128_t x = 0; int i = 0; bool neg = false;
  if (s[0] == '-') { neg = true; i++; }
  for (; i < int(s.length()); i++) x = x * 10 + (s[i] - '0');
  if (neg) x *= -1;
  return x;
}

istream &operator >> (istream &stream, __uint128_t &x) {
  string s; stream >> s; x = stoui128(s); return stream;
}

istream &operator >> (istream &stream, __int128_t &x) {
  string s; stream >> s; x = stoi128(s); return stream;
}

string to_string(__uint128_t x) {
  if (x == 0) return string("0");
  string s; for (; x > 0; x /= 10) s.push_back('0' + x % 10);
  reverse(s.begin(), s.end()); return s;
}

string to_string(__int128_t x) {
  string s = to_string(__uint128_t(x >= 0 ? x : -x));
  if (x < 0) s.insert(s.begin(), '-');
  return s;
}

ostream &operator << (ostream &stream, __uint128_t x) {
  return stream << to_string(x);
}

ostream &operator << (ostream &stream, __int128_t x) {
  return stream << to_string(x);
}
