#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Struct to compare angles around a pivot based on arg(p - pivot)
// Points directly to the left have an angle of -PI, points equal to the pivot
//   are placed after all points
// Constructor Arguments:
//   p: the point
// Fields:
//   static pivot: the pivot point
// Functions:
//   static setPivot(p): sets p as the pivot point
//   <, <=, >, >=, ==, !=: compares 2 angles
// Time Complexity:
//   constructor, setPivot, <, <=, >, >=, ==, !=: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/sort_points_by_argument
struct Angle {
  static pt pivot; static void setPivot(ref p) { pivot = p; }
  pt p; Angle(ref p = pt(0, 0)) : p(p) {}
  int half() const {
    if (eq(p.x, 0) && eq(p.y, 0)) return 2;
    return int(!lt(p.y, 0) && (!eq(p.y, 0) || !lt(p.x, 0)));
  }
  bool operator < (const Angle &a) const {
    int h = half() - a.half(); return h == 0 ? ccw(pivot, p, a.p) > 0 : h < 0;
  }
  bool operator <= (const Angle &a) const { return !(a < *this); }
  bool operator > (const Angle &a) const { return a < *this; }
  bool operator >= (const Angle &a) const { return !(*this < a); }
  bool operator == (const Angle &a) const {
    return !(*this < a) && !(a < *this);
  }
  bool operator != (const Angle &a) const { return *this < a || a < *this; }
};

pt Angle::pivot = pt(0, 0);
