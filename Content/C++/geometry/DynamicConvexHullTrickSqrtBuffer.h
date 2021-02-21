#pragma once
#include <bits/stdc++.h>
#include "../search/BinarySearch.h"
using namespace std;

// Supports adding lines in the form f(x) = mx + b and finding
//   the maximum value of f(x) at any given x; this version allows for
//   updates and queries in arbitrary order
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x), must be able to store m * b
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   SCALE: the value to scale sqrt by
// Functions:
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines
//   getMax(x): finds the maximum value of f(x) (based on the comparator)
//     for all inserted lines
//   size(): returns the number of lines in the convex hull
//   reserve(N): reserves space for N lines in the convex hull
// In practice, has a very small constant, performance compared to
//   DynamicConvexHullTrick (which uses multiset) and DynamicLiChao can vary
// Time Complexity:
//   constructor: O(1)
//   addLine: O(1) amortized
//   getMax: O(sqrt(N) + log(N)) amortized for N lines in the convex hull
//   size: O(1)
//   reserve: O(N)
// Memory Complexity: O(N) for N lines in the convex hull
// Tested:
//   https://judge.yosupo.jp/problem/line_add_get_min
//   https://naq18.kattis.com/problems/longestlife
//   https://www.spoj.com/problems/CHTPRAC/
//   https://csacademy.com/contest/round-70/task/squared-ends/
//   https://facebook.com/codingcompetitions/hacker-cup/2020/round-2/problems/D
template <class T, class Cmp = less<T>>
struct DynamicConvexHullTrickSqrtBuffer {
  struct Line {
    T m, b; Line(T m, T b) : m(m), b(b) {}
    T eval(T x) const { return m * x + b; }
    bool operator < (const Line &l) const { return Cmp()(m, l.m); }
  };
  vector<Line> large, small; double SCALE;
  DynamicConvexHullTrickSqrtBuffer(double SCALE = 4) : SCALE(SCALE) {}
  bool ccw(const Line &a, const Line &b, const Line &c) {
    return (b.m - a.m) * (c.b - a.b) <= (b.b - a.b) * (c.m - a.m);
  }
  bool slope(const Line &a, const Line &b) {
    return !Cmp()(a.m, b.m) && !Cmp()(b.m, a.m) && !Cmp()(a.b, b.b);
  }
  void rebuildHull() {
    int back = 0; for (auto &&line : large) {
      while (back >= 1 && slope(line, large[back - 1])) back--;
      while (back >= 2 && ccw(line, large[back - 1], large[back - 2])) back--;
      large[back++] = line;
    }
    large.resize(back, Line(T(), T()));
  }
  int size() const { return large.size() + small.size(); }
  void rebuild() {
    if (int(small.size()) > SCALE * sqrt(size())) {
      int lSz = large.size(); sort(small.begin(), small.end());
      large.insert(large.end(), small.begin(), small.end()); small.clear();
      inplace_merge(large.begin(), large.begin() + lSz, large.end());
      rebuildHull();
    }
  }
  void addLine(T m, T b) { small.emplace_back(m, b); }
  T getMax(T x) {
    rebuild(); int ind = bsearch<FIRST>(0, int(large.size()) - 1, [&] (int i) {
      return Cmp()(large[i + 1].eval(x), large[i].eval(x));
    });
    T mx = (large.empty() ? small[0] : large[ind]).eval(x);
    for (auto &&line : small) mx = max(mx, line.eval(x), Cmp());
    return mx;
  }
};
