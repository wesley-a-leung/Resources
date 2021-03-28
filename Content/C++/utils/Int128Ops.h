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
