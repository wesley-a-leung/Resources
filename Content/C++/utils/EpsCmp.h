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
template <class F> struct Functor {
  F f; Functor(F f) : f(f) {}
  template <class U>
  bool operator () (const U &a, const U &b) const { return f(a, b); }
};
template <class F> Functor<F> makeFunctor(F f) { return Functor<F>(f); }
