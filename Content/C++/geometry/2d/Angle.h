#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Struct to compare angles in the range [-PI, PI) around a pivot based
//   on arg(p - pivot)
// Points directly to the left have an angle of -PI, points equal to the pivot
//   are placed after all points
// Constructor Arguments:
//   p: the point
// Fields:
//   static pivot: the pivot point
// Functions:
//   static setPivot(p): sets p as the pivot point
//   <, <=, >, >=, ==, !=: compares 2 angles
//   +, +=, -, -=: arithmetic operators
// Time Complexity:
//   constructor, setPivot, <, <=, >, >=, ==, !=, +, +=, -, -=: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/sort_points_by_argument
#define OP(op, body) Angle operator op (Angle a) const { return body; } \
  Angle &operator op##= (Angle a) { return *this = *this op a; }
#define CMP(op, body) bool operator op (Angle a) const { return body; }
struct Angle {
  static pt pivot; static void setPivot(pt p) { pivot = p; }
  pt p; Angle(pt p = pt(0, 0)) : p(p) {}
  int half() const {
    if (eq(p.x, pivot.x) && eq(p.y, pivot.y)) return 2;
    return int(!lt(p.y, pivot.y) && (!eq(p.y, pivot.y) || !lt(p.x, pivot.x)));
  }
  bool operator < (Angle a) const {
    int h = half() - a.half(); return h == 0 ? ccw(pivot, p, a.p) > 0 : h < 0;
  }
  CMP(<=, !(a < *this)) CMP(>, a < *this) CMP(>=, !(*this < a))
  CMP(==, !(*this < a) && !(a < *this)) CMP(!=, *this < a || a < *this)
  Angle operator + () const { return *this; }
  Angle operator - () const { return Angle(conj(p)); }
  OP(+, Angle(pivot + (p - pivot) * (a.p - pivot))) OP(-, *this + (-a))
};
#undef OP
#undef CMP

pt Angle::pivot = pt(0, 0);
