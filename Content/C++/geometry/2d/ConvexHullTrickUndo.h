#pragma once
#include <bits/stdc++.h>
#include "../../search/BinarySearch.h"
using namespace std;

// Supports adding lines in the form f(x) = mx + b, finding
//   the maximum value of f(x) at any given x, and removing the last line added
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x), must be able to store m * b
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Functions:
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines,
//     lines must be added in the order of slope sorted by Cmp
//   undo(): removed the last line added
//   getMax(x): finds the maximum x value (based on the comparator) for all
//     inserted lines
//   size(): returns the number of lines in the convex hull
//   reserve(N): reserves space for N lines in the convex hull
// In practice, has a moderate constant
// Time Complexity:
//   addLine: O(log N) if reserve is called beforehand,
//            O(log N) amortized otherwise
//   getMax: O(log N)
//   undo, size: O(1)
//   reserve: O(N)
// Memory Complexity: O(N) for N lines in the convex hull
// Tested:
//   https://oj.uz/problem/view/CEOI09_harbingers
template <class T, class Cmp = less<T>> struct ConvexHullTrickUndo {
  struct Line {
    T m, b; Line(T m = T(), T b = T()) : m(m), b(b) {}
    T eval(T x) const { return m * x + b; }
  };
  vector<pair<int, Line>> history; vector<Line> L; int back;
  ConvexHullTrickUndo() : back(0) {}
  int size() const { return back; }
  void addLine(T m, T b) {
    int i = back; if (i >= 1)
      i = bsearch<LAST>(1, i + 1, [&] (int j) {
        return Cmp()(m, L[j - 1].m) || Cmp()(L[j - 1].m, m)
            || Cmp()(b, L[j - 1].b);
      });
    if (i >= 2)
      i = bsearch<LAST>(2, i + 1, [&] (int j) {
        T c1 = (L[j - 1].m - m) * (L[j - 2].b - b);
        T c2 = (L[j - 1].b - b) * (L[j - 2].m - m);
        return c1 > c2;
      });
    if (i == int(L.size())) L.emplace_back();
    history.emplace_back(back, L[i]); L[i] = Line(m, b); back = i + 1;
  }
  void undo() { tie(back, L[back - 1]) = history.back(); history.pop_back(); }
  T getMax(T x) const {
    return L[bsearch<FIRST>(0, back - 1, [&] (int i) {
      return Cmp()(L[i + 1].eval(x), L[i].eval(x));
    })].eval(x);
  }
  void reserve(int N) { L.reserve(N); history.reserve(N); }
};
