#pragma once
#include <bits/stdc++.h>
#include "Point3D.h"
#include "Line3D.h"
#include "Plane3D.h"
using namespace std;

// Functions for a 3D sphere
struct Sphere3D {
  pt3 o; T r; Sphere3D(T r = 0) : o(0, 0), r(r) {}
  Sphere3D(pt3 o, T r) : o(o), r(r) {}
  // 1 if p is inside this sphere, 0 if on this sphere,
  //   -1 if outside this sphere
  int contains(pt3 p) const { return sgn(r * r - distSq(o, p)); }
  // 1 if s is strictly inside this sphere, 0 if inside and touching this
  //   sphere, -1 otherwise
  int contains(Sphere3D s) const {
    T dr = r - s.r; return lt(dr, 0) ? -1 : sgn(dr * dr - distSq(o, s.o));
  }
  // 1 if s is strictly outside this sphere, 0 if outside and touching this
  //   sphere, -1 otherwise
  int disjoint(Sphere3D s) const {
    T sr = r + s.r; return sgn(distSq(o, s.o) - sr * sr);
  }
  pt3 proj(pt3 p) const { return o + (p - o) * r / dist(o, p); }
  pt3 inv(pt3 p) const { return o + (p - o) * r * r / distSq(o, p); }
  // Shortest distance on the sphere between the projections of a and b onto
  //   this sphere
  T greatCircleDist(pt3 a, pt3 b) const { return r * ang(a, o, b); }
  // Is a-b a valid great circle segment (not on opposite sides)
  bool isGreatCircleSeg(pt3 a, pt3 b) const {
    assert(contains(a) == 0 && contains(b) == 0);
    a -= o; b -= o; return !eq(norm(a * b), 0) || lt(0, (a | b)); 
  }
  // Returns whether p is on the great circle segment a-b
  bool onSphSeg(pt3 p, pt3 a, pt3 b) const {
    assert(isGreatCircleSeg(a, b)); p -= o; a -= o; b -= o; pt3 n = a * b;
    if (eq(norm(n), 0)) return eq(norm(a * p), 0) && lt(0, (a | p));
    return eq((n | p), 0) && !lt((n | a * p), 0) && !lt(0, (n | b * p));
  }
  // Returns the points of intersection (or segment of intersection) between
  //   the great circle segments a-b and p-q
  vector<pt3> greatCircleSegIntersection(pt3 a, pt3 b,
                                         pt3 p, pt3 q) const {
    assert(isGreatCircleSeg(a, b) && isGreatCircleSeg(p, q));
    pt3 ab = (a - o) * (b - o), pq = (p - o) * (q - o);
    int oa = sgn(pq | (a - o)), ob = sgn(pq | (b - o));
    int op = sgn(ab | (p - o)), oq = sgn(ab | (q - o));
    if (oa != ob && op != oq && oa != op)
      return vector<pt3>{proj(o + ab * pq * op)};
    vector<pt3> ret; if (onSphSeg(p, a, b)) ret.push_back(p);
    if (onSphSeg(q, a, b)) ret.push_back(q);
    if (onSphSeg(a, p, q)) ret.push_back(a);
    if (onSphSeg(b, p, q)) ret.push_back(b);
    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(), ret.end()), ret.end()); return ret;
  }
  // Returns the angle between 3 points projected onto the sphere,
  //   positive angle if a-b-c forms a ccw turn, negative if cw, 0 if collinear
  T angSph(pt3 a, pt3 b, pt3 c) const {
    a -= o; b -= o; c -= o; T theta = ang(b * a, pt3(0, 0, 0), b * c);
    return (a * b | c) < 0 ? -theta : theta;
  }
  // Returns the surface area of a polygon projected onto the sphere,
  //   inside area if points are in ccw order, outside area if points are
  //   in cw order
  T surfaceAreaOnSph(const vector<pt3> &poly) const {
    int n = poly.size(); T PI = acos(T(-1)), a = -(n - 2) * PI;
    for (int i = 0; i < n; i++) {
      T ang = angSph(poly[i], poly[(i + 1) % n], poly[(i + 2) % n]);
      if (ang < 0) ang += 2 * PI;
      a += ang;
    }
    return r * r * a;
  }
};

