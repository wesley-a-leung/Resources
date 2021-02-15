#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"
using namespace std;

// Functions for 2D polygons
int mod(int i, int n) { return i < n ? i : i - n; }
// Determines twice the signed area of a simple polygon
// Function Arguments:
//   poly: the points of the simple polygon
// Return Value: twice the signed area of the polygon, positive if
//   counterclockwise, negative if clockwise
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/crane
T getArea2(const vector<pt> &poly) {
  T ret = 0; int n = poly.size();
  for (int i = 0; i < n; i++) ret += cross(poly[i], poly[mod(i + 1, n)]);
  return ret;
}
// Determines centroid of a simple polygon
// Function Arguments:
//   poly: the points of the simple polygon
// Return Value: the centroid of the polygon
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/crane
pt getCentroid(const vector<pt> &poly) {
  T A2 = 0; pt cen(0, 0); int n = poly.size(); for (int i = 0; i < n; i++) {
    T a = cross(poly[i], poly[mod(i + 1, n)]); A2 += a;
    cen += a * (poly[i] + poly[mod(i + 1, n)]);
  }
  return cen / A2 / T(3);
}
// Determines whether a point is inside a convex polygon
// Function Arguments:
//   poly: the points of the convex polygon
//   p: the point to check
// Return Value: -1 if inside the polygon, 0 if on the edge, 1 if outside
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/166/problem/B
int isInsideConvexPolygon(const vector<pt> &poly, ref p) {
  int n = poly.size(), a = 1, b = n - 1;
  if (n < 3) return onSeg(p, poly[0], poly.back()) ? 0 : 1;
  if (Line(poly[0], poly[a]).onLeft(poly[b])) swap(a, b);
  int o1 = Line(poly[0], poly[a]).onLeft(p);
  int o2 = Line(poly[0], poly[b]).onLeft(p);
  if (o1 > 0 || o2 < 0) return 1;
  if (o1 == 0 || o2 == 0) return 0;
  while (abs(a - b) > 1) {
    int c = (a + b) / 2; (Line(poly[0], poly[c]).onLeft(p) > 0 ? b : a) = c;
  }
  return ccw(poly[a], poly[b], p);
}
// Finds an extreme vertex of a convex polygon (a vertex where there are
//   no points in the polygon to the right of a vector drawn in
//   the specified direction from that point)
// Function Arguments:
//   poly: the points of the convex polygon
//   dir: the direction
// Return Value: the index of an extreme vertex
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/799/problem/G
int extremeVertex(const vector<pt> &poly, ref dir) {
  int n = poly.size(), lo = 0, hi = n; pt pp = perp(dir);
  auto cmp = [&] (int i, int j) {
    return sgn(cross(pp, poly[mod(i, n)] - poly[mod(j, n)]));
  };
  auto extr = [&] (int i) {
    return cmp(i + 1, i) >= 0 && cmp(i, i + n - 1) < 0;
  };
  if (extr(0)) return 0;
  while (lo + 1 < hi) {
    int m = lo + (hi - lo) / 2; if (extr(m)) return m;
    int ls = cmp(lo + 1, lo), ms = cmp(m + 1, m);
    (ls < ms || (ls == ms && ls == cmp(lo, m)) ? hi : lo) = m;
  }
  return lo;
}
// Finds the intersection of a convex polygon and a line
// Function Arguments:
//   poly: the points of the convex polygon in ccw order
//   line: the line
// Return Value: (-1, -1) if no collision
//               (i, -1) if touching corner i
//               (i, i) if along side (i, i + 1)
//               (i, j) if crossing sides (i, i + 1) and (j, j + 1)
//                 crossing corner i is treated as crossing side (i, i + 1)
//   first index in pair is guaranteed to NOT be on the left side of the line
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/799/problem/G
pair<int, int> convexPolygonLineIntersection(const vector<pt> &poly,
                                             const Line &line) {
  int n = poly.size(), endA = extremeVertex(poly, -perp(line.v));
  int endB = extremeVertex(poly, perp(line.v));
  auto cmpL = [&] (int i) { return line.onLeft(poly[i]); };
  pair<int, int> ret(-1, -1);
  if (cmpL(endA) > 0 || cmpL(endB) < 0) return ret;
  for (int i = 0; i < 2; i++) {
    int lo = endB, hi = endA; while (mod(lo + 1, n) != hi) {
      int m = mod((lo + hi + (lo < hi ? 0 : n)) / 2, n);
      (cmpL(m) == cmpL(endB) ? lo : hi) = m;
    }
    (i ? ret.second : ret.first) = mod(lo + !cmpL(hi), n); swap(endA, endB);
  }
  if (ret.first == ret.second) return make_pair(ret.first, -1);
  if (!cmpL(ret.first) && !cmpL(ret.second)) {
    switch ((ret.first - ret.second + n + 1) % n) {
      case 0: return make_pair(ret.first, ret.first);
      case 2: return make_pair(ret.second, ret.second);
    }
  }
  return ret;
}
// Determines the area of the intersection of a simple polygon and a circle
// Function Arguments:
//   poly: the points of the simple polygon in ccw order
//   c: the circle
// Return Value: the area of the intersection of the simple polygon and
//   the circle
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://ecna18.kattis.com/problems/pizzacutting
T polygonCircleIntersectionArea(const vector<pt> &poly, const Circle &c) {
  T r2 = c.r * c.r / 2;
  auto f = [&] (ref p, ref q) { return atan2(cross(p, q), dot(p, q)); };
  auto tri = [&] (ref p, ref q) {
    pt d = q - p; T a = dot(d, p) / norm(d);
    T b = (norm(p) - c.r * c.r) / norm(d), det = a * a - b;
    if (!lt(0, det)) return f(p, q) * r2;
    T s = max(T(0), -a - sqrt(det)), t = min(T(1), -a + sqrt(det));
    if (lt(t, 0) || !lt(s, 1)) return f(p, q) * r2;
    pt u = p + d * s, v = p + d * t;
    return f(p, u) * r2 + cross(u, v) / 2 + f(v, q) * r2;
  };
  T ret = 0; for (int n = poly.size(), i = 0; i < n; i++)
    ret += tri(poly[i] - c.o, poly[mod(i + 1, n)] - c.o);
  return ret;
}
