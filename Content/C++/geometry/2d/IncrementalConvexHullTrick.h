#pragma once
#include <bits/stdc++.h>
using namespace std;

// Helper struct for IncrementalConvexHullTrick
template <class T> struct CHTLine {
  bool isQuery; T m, b; mutable T x;
  CHTLine(T m, T b) : isQuery(false), m(m), b(b), x(T()) {}
  CHTLine(T x) : isQuery(true), m(T()), b(T()), x(x) {}
};

// Supports adding lines in the form f(x) = mx + b and finding
//   the maximum value of f(x) at any given x; this version allows for
//   updates and queries in arbitrary order
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x)
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   cmp: an instance of the Cmp struct
//   INF: a value for positive infinity, must be negatable
// Functions (in addition to std::multiset):
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines
//   getMax(x): finds the maximum value of f(x) (based on the comparator)
//     for all inserted lines
// In practice, has a moderate constant, performance compared to
//   IncrementalConvexHullTrickSqrtBuffer and SparseLiChao can vary, slower
//   than LiChao
// Time Complexity:
//   constructor: O(1)
//   addLine, getMax: O(log(N)) amortized for N lines in the convex hull
// Memory Complexity: O(N) for N lines in the convex hull
// Tested:
//   https://judge.yosupo.jp/problem/line_add_get_min
//   https://open.kattis.com/problems/longestlife
//   https://www.spoj.com/problems/CHTPRAC/
#define FUN function<bool(const CHTLine<T> &, const CHTLine<T> &)>
template <class T, class Cmp = less<T>>
struct IncrementalConvexHullTrick : public multiset<CHTLine<T>, FUN> {
  using MS = multiset<CHTLine<T>, FUN>; using iter = typename MS::iterator;
  using MS::begin; using MS::end; using MS::emplace; using MS::erase;
  using MS::lower_bound; Cmp cmp; T INF;
  FUN makeFun(Cmp cmp) {
    return [=] (const CHTLine<T> &a, const CHTLine<T> &b) {
      return a.isQuery || b.isQuery ? a.x < b.x : cmp(a.m, b.m);
    };
  }
  IncrementalConvexHullTrick(Cmp cmp = Cmp(), T INF = numeric_limits<T>::max())
      : MS(makeFun(cmp)), cmp(cmp), INF(INF) {}
  template <const bool _ = is_floating_point<T>::value>
  typename enable_if<_, T>::type div(T a, T b) { return a / b; }
  template <const bool _ = is_floating_point<T>::value>
  typename enable_if<!_, T>::type div(T a, T b) {
    return a / b - T((a ^ b) < T() && cmp(T(), a % b));
  }
  bool intersect(iter x, iter y) {
    if (y == end()) { x->x = INF; return false; }
    if (!cmp(x->m, y->m) && !cmp(y->m, x->m))
      x->x = cmp(y->b, x->b) ? INF : -INF;
    else x->x = div(y->b - x->b, x->m - y->m);
    return x->x >= y->x;
  }
  void addLine(T m, T b) {
    auto z = emplace(m, b), y = z++, x = y;
    while (intersect(y, z)) z = erase(z);
    if (x != begin() && intersect(--x, y)) intersect(x, y = erase(y));
    while ((y = x) != begin() && (--x)->x >= y->x) intersect(x, erase(y));
  }
  T getMax(T x) const {
    auto l = *lower_bound(CHTLine<T>(x)); return l.m * x + l.b;
  }
};
#undef FUN
