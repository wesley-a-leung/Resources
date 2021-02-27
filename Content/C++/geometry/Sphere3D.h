#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point3D.h"
#include "Line3D.h"
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
