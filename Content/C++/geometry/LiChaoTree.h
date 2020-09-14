#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b, or line segments
//   in the form of f(x) = mx + b over l <= x <= r, and finding
//   the maximum value of f(x) at an integral point x where 0 <= x < N
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x)
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   N: the maximum bound (exclusive) for the value of x, allowing queries
//     where 0 <= x < N
//   INF: a value for positive infinity, must be negatable
// Functions:
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines
//   addLine(m, b, l, r): adds a line segment in the form f(x) = mx + b
//     where l <= x <= r, to the set of lines
//   getMax(x): finds the maximum value of f(x) (based on the comparator)
//     for all inserted lines
//   clear(): removes all lines from the seg
// In practice, has a moderate constant, performance compared to
//   DynamicConvexHullTrick (which uses multiset) and
//   DynamicConvexHullTrickSqrtBuffer can vary, faster than DynamicLiChaoTree
// Time Complexity:
//   constructor: O(1)
//   addLine, getMax: O(log N) for the range [0, N)
//   addLineSegment: O((log N) ^ 2) for the range [0, N)
//   clear: O(N) for the range [0, N)
// Memory Complexity: O(N) for the range [0, N)
// Tested:
//   https://csacademy.com/contest/round-70/task/squared-ends/
template <class T, class Cmp = less<T>> struct LiChaoTree {
  int N; T INF; using Line = pair<T, T>; vector<Line> TR;
  T eval(const Line &l, int x) const {
    return l.second == INF ? INF : l.first * x + l.second;
  }
  bool majorize(const Line &a, const Line &b, int l, int r) {
    return !Cmp()(eval(a, l), eval(b, l)) && !Cmp()(eval(a, r), eval(b, r));
  }
  LiChaoTree(int N, T inf = numeric_limits<T>::max())
      : N(N), INF(min(inf, -inf, Cmp())), TR(N * 4, Line(T(), INF)) {}
  void addLine(int k, int tl, int tr, Line line) {
    if (majorize(line, TR[k], tl, tr)) swap(line, TR[k]);
    if (majorize(TR[k], line, tl, tr)) return;
    if (Cmp()(eval(TR[k], tl), eval(line, tl))) swap(line, TR[k]);
    int m = tl + (tr - tl) / 2; if (!Cmp()(eval(line, m), eval(TR[k], m))) {
      swap(line, TR[k]); addLine(k * 2, tl, m, line);
    } else addLine(k * 2 + 1, m + 1, tr, line);
  }
  void addLineSegment(int k, int tl, int tr, int l, int r, Line line) {
    if (r < tl || tr < l) return;
    if (l <= tl && tr <= r) { addLine(k, tl, tr, line); return; }
    int m = tl + (tr - tl) / 2; addLineSegment(k * 2, tl, m, l, r, line);
    addLineSegment(k * 2 + 1, m + 1, tr, l, r, line);
  }
  T getMax(int k, int tl, int tr, int x) const {
    T ret = eval(TR[k], x); if (ret == INF || tl == tr) return ret;
    int m = tl + (tr - tl) / 2;
    if (x <= m) return max(ret, getMax(k * 2, tl, m, x), Cmp());
    else return max(ret, getMax(k * 2 + 1, m + 1, tr, x), Cmp());
  }
  void addLine(T m, T b) { addLine(1, 0, N - 1, Line(m, b)); }
  void addLineSegment(T m, T b, int l, int r) {
    addLineSegment(1, 0, N - 1, l, r, Line(m, b));
  }
  T getMax(int x) const { return getMax(1, 0, N - 1, x); }
  void clear() { fill(TR.begin(), TR.end(), Line(T(), INF)); }
};
