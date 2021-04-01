#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b, or line segments
//   in the form of f(x) = mx + b over l <= x <= r, and finding
//   the maximum value of f(x) where all possible value of x are
//   known beforehand
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x)
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   xs: a vector of type T of the possible x values for each query
//   inf: a value for positive infinity, must be negatable
// Functions:
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines
//   addLine(m, b, l, r): adds a line segment in the form f(x) = mx + b
//     where l <= x <= r, to the set of lines
//   getMax(x): finds the maximum value of f(x) (based on the comparator)
//     for all inserted lines, x must be in the vector xs
//   clear(): removes all lines from the seg
// In practice, has a moderate constant, performance compared to
//   DynamicConvexHullTrick (which uses multiset) and
//   DynamicConvexHullTrickSqrtBuffer can vary, faster than SparseLiChaoTree
// Time Complexity:
//   constructor: O(1)
//   addLine, getMax: O(log N) for the range [0, N)
//   addLineSegment: O((log N) ^ 2) for the range [0, N)
//   clear: O(N) for the range [0, N)
// Memory Complexity: O(N) for the range [0, N)
// Tested:
//   https://judge.yosupo.jp/problem/line_add_get_min
//   https://judge.yosupo.jp/problem/segment_add_get_min
//   https://open.kattis.com/problems/longestlife
//   https://www.spoj.com/problems/CHTPRAC/
template <class T, class Cmp = less<T>> struct LiChaoTree {
  using Line = pair<T, T>; int N; T INF; vector<Line> TR; vector<T> X;
  T eval(const Line &l, int i) const { return l.first * X[i] + l.second; }
  bool majorize(const Line &a, const Line &b, int l, int r) {
    return !Cmp()(eval(a, l), eval(b, l)) && !Cmp()(eval(a, r), eval(b, r));
  }
  int cInd(T x) const {
    return lower_bound(X.begin(), X.end(), x) - X.begin();
  }
  int fInd(T x) const {
    return upper_bound(X.begin(), X.end(), x) - X.begin() - 1;
  }
  LiChaoTree(const vector<T> &xs, T inf = numeric_limits<T>::max())
      : INF(min(inf, -inf, Cmp())), X(xs) {
    sort(X.begin(), X.end()); X.erase(unique(X.begin(), X.end()), X.end());
    N = X.size(); TR.assign(N == 0 ? 0 : 1 << __lg(N * 4 - 1), Line(T(), INF));
  }
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
  T getMax(int k, int tl, int tr, int i) const {
    T ret = eval(TR[k], i); if (tl == tr) return ret;
    int m = tl + (tr - tl) / 2;
    if (i <= m) return max(ret, getMax(k * 2, tl, m, i), Cmp());
    else return max(ret, getMax(k * 2 + 1, m + 1, tr, i), Cmp());
  }
  void addLine(T m, T b) { addLine(1, 0, N - 1, Line(m, b)); }
  void addLineSegment(T m, T b, T l, T r) {
    int li = cInd(l), ri = fInd(r);
    if (li <= ri) addLineSegment(1, 0, N - 1, li, ri, Line(m, b));
  }
  T getMax(T x) const { return getMax(1, 0, N - 1, cInd(x)); }
  void clear() { fill(TR.begin(), TR.end(), Line(T(), INF)); }
};
