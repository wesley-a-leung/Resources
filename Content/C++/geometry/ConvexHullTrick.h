#pragma once
#include <bits/stdc++.h>
#include "../search/BinarySearch.h"
using namespace std;

// Supports adding lines in the form f(x) = mx + b and finding
//   the maximum value of f(x) at any given x; this version only supports
//   adding lines in sorted order
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x), must be able to store m * b
//   Cmp: the comparator to compare two f(x) values,
//     convention is same as priority_queue in STL
//   REVERSE: boolean to indicate whether the lines are added in reverse order
//     of the comparator
// Functions:
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines,
//     lines must be added in the order of slope sorted by Cmp, or in reverse
//     order if REVERSE is true
//   getMax(x): finds the maximum x value (based on the comparator) for all
//     inserted lines
//   getMaxMonoInc(x): finds the maximum x value (based on the comparator)
//     for all inserted lines, where all queries have non decreasing x values
//   getMaxMonoDec(x): finds the maximum x value (based on the comparator)
//     for all inserted lines, where all queries have non increasing x values
//   size(): returns the number of lines in the convex hull
//   reserve(N): reserves space for N lines in the convex hull
// In practice, has a moderate constant
// Time Complexity:
//   addLine, getMaxMonoInc, getMaxMonoDec: O(1) amortized
//   getMax: O(log N) for N lines in the convex hull
//   size: O(1)
//   reserve: O(N)
// Memory Complexity: O(N) for N lines in the convex hull
// Tested:
//   https://www.spoj.com/problems/CHTPRAC/
//   https://atcoder.jp/contests/dp/tasks/dp_z
template <class T, class Cmp = less<T>, const bool REVERSE = false>
struct ConvexHullTrick {
  struct Line {
    T m, b; Line(T m, T b) : m(m), b(b) {}
    T eval(T x) const { return m * x + b; }
  };
  vector<Line> L; int front, back; ConvexHullTrick() : front(0), back(0) {}
  int size() const { return L.size(); }
  void addLine(T m, T b) {
    auto ccw = [&] {
      T c1 = (L[size() - 1].m - m) * (L[size() - 2].b - b);
      T c2 = (L[size() - 1].b - b) * (L[size() - 2].m - m);
      return REVERSE ? c1 >= c2 : c1 <= c2;
    };
    while (size() >= 2 && ccw()) L.pop_back();
    while (!L.empty() && !Cmp()(m, L[size() - 1].m)
           && !Cmp()(L[size() - 1].m, m) && !Cmp()(b, L[size() - 1].b))
      L.pop_back();
    if (size() == back) back++;
    L.emplace_back(m, b);
    front = min(front, size() - 1); back = min(back, size());
  }
  T moveFront(T x) {
    while (front + 1 < size()
           && !Cmp()(L[front + 1].eval(x), L[front].eval(x)))
      front++;
    return L[front].eval(x);
  }
  T moveBack(T x) {
    while (back - 2 >= 0 && !Cmp()(L[back - 2].eval(x), L[back - 1].eval(x)))
      back--;
    return L[back - 1].eval(x);
  }
  T getMaxMonoInc(T x) { return REVERSE ? moveBack(x) : moveFront(x); }
  T getMaxMonoDec(T x) { return REVERSE ? moveFront(x) : moveBack(x); }
  T getMax(T x) const {
    return L[bsearch<FIRST>(0, size() - 1, [&] (int i) {
      return Cmp()(L[i + 1].eval(x), L[i].eval(x));
    })].eval(x);
  }
  void reserve(int N) { L.reserve(N); }
};
