#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point3D.h"
using namespace std;

// Functions for a 3D plane
struct Plane3D {
  pt3 n; T d;
  // ax + by + cz = d
  Plane3D(T a = 0, T b = 0, T c = 0, T d = 0) : n(a, b, c), d(d) {}
  // normal n, offset d
  Plane3D(ref3 n, T d) : n(n), d(d) {}
  // normal n, point p
  Plane3D(ref3 n, ref3 p) : n(n), d(n | p) {}
  // 3 non-collinear points p, q, r
  Plane3D(ref3 p, ref3 q, ref3 r) : Plane3D((q - p) * (r - p), p) {}
  T eval(ref3 p) const { return (n | p) - d; }
  // 1 if above plane, 0 if on plane, -1 if below plane
  int isAbove(ref3 p) const { return sgn(eval(p)); }
  T distSq(ref3 p) const { T e = eval(p); return e * e / norm(n); }
  T dist(ref3 p) const { return abs(eval(p) / abs(n)); }
  Plane3D translate(ref3 p) const { return Plane3D(n, d + (n | p)); }
  Plane3D shiftUp(T e) const { return Plane3D(n, d + e * abs(n)); }
  pt3 proj(ref3 p) const { return p - n * eval(p) / norm(n); }
  pt3 refl(ref3 p) const { return p - n * T(2) * eval(p) / norm(n); }
  // returns 1 if the projections of a, b, c on this plane form a ccw turn,
  //   0 if collinear, -1 if cw
  int ccwProj(ref3 a, ref3 b, ref3 c) const {
    return sgn((b - a) * (c - a) | n);
  }
  // returns 3 non-collinear points on the plane
  tuple<pt3, pt3, pt3> getPts() const {
    pt3 v = pt3(1, 0, 0); if (eq(abs(n * v), 0)) v = pt3(0, 1, 0);
    pt3 v1 = n * v, v2 = n * v1; pt3 a = proj(pt3(0, 0, 0));
    return make_tuple(a, a + v1, a + v2);
  }
};
