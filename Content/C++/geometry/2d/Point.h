#pragma once
#include <bits/stdc++.h>
#include "../../utils/EpsCmp.h"
using namespace std;

// Functions for a 2D point
// * operator between 2 points is complex number multiplication
// / operator between 2 points is complex number division
#define ref const pt &
#define OP(op, U, a, x, y) pt operator op (U a) const { return pt(x, y); } \
  pt &operator op##= (U a) { return *this = *this op a; }
#define CMP(op, body) bool operator op (ref p) const { return body; }
struct pt {
  T x, y; constexpr pt(T x = 0, T y = 0) : x(x), y(y) {}
  pt operator + () const { return *this; }
  pt operator - () const { return pt(-x, -y); }
  OP(+, ref, p, x + p.x, y + p.y) OP(-, ref, p, x - p.x, y - p.y)
  OP(*, T, a, x * a, y * a) OP(/, T, a, x / a, y / a)
  friend pt operator * (T a, ref p) { return pt(a * p.x, a * p.y); }
  bool operator < (ref p) const {
    return eq(x, p.x) ? lt(y, p.y) : lt(x, p.x);
  }
  CMP(<=, !(p < *this)) CMP(>, p < *this) CMP(>=, !(*this < p))
  CMP(==, !(*this < p) && !(p < *this)) CMP(!=, *this < p || p < *this)
  OP(*, ref, p, x * p.x - y * p.y, y * p.x + x * p.y)
  OP(/, ref, p, (x * p.x + y * p.y) / (p.x * p.x + p.y * p.y),
                (y * p.x - x * p.y) / (p.x * p.x + p.y * p.y))
};
#undef OP
#undef CMP
istream &operator >> (istream &stream, pt &p) {
  return stream >> p.x >> p.y;
}
ostream &operator << (ostream &stream, ref p) {
  return stream << p.x << ' ' << p.y;
}
pt conj(ref a) { return pt(a.x, -a.y); }
T dot(ref a, ref b) { return a.x * b.x + a.y * b.y; }
T cross(ref a, ref b) { return a.x * b.y - a.y * b.x; }
T norm(ref a) { return dot(a, a); }
T abs(ref a) { return sqrt(norm(a)); }
T arg(ref a) { return atan2(a.y, a.x); }
pt polar(T r, T theta) { return r * pt(cos(theta), sin(theta)); }
T distSq(ref a, ref b) { return norm(b - a); }
T dist(ref a, ref b) { return abs(b - a); }
T ang(ref a, ref b) { return arg(b - a); }
// sign of ang, area2, ccw: 1 if counterclockwise, 0 if collinear,
//   -1 if clockwise
T ang(ref a, ref b, ref c) {
  return remainder(ang(b, a) - ang(b, c), 2 * acos(T(-1)));
}
// twice the signed area of triangle a, b, c
T area2(ref a, ref b, ref c) { return cross(b - a, c - a); }
int ccw(ref a, ref b, ref c) { return sgn(area2(a, b, c)); }
// a rotated theta radians around p
pt rot(ref a, ref p, T theta) { return (a - p) * pt(polar(T(1), theta)) + p; }
// rotated 90 degrees ccw
pt perp(ref a) { return pt(-a.y, a.x); }
