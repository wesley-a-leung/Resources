#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point.h"
#include "Line.h"
using namespace std;

// Functions for a 2D circle
struct Circle {
  pt o; T r; Circle(T r = 0) : o(0, 0), r(r) {}
  Circle(ref o, T r) : o(o), r(r) {}
  // -1 if p is inside this circle, 0 if on this circle,
  //   1 if outside this circle
  int contains(ref p) const { return sgn(distSq(o, p) - r * r); }
  // -1 if c is strictly inside this circle, 0 if inside and touching this
  //   circle, 1 otherwise
  int contains(const Circle &c) const { return sgn(c.r - r + dist(o, c.o)); }
  // 1 if c is strictly outside this circle, 0 if outside and touching this
  //   circle, -1 otherwise
  int disjoint(const Circle &c) const { return sgn(c.r + r - dist(o, c.o)); }
};
// Determine the intersection of a circle and a line
// Function Arguments:
//   c: the circle
//   l: the line
// Return Value: the points of intersection (if any) of the circle and the line
// Time Complexity: O(1)
// Memory Complexity: O(1)
vector<pt> circleLineIntersection(const Circle &c, const Line &l) {
  vector<pt> ret; T h2 = c.r * c.r - l.distSq(c.o); if (!lt(h2, 0)) {
    pt p = l.proj(c.o), h = l.v * sqrt(max(h2, T(0))) / abs(l.v);
    ret.push_back(p - h); ret.push_back(p + h);
  }
  sort(ret.begin(), ret.end(), pt_lt());
  ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end());
  return ret;
}
// Determine the area of the intersection of a circle and a half-plane defined
//   by the left side of a line
// Function Arguments:
//   c: the circle
//   l: the line with the half-plane defined by the left side
// Return Value: the are of the intersection of the circle and the half-plane
// Time Complexity: O(1)
// Memory Complexity: O(1)
T circleHalfPlaneIntersectionArea(const Circle &c, const Line &l) {
  T h2 = c.r * c.r - l.distSq(c.o), ret = 0; if (!lt(h2, 0)) {
    pt p = l.proj(c.o), h = l.v * sqrt(max(h2, T(0))) / abs(l.v);
    pt a = p - h, b = p + h; T theta = abs(ang(a, c.o, b));
    ret = c.r * c.r * (theta - sin(theta)) / 2;
  }
  if (l.onLeft(c.o) > 0) ret = PI * c.r * c.r - ret;
  return ret;
}
// Determine the intersection of two circles
// Function Arguments:
//   c1: the first circle
//   c2: the second circle
//   res: the points of intersection (if any) of the two circles
// Return Value: 0 if no intersection, 2 if identical circles, 1 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/420/problem/E
//   https://naq20.kattis.com/problems/drawingcircles
int circleCircleIntersection(const Circle &c1, const Circle &c2,
                             vector<pt> &res) {
  pt d = c2.o - c1.o; T d2 = norm(d);
  if (eq(d2, 0)) return eq(c1.r, c2.r) ? 2 : 0;
  T pd = (d2 + c1.r * c1.r - c2.r * c2.r) / 2;
  T h2 = c1.r * c1.r - pd * pd / d2; if (!lt(h2, 0)) {
    pt p = c1.o + d * pd / d2, h = perp(d) * sqrt(max(h2 / d2, T(0)));
    res.push_back(p - h); res.push_back(p + h);
  }
  sort(res.begin(), res.end(), pt_lt());
  res.erase(unique(res.begin(), res.end(), pt_eq()), res.end());
  return !res.empty();
}
// Determine the tangents of two circles
// Function Arguments:
//   c1: the first circle
//   c2: the second circle
//   inner: whether to find the inner or outer tangents
//   res: a vector of pairs of size 2 of the tangents, with each pair
//     representing a point on the first circle and the second circle
// Return Value: 0 if no tangents, 2 if identical circles, 1 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/nccc7s4
int circleTangentPoints(const Circle &c1, const Circle &c2, bool inner,
                       vector<pair<pt, pt>> &res) {
  pt d = c2.o - c1.o; T r2 = inner ? -c2.r : c2.r, dr = c1.r - r2;
  T d2 = norm(d), h2 = d2 - dr * dr;
  if (eq(d2, 0) || lt(h2, 0)) return eq(h2, 0) ? 2 : 0;
  for (T sign : {T(-1), T(1)}) {
    pt v = (d * dr + perp(d) * sqrt(max(h2, T(0))) * sign) / d2;
    res.emplace_back(c1.o + v * c1.r, c2.o + v * r2);
  }
  return 1;
}
// Determines the circumcircle from 3 collinear points
// Function Arguments:
//   a: the first point
//   b: the second point
//   c: the third point
// Return Value: the circumcircle of the 3 points
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://www.spoj.com/problems/QCJ4/
Circle circumcircle(pt a, pt b, pt c) {
  b -= a; c -= a;
  pt ret = b * c * (conj(c) - conj(b)) / (b * conj(c) - conj(b) * c);
  return Circle(a + ret, abs(ret));
}
