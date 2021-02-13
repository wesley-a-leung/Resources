#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
using namespace std;

// Functions for a 3D point
#define ref3 const pt3 &
struct pt3 {
  T x, y, z; pt3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
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
  bool operator != (ref3 p) const { return !(*this == p); }
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
T ang(ref3 a, ref3 b) {
  return acos(max(T(-1), min(T(1), (a | b) / abs(a) / abs(b))));
}
pt3 rot(ref3 a, ref3 axis, T theta) {
  return a * cos(theta) + (unit(axis) * a * sin(theta))
      + (unit(axis) * (unit(axis) | a) * (1 - cos(theta)));
}
