#pragma once
#include <bits/stdc++.h>
#include "../../utils/EpsCmp.h"
using namespace std;

// Functions for a 3D point
// * operator between 2 points is cross product
// | operator between 2 points is dot product
#define OP(op, U, a, x, y, z) \
  pt3 operator op (U a) const { return pt3(x, y, z); } \
  pt3 &operator op##= (U a) { return *this = *this op a; }
#define CMP(op, body) bool operator op (pt3 p) const { return body; }
struct pt3 {
  T x, y, z; constexpr pt3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
  pt3 operator + () const { return *this; }
  pt3 operator - () const { return pt3(-x, -y, -z); }
  OP(+, pt3, p, x + p.x, y + p.y, z + p.z)
  OP(-, pt3, p, x - p.x, y - p.y, z - p.z)
  OP(*, T, a, x * a, y * a, z * a) OP(/, T, a, x / a, y / a, z / a)
  friend pt3 operator * (T a, pt3 p) { return pt3(a * p.x, a * p.y, a * p.z); }
  bool operator < (pt3 p) const {
    return eq(x, p.x) ? (eq(y, p.y) ? lt(z, p.z) : lt(y, p.y)) : lt(x, p.x);
  }
  CMP(<=, !(p < *this)) CMP(>, p < *this) CMP(>=, !(*this < p))
  CMP(==, !(*this < p) && !(p < *this)) CMP(!=, *this < p || p < *this)
  T operator | (pt3 p) const { return x * p.x + y * p.y + z * p.z; }
  OP(*, pt3, p, y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x)
};
#undef OP
#undef CMP
istream &operator >> (istream &stream, pt3 &p) {
  return stream >> p.x >> p.y >> p.z;
}
ostream &operator << (ostream &stream, pt3 p) {
  return stream << p.x << ' ' << p.y << ' ' << p.z;
}
T norm(pt3 p) { return p | p; }
T abs(pt3 p) { return sqrt(norm(p)); }
pt3 unit(pt3 p) { return p / abs(p); }
T distSq(pt3 a, pt3 b) { return norm(b - a); }
T dist(pt3 a, pt3 b) { return abs(b - a); }
// returns an angle in the range [0, PI]
T ang(pt3 a, pt3 b, pt3 c) {
  a = unit(a - b); c = unit(c - b); return 2 * atan2(abs(a - c), abs(a + c));
}
pt3 rot(pt3 a, pt3 axis, T theta) {
  return a * cos(theta) + (unit(axis) * a * sin(theta))
      + (unit(axis) * (unit(axis) | a) * (1 - cos(theta)));
}
// sign of volume6 and above: 1 if d is above the plane abc with
//   normal ab x ac, 0 if on the plane, -1 if below the plane
// 6 times the signed area of the tetrahedron abcd
T volume6(pt3 a, pt3 b, pt3 c, pt3 d) {
  return (b - a) * (c - a) | (d - a);
}
int above(pt3 a, pt3 b, pt3 c, pt3 d) { return sgn(volume6(a, b, c, d)); }
// Converts a position based on radius (r >= 0), inclination/latitude
//   (-PI / 2 <= theta <= PI / 2), and azimuth/longitude (-PI < phi <= PI)
// Convention is that the x axis passes through the meridian (phi = 0), and
//   the z axis passes through the North Pole (theta = Pi / 2)
pt3 sph(T r, T theta, T phi) {
  return pt3(r * cos(theta) * cos(phi), r * cos(theta) * sin(phi),
             r * sin(theta));
}
T inc(pt3 p) { return atan2(p.z, T(sqrt(p.x * p.x + p.y * p.y))); }
T az(pt3 p) { return atan2(p.y, p.x); }
