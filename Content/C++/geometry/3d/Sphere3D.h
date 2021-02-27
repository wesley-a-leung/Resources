#pragma once
#include <bits/stdc++.h>
#include "Point3D.h"
#include "Line3D.h"
#include "Plane3D.h"
using namespace std;

// Functions for a 3D sphere
struct Sphere {
  pt3 o; T r; Sphere(T r = 0) : o(0, 0), r(r) {}
  Sphere(ref3 o, T r) : o(o), r(r) {}
  // -1 if p is inside this sphere, 0 if on this sphere,
  //   1 if outside this sphere
  int contains(ref3 p) const { return sgn(distSq(o, p) - r * r); }
  // -1 if s is strictly inside this sphere, 0 if inside and touching this
  //   sphere, 1 otherwise
  int contains(const Sphere &s) const { return sgn(s.r - r + dist(o, s.o)); }
  // 1 if s is strictly outside this sphere, 0 if outside and touching this
  //   sphere, -1 otherwise
  int disjoint(const Sphere &s) const { return sgn(s.r + r - dist(o, s.o)); }
  pt3 proj(ref3 p) const { return o + (p - o) * r / dist(o, p); }
  pt3 inv(ref3 p) const { return o + (p - o) * r * r / distSq(o, p); }
  // Shortest distance on the sphere between the projections of a and b onto
  //   this sphere
  T greatCircleDist(ref3 a, ref3 b) const { return r * ang(a - o, b - o); }
};

// Determine the intersection of a sphere and a line
// Function Arguments:
//   s: the sphere
//   l: the line
// Return Value: the points of intersection (if any) of the sphere and the line
// Time Complexity: O(1)
// Memory Complexity: O(1)
vector<pt3> sphereLineIntersection(const Sphere &s, const Line3D &l) {
  vector<pt3> ret; T h2 = s.r * s.r - l.distSq(s.o); if (!lt(h2, 0)) {
    pt3 p = l.proj(s.o), h = l.d * sqrt(max(h2, T(0))) / abs(l.d);
    ret.push_back(p - h); ret.push_back(p + h);
  }
  sort(ret.begin(), ret.end());
  ret.erase(unique(ret.begin(), ret.end()), ret.end()); return ret;
}

// Determine the intersection of a sphere and a plane
// Function Arguments:
//   s: the sphere
//   pi: the plane
//   res: a pair of pt3 and T, representing the centre of the circle, and the
//     radius of the circle of intersection if it exists, guaranteed to be on
//     the plane pi
// Return Value: a boolean indicating whether an intersection exists or not
// Time Complexity: O(1)
// Memory Complexity: O(1)
bool spherePlaneIntersection(const Sphere &s, const Plane3D &pi,
                             pair<pt3, T> &res) {
  T d2 = s.r * s.r - pi.distSq(s.o); if (lt(d2, 0)) return false;
  res.first = pi.proj(s.o); res.second = sqrt(max(d2, T(0))); return true;
}

// Determine the surface area of the sphere above the intersection of
//   a sphere and a half-space defined by the space above a plane
// Function Arguments:
//   s: the sphere
//   pi: the plane with the half-space defined as the space above the plane
// Return Value: the surface area of the sphere above the intersection of
//   the sphere and the half-space
// Time Complexity: O(1)
// Memory Complexity: O(1)
T sphereHalfSpaceIntersectionSurfaceArea(const Sphere &s, const Plane3D &pi) {
  T d2 = s.r * s.r - pi.distSq(s.o), h = lt(d2, 0) ? 0 : s.r - pi.dist(s.o);
  if (pi.isAbove(s.o) > 0) h = s.r * 2 - h;
  return acos(T(-1)) * 2 * s.r * h;
}

// Determine the volume of the sphere above the intersection of
//   a sphere and a half-space defined by the space above a plane
// Function Arguments:
//   s: the sphere
//   pi: the plane with the half-space defined as the space above the plane
// Return Value: the volume of the sphere above the intersection of
//   the sphere and the half-space
// Time Complexity: O(1)
// Memory Complexity: O(1)
T sphereHalfSpaceIntersectionVolume(const Sphere &s, const Plane3D &pi) {
  T d2 = s.r * s.r - pi.distSq(s.o), h = lt(d2, 0) ? 0 : s.r - pi.dist(s.o);
  if (pi.isAbove(s.o) > 0) h = s.r * 2 - h;
  return acos(T(-1)) * h * h / 3 * (3 * s.r - h);
}

// Determine the intersection of two spheres
// Function Arguments:
//   s1: the first sphere
//   s2: the second sphere
//   pi: the plane of intersection, if it exists, of the two spheres,
//     with the normal pointing in the direction from s1.o to s2.o
//   c: a pair of pt3 and T, representing the centre of the circle, and the
//     radius of the circle of intersection if it exists, guaranteed to be on
//     the plane pi
// Return Value: 0 if no intersection, 2 if identical spheres, 1 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
int sphereSphereIntersection(const Sphere &s1, const Sphere &s2,
                             Plane3D &pi, pair<pt3, T> &c) {
  pt3 d = s2.o - s1.o; T d2 = norm(d);
  if (eq(d2, 0)) return eq(s1.r, s2.r) ? 2 : 0;
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2, h2 = s1.r * s1.r - pd * pd / d2;
  if (lt(h2, 0)) return 0;
  c.second = sqrt(max(h2, T(0)));
  pi = Plane3D(d, c.first = s1.o + d * pd / d2); return 1;
}

// Determine the surface area of the union of two spheres
// Function Arguments:
//   s1: the first sphere
//   s2: the second sphere
// Return Value: the surface area of the union of the two spheres
// Time Complexity: O(1)
// Memory Complexity: O(1)
T sphereSphereUnionSurfaceArea(const Sphere &s1, const Sphere &s2) {
  pt3 d = s2.o - s1.o; T d2 = norm(d), dr = abs(s1.r - s2.r);
  if (!lt(dr * dr, d2)) {
    T r = max(s1.r, s2.r); return acos(T(-1)) * 4 * r * r;
  }
  T sr = s1.r + s2.r; if (lt(sr * sr, d2)) {
    return acos(T(-1)) * 4 * (s1.r * s1.r + s2.r * s2.r);
  }
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2;
  Plane3D pi = Plane3D(d, s1.o + d * pd / d2);
  T ret = sphereHalfSpaceIntersectionSurfaceArea(s2, pi);
  ret += sphereHalfSpaceIntersectionSurfaceArea(s1, Plane3D(-pi.n, -pi.d));
  return ret;
}

// Determine the volume of the union of two spheres
// Function Arguments:
//   s1: the first sphere
//   s2: the second sphere
// Return Value: the volume of the union of the two spheres
// Time Complexity: O(1)
// Memory Complexity: O(1)
T sphereSphereUnionVolume(const Sphere &s1, const Sphere &s2) {
  pt3 d = s2.o - s1.o; T d2 = norm(d), dr = abs(s1.r - s2.r);
  if (!lt(dr * dr, d2)) {
    T r = max(s1.r, s2.r); return acos(T(-1)) * 4 * r * r * r / 3;
  }
  T sr = s1.r + s2.r; if (lt(sr * sr, d2)) {
    return acos(T(-1)) * 4 * (s1.r * s1.r * s1.r + s2.r * s2.r * s2.r) / 3;
  }
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2;
  Plane3D pi = Plane3D(d, s1.o + d * pd / d2);
  T ret = sphereHalfSpaceIntersectionVolume(s2, pi);
  ret += sphereHalfSpaceIntersectionVolume(s1, Plane3D(-pi.n, -pi.d));
  return ret;
}
