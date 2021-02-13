#pragma once
#include <bits/stdc++.h>
using namespace std;

// Functions for epsilon comparison of floating points
using T = long double;
constexpr const T EPS = 1e-9;
bool lt(T a, T b) { return a + EPS < b; }
bool le(T a, T b) { return !lt(b, a); }
bool gt(T a, T b) { return lt(b, a); }
bool ge(T a, T b) { return !lt(a, b); }
bool eq(T a, T b) { return !lt(a, b) && !lt(b, a); }
bool ne(T a, T b) { return lt(a, b) || lt(b, a); }
int sgn(T a) { return lt(a, 0) ? -1 : lt(0, a) ? 1 : 0; }
struct eps_lt { bool operator () (T a, T b) const { return lt(a, b); } };
struct eps_le { bool operator () (T a, T b) const { return !lt(b, a); } };
struct eps_gt { bool operator () (T a, T b) const { return lt(b, a); } };
struct eps_ge { bool operator () (T a, T b) const { return !lt(a, b); } };
struct eps_eq {
  bool operator () (T a, T b) const { return !lt(a, b) && !lt(b, a); }
};
struct eps_ne {
  bool operator () (T a, T b) const { return lt(a, b) || lt(b, a); }
};
