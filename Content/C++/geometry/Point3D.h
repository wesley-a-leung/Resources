#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
using namespace std;

// Functions for a 3D point
#define ref3 const pt3 &
struct pt3 {
  T x, y, z; pt3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
  pt3 operator + () const { return *this; }
  pt3 operator - () const { return pt3(-x, -y, -z); }
  pt3 operator + (ref3 p) const { return pt3(x + p.x, y + p.y, z + p.z); }
  pt3 &operator += (ref3 p) { return *this = *this + p; }
  pt3 operator - (ref3 p) const { return pt3(x - p.x, y - p.y, z - p.z); }
  pt3 &operator -= (ref3 p) { return *this = *this + p; }
  pt3 operator * (T a) const { return pt3(x * a, y * a, z * a); }
  pt3 &operator *= (T a) { return *this = *this * a; }
  friend pt3 operator * (T a, ref3 p) {
    return pt3(a * p.x, a * p.y, a * p.z);
  }
  pt3 operator / (T a) const { return pt3(x / a, y / a, z / a); }
  pt3 &operator /= (T a) { return *this = *this / a; }
  bool operator < (ref3 p) const {
    return eq(x, p.x) ? (eq(y, p.y) ? lt(z, p.z) : lt(y, p.y)) : lt(x, p.x);
  }
  bool operator <= (ref3 p) const { return !(p < *this); }
  bool operator > (ref3 p) const { return p < *this; }
  bool operator >= (ref3 p) const { return !(*this < p); }
  bool operator == (ref3 p) const { return !(*this < p) && !(p < *this); }
  bool operator != (ref3 p) const { return *this < p || p < *this; }
  T operator | (ref3 p) const { return x * p.x + y * p.y + z * p.z; }
  pt3 operator * (ref3 p) const {
    return pt3(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
  }
};
istream &operator >> (istream &stream, pt3 &p) {
  return stream >> p.x >> p.y >> p.z;
}
ostream &operator << (ostream &stream, ref3 p) {
  return stream << p.x << ' ' << p.y << ' ' << p.z;
}
T norm(ref3 p) { return p | p; }
T abs(ref3 p) { return sqrt(norm(p)); }
pt3 unit(ref3 p) { return p / abs(p); }
T distSq(ref3 a, ref3 b) { return norm(b - a); }
T dist(ref3 a, ref3 b) { return abs(b - a); }
// returns an angle in the range [0, PI]
T ang(ref3 a, ref3 b) {
  return acos(max(T(-1), min(T(1), (a | b) / abs(a) / abs(b))));
}
pt3 rot(ref3 a, ref3 axis, T theta) {
  return a * cos(theta) + (unit(axis) * a * sin(theta))
      + (unit(axis) * (unit(axis) | a) * (1 - cos(theta)));
}
// sign of volume6 and above: 1 if d is above the plane abc with
//   normal ab x ac, 0 if on the plane, -1 if below the plane
// 6 times the signed area of the tetrahedron abcd
T volume6(ref3 a, ref3 b, ref3 c, ref3 d) {
  return (b - a) * (c - a) | (d - a);
}
int above(ref3 a, ref3 b, ref3 c, ref3 d) { return sgn(volume6(a, b, c, d)); }
// Converts a position based on radius (r >= 0), inclination/latitude
//   (-PI / 2 <= theta <= PI / 2), and azimuth/longitude (-PI < phi <= PI)
// Convention is that the x axis passes through the meridian (phi = 0), and
//   the z axis passes through the North Pole (theta = Pi / 2)
pt3 sph(T r, T theta, T phi) {
  return pt3(r * cos(theta) * cos(phi), r * cos(theta) * sin(phi),
             r * sin(theta));
}
T inc(ref3 p) { return atan(sqrt(p.x * p.x + p.y * p.y) / p.z); }
T az(ref3 p) { return atan2(p.y, p.x); }
