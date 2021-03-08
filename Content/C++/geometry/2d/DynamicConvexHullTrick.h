#pragma once
#include <bits/stdc++.h>
using namespace std;

// Helper struct for DynamicConvexHullTrick
template <class T, class Cmp> struct Line {
  bool isQuery; T m, b; mutable T x;
  Line(T m, T b) : isQuery(false), m(m), b(b), x(T()) {}
  Line(T x) : isQuery(true), m(T()), b(T()), x(x) {}
  bool operator < (const Line &l) const {
    return isQuery || l.isQuery ? x < l.x : Cmp()(m, l.m);
  }
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
//   INTEGRAL: a boolean indicating whether T is integral or not (necessary
//     for types such as __int128_t)
// Constructor Arguments:
//   INF: a value for positive infinity, must be negatable
// Functions (in addition to std::multiset):
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines
//   getMax(x): finds the maximum value of f(x) (based on the comparator)
//     for all inserted lines
// In practice, has a moderate constant, performance compared to
//   DynamicConvexHullTrickSqrtBuffer and DynamicLiChao can vary
// Time Complexity:
//   constructor: O(1)
//   addLine, getMax: O(log(N)) amortized for N lines in the convex hull
// Memory Complexity: O(N) for N lines in the convex hull
// Tested:
//   https://judge.yosupo.jp/problem/line_add_get_min
//   https://naq18.kattis.com/problems/longestlife
//   https://www.spoj.com/problems/CHTPRAC/
//   https://csacademy.com/contest/round-70/task/squared-ends/
//   https://facebook.com/codingcompetitions/hacker-cup/2020/round-2/problems/D
template <class T, class Cmp = less<T>,
          const bool INTEGRAL = is_integral<T>::value>
struct DynamicConvexHullTrick : public multiset<Line<T, Cmp>> {
  using L = Line<T, Cmp>; using multiset<L>::begin; using multiset<L>::end;
  using multiset<L>::emplace; using multiset<L>::erase;
  using multiset<L>::lower_bound; using iter = typename multiset<L>::iterator;
  T INF; DynamicConvexHullTrick(T INF = numeric_limits<T>::max()) : INF(INF) {}
  template <const bool _ = INTEGRAL>
  static typename enable_if<_, T>::type div(T a, T b) {
    return a / b - T((a ^ b) < T() && Cmp()(T(), a % b));
  }
  template <const bool _ = INTEGRAL>
  static typename enable_if<!_, T>::type div(T a, T b) { return a / b; }
  bool intersect(iter x, iter y) {
    if (y == end()) { x->x = INF; return false; }
    if (!Cmp()(x->m, y->m) && !Cmp()(y->m, x->m))
      x->x = Cmp()(y->b, x->b) ? INF : -INF;
    else x->x = div(y->b - x->b, x->m - y->m);
    return x->x >= y->x;
  }
  void addLine(T m, T b) {
    auto z = emplace(m, b), y = z++, x = y;
    while (intersect(y, z)) z = erase(z);
    if (x != begin() && intersect(--x, y)) intersect(x, y = erase(y));
    while ((y = x) != begin() && (--x)->x >= y->x) intersect(x, erase(y));
  }
  T getMax(T x) const { auto l = *lower_bound(L(x)); return l.m * x + l.b; }
};
