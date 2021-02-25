#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point3D.h"
using namespace std;

// Functions for a 3D plane (represented in parametric form o + kd for real k)
struct Line3D {
  pt3 o, d;
  Line3D(pt3 p = pt3(), pt3 q = pt3()) : o(p), d(p - q) {}
  T distSq(ref3 p) const { return norm(d * (p - o)) / norm(d); }
  T dist(ref3 p) const { return sqrt(distSq(p)); }
  Line3D translate(ref3 p) const { return Line3D(o + p, d); }
  pt3 proj(ref3 p) const { return o + d * (d | (p - o)) / norm(d); }
  pt3 refl(ref3 p) const { return proj(p) * T(2) - p; }
  // compares points by projection (3 way comparison)
  int cmpProj(ref3 p, ref3 q) const { return sgn((d | p) - (d | q)); }
};
bool isParallel(const Line3D &l1, const Line3D &l2) {
  return eq(norm(l1.d * l2.d), 0);
}
bool isPerpendicular(const Line3D &l1, const Line3D &l2) {
  return eq(norm(l1.d | l2.d), 0);
}

// Closest distance between 2 lines
// Function Arguments:
//   l1: the first line
//   l2: the second line
// Return Value: the closest distance between 2 lines
// Time Complexity: O(1)
// Memory Complexity: O(1)
T lineLineDist(const Line3D &l1, const Line3D &l2) {
  pt3 n = l1.d * l2.d; if (eq(norm(n), 0)) return l1.dist(l2.o);
  return abs((l2.o - l1.o) | n) / abs(n);
}

// Closest point on the line l1 to the line l2
// Function Arguments:
//   l1: the first line
//   l2: the second line
// Return Value: the closest point on the line l1 to the line l2
// Time Complexity: O(1)
// Memory Complexity: O(1)
pt3 closestOnL1ToL2(const Line3D &l1, const Line3D &l2) {
  pt3 n2 = l2.d * (l1.d * l2.d);
  return l1.o + l2.d * ((l2.o - l1.o) | n2) / (l1.d | n2);
}

// Intersection of 2 lines
// Function Arguments:
//   l1: the first line
//   l2: the second line
//   res: a reference to the point to store the intersection if it exists
// Return Value: 0 if no intersection, 1 if point of intersection, 2 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
int lineIntersection(const Line3D &l1, const Line3D &l2, pt3 &res) {
  pt3 n = l1.d * l2.d; if (eq(norm(n), 0)) return eq(l1.dist(l2.o), 0) ? 2 : 0;
  res = closestOnL1ToL2(l1, l2); return 1;
}
