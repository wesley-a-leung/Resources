#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point.h"
using namespace std;

// Functions for a 2D line
struct Line {
  pt v; T c;
  // ax + by = c
  Line(T a = 0, T b = 0, T c = 0) : v(b, -a), c(c) {}
  // direction vector v with offset c
  Line(ref v, T c) : v(v), c(c) {}
  // points p and q
  Line(ref p, ref q) : v(q - p), c(cross(v, p)) {}
  T eval(ref p) const { return cross(v, p) - c; }
  // 1 if left of line, 0 if on line, -1 if right of line
  int onLeft(ref p) const { return sgn(eval(p)); }
  T distSq(ref p) const { T e = eval(p); return e * e / norm(v); }
  T dist(ref p) const { return abs(eval(p) / abs(v)); }
  // rotated 90 degrees ccw
  Line perpThrough(ref p) const { return Line(p, p + perp(v)); }
  Line translate(ref p) const { return Line(v, c + cross(v, p)); }
  Line shiftLeft(T d) const { return Line(v, c + d * abs(v)); }
  pt proj(ref p) const { return p - perp(v) * eval(p) / norm(v); }
  pt refl(ref p) const { return p - perp(v) * T(2) * eval(p) / norm(v); }
  // compares points by orthogonal projection (3 way comparison)
  int cmpProj(ref p, ref q) const { return sgn(dot(v, p) - dot(v, q)); }
};

// Bisector of 2 lines
// Function Arguments:
//   l1: the first line
//   l2: the second line
//   interior: whether the interior or exterior bisector is used
// Return Value: a line representing the bisector
// Time Complexity: O(1)
// Memory Complexity: O(1)
Line bisector(const Line &l1, const Line &l2, bool interior) {
  T s = interior ? 1 : -1;
  return Line(l2.v / abs(l2.v) + l1.v / abs(l1.v) * s,
              l2.c / abs(l2.v) + l1.c / abs(l1.v) * s);
}

// Intersection of 2 lines
// Function Arguments:
//   l1: the first line
//   l2: the second line
//   res: a reference to a point to store the intersection if it exists
// Return Value: 0 if no intersection, 1 if proper intersection, 2 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/nccc7s5
int lineIntersection(const Line &l1, const Line &l2, pt &res) {
  T d = cross(l1.v, l2.v);
  if (eq(d, 0)) return pt_eq()(l2.v * l1.c, l1.v * l2.c) ? 2 : 0;
  res = (l2.v * l1.c - l1.v * l2.c) / d; return 1;
}

// Determines if a point is on a line segment
// Function Arguments:
//   p: the point to check if on the line segment
//   a: one endpoint of the line segment
//   b: the other endpoint of the line segment
// Return Value: true if p is on the line segment a-b, false otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/segmentintersection
bool onSeg(ref p, ref a, ref b) {
  return !ccw(p, a, b) && !lt(0, dot(a - p, b - p));
}

// Determine if two line segments intersect
// Function Arguments:
//   a: one endpoint of the first line segment
//   b: the other endpoint of the first line segment
//   p: one endpoint of the second line segment
//   q: the other endpoint of the second line segment
// Return Value: 0 if no intersection, 1 if proper intersection (a single
//   point and not an endpoint), 2 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/segmentintersection
int lineSegIntersects(ref a, ref b, ref p, ref q) {
  int o1 = ccw(a, b, p), o2 = ccw(a, b, q), o3 = ccw(p, q, a);
  int o4 = ccw(p, q, b); if (o1 != o2 && o3 != o4) return 1;
  else if ((!o1 && onSeg(p, a, b)) || (!o2 && onSeg(q, a, b))
           || (!o3 && onSeg(a, p, q)) || (!o4 && onSeg(b, p, q)))
    return 2;
  else return 0;
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
//   line segment intersection
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/segmentintersection
vector<pt> lineSegIntersection(ref a, ref b, ref p, ref q) {
  int intersects = lineSegIntersects(a, b, p, q);
  if (!intersects) return vector<pt>();
  else if (intersects == 1) {
    T c1 = cross(p - a, b - a), c2 = cross(q - a, b - a);
    return vector<pt>{(c1 * q - c2 * p) / (c1 - c2)};
  }
  vector<pt> ret; if (onSeg(p, a, b)) ret.push_back(p);
  if (onSeg(q, a, b)) ret.push_back(q);
  if (onSeg(a, p, q)) ret.push_back(a);
  if (onSeg(b, p, q)) ret.push_back(b);
  sort(ret.begin(), ret.end(), pt_lt());
  ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end()); return ret;
}

// Finds the closest point on a line segment to another point
// Function Arguments
//   p: the reference point
//   a: one endpoint of the line segment
//   b: the other endpoint of the line segment
// Return Value: the closest point to p on the line segment a-b
// Time Complexity: O(1)
// Memory Complexity: O(1)
pt closestPtToSeg(ref p, ref a, ref b) {
  if (a != b) {
    Line l(a, b);
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
// Tested:
//   https://open.kattis.com/problems/segmentdistance
T segPtDist(ref p, ref a, ref b) {
  if (a != b) {
    Line l(a, b);
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
// Tested:
//   https://open.kattis.com/problems/segmentdistance
T segSegDist(ref a, ref b, ref p, ref q) {
  return lineSegIntersects(a, b, p, q) > 0
      ? 0
      : min({segPtDist(p, a, b), segPtDist(q, a, b),
             segPtDist(a, p, q), segPtDist(b, p, q)});
}
