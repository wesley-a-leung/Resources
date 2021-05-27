#pragma once
#include <bits/stdc++.h>
#include "Point3D.h"
#include "Line3D.h"
using namespace std;

// Functions for a 3D plane
struct Plane3D {
  pt3 n; T d;
  // ax + by + cz = d, above is ax + by + cz > d
  Plane3D(T a = 0, T b = 0, T c = 0, T d = 0) : n(a, b, c), d(d) {}
  // normal n, offset d
  Plane3D(pt3 n, T d) : n(n), d(d) {}
  // normal n, point p
  Plane3D(pt3 n, pt3 p) : n(n), d(n | p) {}
  // 3 non-collinear points p, q, r
  Plane3D(pt3 p, pt3 q, pt3 r) : Plane3D((q - p) * (r - p), p) {}
  T eval(pt3 p) const { return (n | p) - d; }
  // sign of isAbove, dist, dist: 1 if above plane, 0 if on plane,
  //   -1 if below plane
  int isAbove(pt3 p) const { return sgn(eval(p)); }
  T dist(pt3 p) const { return eval(p) / abs(n); }
  T distSq(pt3 p) const { T e = eval(p); return e * e / norm(n); }
  Plane3D translate(pt3 p) const { return Plane3D(n, d + (n | p)); }
  Plane3D shiftUp(T e) const { return Plane3D(n, d + e * abs(n)); }
  pt3 proj(pt3 p) const { return p - n * eval(p) / norm(n); }
  pt3 refl(pt3 p) const { return p - n * T(2) * eval(p) / norm(n); }
  // returns 1 if the projections of a, b, c on this plane form a ccw turn
  //   looking from above, 0 if collinear, -1 if cw
  int ccwProj(pt3 a, pt3 b, pt3 c) const { return sgn((b - a) * (c - a) | n); }
  // returns a tuple (a, b, c) of 3 non-collinear points on the plane,
  //   guaranteed that ccwProj(a, b, c) == 1
  tuple<pt3, pt3, pt3> getPts() const {
    pt3 v = pt3(1, 0, 0); if (eq(abs(n * v), 0)) v = pt3(0, 1, 0);
    pt3 v1 = n * v, v2 = n * v1; pt3 a = proj(pt3(0, 0, 0));
    return make_tuple(a, a + v1, a + v2);
  }
};
Line3D perpThrough(const Plane3D &pi, pt3 o) {
  Line3D ret; ret.o = o; ret.d = pi.n; return ret;
}
Plane3D perpThrough(const Line3D &l, pt3 o) { return Plane3D(l.d, o); }

// Transforms points to a new coordinate system where the x and y axes are
//   on the plane, with the z axis being the normal vector (positive z is in
//   the direction of the normal vector)
// Z coordinate is guaranteed to be the distance to the plane (positive if
//   above plane, negative if below, 0 if on)
// Constructor Arguments:
//   pi: a plane
//   p, q, r: 3 non-collinear points
// Functions:
//   transform(p): transforms p into the new coordinate system
// Time Complexity:
//   constructor, transform: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/utso15p6
struct CoordinateTransformation {
  pt3 o, dx, dy, dz;
  CoordinateTransformation(const Plane3D &pi) {
    pt3 p, q, r; tie(p, q, r) = pi.getPts(); o = p;
    dx = unit(q - p); dz = unit(dx * (r - p)); dy = dz * dx;
  }
  CoordinateTransformation(pt3 p, pt3 q, pt3 r) : o(p) {
    dx = unit(q - p); dz = unit(dx * (r - p)); dy = dz * dx;
  }
  pt3 transform(pt3 p) const {
    return pt3((p - o) | dx, (p - o) | dy, (p - o) | dz);
  }
};

// Intersection of plane and line
// Function Arguments:
//   pi: the plane
//   l: the line
//   res: a reference to the point to store the intersection if it exists
// Return Value: 0 if no intersection, 1 if point of intersection, 2 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
int planeLineIntersection(const Plane3D &pi, const Line3D &l, pt3 &res) {
  T a = pi.n | l.d; if (eq(norm(a), 0)) return pi.isAbove(l.o) == 0 ? 2 : 0;
  res = l.o - l.d * pi.eval(l.o) / a; return 1;
}

// Intersection of 2 planes
// Function Arguments:
//   pi1: the first plane
//   pi2: the second plane
//   res: a reference to the line to store the intersection if it exists
// Return Value: 0 if no intersection, 1 if line of intersection, 2 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/utso15p6
int planePlaneIntersection(const Plane3D &pi1, const Plane3D &pi2,
                           Line3D &res) {
  pt3 d = pi1.n * pi2.n; if (eq(norm(d), 0))
    return eq(abs(pi1.d / abs(pi1.n)), abs(pi2.d / abs(pi2.n))) ? 2 : 0;
  res.o = (pi2.n * pi1.d - pi1.n * pi2.d) * d / norm(d); res.d = d; return 1;
}
