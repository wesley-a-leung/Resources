#pragma once
#include <bits/stdc++.h>
#include "Point3D.h"
using namespace std;

// Functions for a 3D plane (represented in parametric form o + kd for real k)
struct Line3D {
  pt3 o, d;
  // points p and q
  Line3D(pt3 p = pt3(0, 0, 0), pt3 q = pt3(0, 0, 0)) : o(p), d(q - p) {}
  bool onLine(pt3 p) const { return eq(norm(d * (p - o)), 0); }
  T distSq(pt3 p) const { return norm(d * (p - o)) / norm(d); }
  T dist(pt3 p) const { return sqrt(distSq(p)); }
  Line3D translate(pt3 p) const { return Line3D(o + p, d); }
  pt3 proj(pt3 p) const { return o + d * (d | (p - o)) / norm(d); }
  pt3 refl(pt3 p) const { return proj(p) * T(2) - p; }
  // compares points by projection (3 way comparison)
  int cmpProj(pt3 p, pt3 q) const { return sgn((d | p) - (d | q)); }
};

// Closest distance between 2 lines
// Function Arguments:
//   l1: the first line
//   l2: the second line
// Return Value: the closest distance between 2 lines
// Time Complexity: O(1)
// Memory Complexity: O(1)
T lineLineDist(Line3D l1, Line3D l2) {
  pt3 n = l1.d * l2.d; if (eq(norm(n), 0)) return l1.dist(l2.o);
  return abs((l2.o - l1.o) | n) / abs(n);
}

// Closest point on the line l1 to the line l2
// If l1 and l2 are parallel, it returns the projection of (0, 0, 0) on l1
// Function Arguments:
//   l1: the first line
//   l2: the second line
// Return Value: the closest point on the line l1 to the line l2
// Time Complexity: O(1)
// Memory Complexity: O(1)
pt3 closestOnL1ToL2(Line3D l1, Line3D l2) {
  pt3 n = l1.d * l2.d; if (eq(norm(n), 0)) return l1.proj(pt3(0, 0, 0));
  pt3 n2 = l2.d * n; return l1.o + l2.d * ((l2.o - l1.o) | n2) / (l1.d | n2);
}

// Intersection of 2 lines
// Function Arguments:
//   l1: the first line
//   l2: the second line
//   res: a reference to the point to store the intersection if it exists
// Return Value: 0 if no intersection, 1 if point of intersection, 2 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
int lineLineIntersection(Line3D l1, Line3D l2, pt3 &res) {
  pt3 n = l1.d * l2.d; if (eq(norm(n), 0)) return eq(l1.dist(l2.o), 0) ? 2 : 0;
  res = closestOnL1ToL2(l1, l2); return 1;
}

// Determines if a point is on a line segment
// Function Arguments:
//   p: the point to check if on the line segment
//   a: one endpoint of the line segment
//   b: the other endpoint of the line segment
// Return Value: true if p is on the line segment a-b, false otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
bool onSeg(pt3 p, pt3 a, pt3 b) {
  if (a == b) return p == a;
  Line3D l(a, b);
  return l.onLine(p) && l.cmpProj(a, p) <= 0 && l.cmpProj(p, b) <= 0;
}

// Determine the intersection of two line segments
// Function Arguments:
//   a: one endpoint of the first line segment
//   b: the other endpoint of the first line segment
//   p: one endpoint of the second line segment
//   q: the other endpoint of the second line segment
// Return Value: if the line segments do not intersect, an empty vector
//   of points; if the line segments intersect at a point, a vector containing
//   the point of intersection; if the line segments have a line segment of
//   intersection, a vector containing the two endpoints of the
//   line segment intersection (it can return more if there is precision error)
vector<pt3> segSegIntersection(pt3 a, pt3 b, pt3 p, pt3 q) {
  vector<pt3> ret; if (a == b) {
    if (onSeg(a, p, q)) ret.push_back(a);
  } else if (p == q) {
    if (onSeg(p, a, b)) ret.push_back(p);
  } else {
    pt3 inter; Line3D l1(a, b), l2(p, q);
    int cnt = lineLineIntersection(l1, l2, inter);
    if (cnt == 1) ret.push_back(inter);
    else if (cnt == 2) {
      if (onSeg(p, a, b)) ret.push_back(p);
      if (onSeg(q, a, b)) ret.push_back(q);
      if (onSeg(a, p, q)) ret.push_back(a);
      if (onSeg(b, p, q)) ret.push_back(b);
    }
  }
  sort(ret.begin(), ret.end());
  ret.erase(unique(ret.begin(), ret.end()), ret.end()); return ret;
}

// Finds the closest point on a line segment to another point
// Function Arguments
//   p: the reference point
//   a: one endpoint of the line segment
//   b: the other endpoint of the line segment
// Return Value: the closest point to p on the line segment a-b
// Time Complexity: O(1)
// Memory Complexity: O(1)
pt3 closestPtOnSeg(pt3 p, pt3 a, pt3 b) {
  if (a != b) {
    Line3D l(a, b);
    if (l.cmpProj(a, p) < 0 && l.cmpProj(p, b) < 0) return l.proj(p);
  }
  return lt(dist(p, a), dist(p, b)) ? a : b;
}

// Finds the distance to the closest point on a line segment to another point
// Function Arguments
//   p: the reference point
//   a: one endpoint of the line segment
//   b: the other endpoint of the line segment
// Return Value: the distance to the closest point to p on the line segment a-b
// Time Complexity: O(1)
// Memory Complexity: O(1)
T ptSegDist(pt3 p, pt3 a, pt3 b) {
  if (a != b) {
    Line3D l(a, b);
    if (l.cmpProj(a, p) < 0 && l.cmpProj(p, b) < 0) return l.dist(p);
  }
  return min(dist(p, a), dist(p, b));
}

// Finds the closest distance between two line segments
// Function Arguments
//   a: one endpoint of the first line segment
//   b: the other endpoint of the first line segment
//   p: one endpoint of the second line segment
//   q: the other endpoint of the second line segment
// Return Value: the closest distance between the two line 
// Time Complexity: O(1)
// Memory Complexity: O(1)
T segSegDist(pt3 a, pt3 b, pt3 p, pt3 q) {
  return !segSegIntersection(a, b, p, q).empty()
      ? 0
      : min({ptSegDist(p, a, b), ptSegDist(q, a, b),
             ptSegDist(a, p, q), ptSegDist(b, p, q)});
}