// Determine the intersection of a sphere and a line
// Function Arguments:
//   s: the sphere
//   l: the line
// Return Value: the points of intersection (if any) of the sphere and
//   the line, guaranteed to be sorted based on projection on the line
// Time Complexity: O(1)
// Memory Complexity: O(1)
vector<pt3> sphereLineIntersection(Sphere3D s, Line3D l) {
  vector<pt3> ret; T h2 = s.r * s.r - l.distSq(s.o); pt3 p = l.proj(s.o);
  if (eq(h2, 0)) ret.push_back(p);
  else if (lt(0, h2)) {
    pt3 h = l.d * sqrt(h2) / abs(l.d);
    ret.push_back(p - h); ret.push_back(p + h);
  }
  return ret;
}

// Determine the intersection of a sphere and a plane
// Function Arguments:
//   s: the sphere
//   pi: the plane
//   res: a pair of pt3 and T, representing the center of the circle, and the
//     radius of the circle of intersection if it exists, guaranteed to be on
//     the plane pi
// Return Value: a boolean indicating whether an intersection exists or not
// Time Complexity: O(1)
// Memory Complexity: O(1)
bool spherePlaneIntersection(Sphere3D s, Plane3D pi, pair<pt3, T> &res) {
  T d2 = s.r * s.r - pi.distSq(s.o); if (lt(d2, 0)) return false;
  res.first = pi.proj(s.o); res.second = sqrt(max(d2, T(0))); return true;
}

// Determine the surface area and volume of the sphere above the intersection
//   of a sphere and a half-space defined by the space above a plane
// Function Arguments:
//   s: the sphere
//   pi: the plane with the half-space defined as the space above the plane
// Return Value: a pair containing the surface area and the volume of the
//   sphere above the intersection of the sphere and the half-space
// Time Complexity: O(1)
// Memory Complexity: O(1)
pair<T, T> sphereHalfSpaceIntersectionSAV(Sphere3D s, Plane3D pi) {
  T d2 = s.r * s.r - pi.distSq(s.o);
  T h = lt(d2, 0) ? T(0) : s.r - abs(pi.dist(s.o));
  if (pi.isAbove(s.o) > 0) h = s.r * 2 - h;
  T PI = acos(T(-1));
  return make_pair(PI * 2 * s.r * h, PI * h * h / 3 * (3 * s.r - h));
}

// Determine the intersection of two spheres
// Function Arguments:
//   c1: the first circle
//   c2: the second circle
//   c: a tuple containing the plane the circle lies on (pointing away
//     from s1), the center of the circle, and the radius
// Return Value: 0 if no intersection, 2 if identical spheres, 1 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
int sphereSphereIntersection(Sphere3D s1, Sphere3D s2,
                             tuple<Plane3D, pt3, T> &c) {
  pt3 d = s2.o - s1.o; T d2 = norm(d);
  if (eq(d2, 0)) return eq(s1.r, s2.r) ? 2 : 0;
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2, h2 = s1.r * s1.r - pd * pd / d2;
  if (lt(h2, 0)) return 0;
  pt3 o = s1.o + d * pd / d2;
  c = make_tuple(Plane3D(d, o), o, sqrt(max(h2, T(0)))); return 1;
}

// Determine the surface area and volume of the intersection of two spheres
// Function Arguments:
//   s1: the first sphere
//   s2: the second sphere
// Return Value: a pair containing the surface area and volume of the
//   intersection of the two spheres
// Time Complexity: O(1)
// Memory Complexity: O(1)
pair<T, T> sphereSphereIntersectionSAV(Sphere3D s1, Sphere3D s2) {
  pt3 d = s2.o - s1.o; T d2 = norm(d), dr = abs(s1.r - s2.r), PI = acos(T(-1));
  if (!lt(dr * dr, d2)) {
    T r = min(s1.r, s2.r);
    return make_pair(PI * 4 * r * r, PI * 4 * r * r * r / 3);
  }
  T sr = s1.r + s2.r; if (lt(sr * sr, d2)) return make_pair(T(0), T(0));
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2;
  Plane3D pi = Plane3D(d, s1.o + d * pd / d2);
  pair<T, T> a = sphereHalfSpaceIntersectionSAV(s1, pi);
  pair<T, T> b = sphereHalfSpaceIntersectionSAV(s2, Plane3D(-pi.n, -pi.d));
  a.first += b.first; a.second += b.second; return a;
}
