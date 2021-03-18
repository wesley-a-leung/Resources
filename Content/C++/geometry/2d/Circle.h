#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
#include "Line.h"
#include "../../datastructures/IntervalUnion.h"
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
  int contains(const Circle &c) const {
    T dr = r - c.r; return lt(dr, 0) ? 1 : sgn(distSq(o, c.o) - dr * dr);
  }
  // 1 if c is strictly outside this circle, 0 if outside and touching this
  //   circle, -1 otherwise
  int disjoint(const Circle &c) const {
    T sr = r + c.r; return sgn(sr * sr - distSq(o, c.o));
  }
  pt proj(ref p) const { return o + (p - o) * r / dist(o, p); }
  pt inv(ref p) const { return o + (p - o) * r * r / distSq(o, p); }
};

// Determine the intersection of a circle and a line
// Function Arguments:
//   c: the circle
//   l: the line
// Return Value: the points of intersection, if any, of the circle and
//   the line, guaranteed to be sorted based on projection on the line
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/noi05p6
vector<pt> circleLineIntersection(const Circle &c, const Line &l) {
  vector<pt> ret; T h2 = c.r * c.r - l.distSq(c.o); if (!lt(h2, 0)) {
    pt p = l.proj(c.o), h = l.v * sqrt(max(h2, T(0))) / abs(l.v);
    ret.push_back(p - h); pt q = p + h; if (ret.back() != q) ret.push_back(q);
  }
  return ret;
}

// Determine the area of the intersection of a circle and a half-plane defined
//   by the left side of a line
// Function Arguments:
//   c: the circle
//   l: the line with the half-plane defined by the left side
// Return Value: the area of the intersection of the circle and the half-plane
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/noi05p6
T circleHalfPlaneIntersectionArea(const Circle &c, const Line &l) {
  T h2 = c.r * c.r - l.distSq(c.o), ret = 0; if (!lt(h2, 0)) {
    pt p = l.proj(c.o), h = l.v * sqrt(max(h2, T(0))) / abs(l.v);
    pt a = p - h, b = p + h; T theta = abs(ang(a, c.o, b));
    ret = c.r * c.r * (theta - sin(theta)) / 2;
  }
  if (l.onLeft(c.o) > 0) ret = acos(T(-1)) * c.r * c.r - ret;
  return ret;
}

// Determine the intersection of two circles
// Function Arguments:
//   c1: the first circle
//   c2: the second circle
//   res: the points of intersection, if any, of the two circles;
//     the first point is guaranteed to not be on the left side of the
//     line from c1.o to c2.o
// Return Value: 0 if no intersection, 2 if identical circles, 1 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/420/problem/E
//   https://open.kattis.com/problems/drawingcircles
//   https://dmoj.ca/problem/noi05p6
int circleCircleIntersection(const Circle &c1, const Circle &c2,
                             vector<pt> &res) {
  pt d = c2.o - c1.o; T d2 = norm(d);
  if (eq(d2, 0)) return eq(c1.r, c2.r) ? 2 : 0;
  T pd = (d2 + c1.r * c1.r - c2.r * c2.r) / 2;
  T h2 = c1.r * c1.r - pd * pd / d2; if (!lt(h2, 0)) {
    pt p = c1.o + d * pd / d2, h = perp(d) * sqrt(max(h2 / d2, T(0)));
    res.push_back(p - h); pt q = p + h; if (res.back() != q) res.push_back(q);
  }
  return !res.empty();
}

// Determine the area of the intersection of two circles
// Function Arguments:
//   c1: the first circle
//   c2: the second circle
// Return Value: the area of the intersection of the two circles
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/600/problem/D
T circleCircleIntersectionArea(const Circle &c1, const Circle &c2) {
  T d = dist(c1.o, c2.o); if (!lt(d, c1.r + c2.r)) return 0;
  if (!lt(c2.r, d + c1.r)) return acos(T(-1)) * c1.r * c1.r;
  if (!lt(c1.r, d + c2.r)) return acos(T(-1)) * c2.r * c2.r;
  auto A = [&] (T r1, T r2) {
    T a = (d * d + r1 * r1 - r2 * r2) / (2 * d * r1);
    T theta = 2 * acos(max(T(-1), min(T(1), a)));
    return r1 * r1 * (theta - sin(theta)) / 2;
  };
  return A(c1.r, c2.r) + A(c2.r, c1.r);
}

// Determine the tangents of two circles
// Function Arguments:
//   c1: the first circle
//   c2: the second circle
//   inner: whether to find the inner or outer tangents
//   res: a vector of pairs of size 2 of the tangents, with each pair
//     representing a point on the first circle and the second circle;
//     the first point in each pair is guaranteed to not be on the left side of
//     the line from c1.o to c2.o
// Return Value: 0 if no tangents, 2 if identical circles, 1 otherwise
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/nccc7s4
//   https://dmoj.ca/problem/noi05p6
int circleCircleTangent(const Circle &c1, const Circle &c2, bool inner,
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

// Determines the circumcircle from 3 non-collinear points
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

// Computes the area of union of multiple circles
// Function Arguments:
//   circles: a vector of the circles
// Return Value: the area of the union of all the circles
// Time Complexity: O(N^2 log N)
// Memory Complexity: O(N^2)
// Tested:
//   https://www.spoj.com/problems/CIRU/
T circleUnion(const vector<Circle> &circles) {
  int n = circles.size(); vector<bool> inside(n, false);
  vector<vector<pair<Angle, Angle>>> intervals(n);
  for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
    int c = circles[j].contains(circles[i]);
    if (c < 0 || (c == 0 && (lt(circles[i].r, circles[j].r) || j < i)))
      inside[i] = true;
  }
  for (int i = 0; i < n; i++) if (!inside[i]) {
    for (int j = 0; j < n; j++) if (!inside[j]) {
      vector<pt> p; circleCircleIntersection(circles[i], circles[j], p);
      if (int(p.size()) == 2) {
        Angle a(p[0]), b(p[1]); Angle::setPivot(circles[i].o);
        if (a < b) intervals[i].emplace_back(a, b);
        else {
          intervals[i].emplace_back(a, circles[i].o);
          Angle c(circles[i].o - pt(circles[i].r, 0));
          intervals[i].emplace_back(c, b);
        }
      }
    }
  }
  T ret = 0; vector<vector<bool>> seen(n); vector<pt> P;
  for (int i = 0; i < n; i++) if (!inside[i]) {
    if (intervals[i].empty()) ret += acos(T(-1)) * circles[i].r * circles[i].r;
    else {
      Angle::setPivot(circles[i].o); intervalUnion(intervals[i]);
      if (intervals[i].back().second == circles[i].o) {
        intervals[i].front().first = intervals[i].back().first;
        intervals[i].pop_back();
      }
      int m = intervals[i].size(); vector<pair<Angle, Angle>> tmp;
      tmp.reserve(m); for (int j = 0; j < m; j++) {
        int k = j + 1 == m ? 0 : j + 1; P.push_back(intervals[i][j].second.p);
        tmp.emplace_back(intervals[i][j].second, intervals[i][k].first);
      }
      intervals[i] = tmp; seen[i].resize(m, false);
    }
  }
  vector<pair<int, int>> ind(P.size(), make_pair(-1, -1));
  sort(P.begin(), P.end()); for (int i = 0; i < n; i++) {
    for (int j = 0; j < int(intervals[i].size()); j++) {
      int k = lower_bound(P.begin(), P.end(), intervals[i][j].first.p)
          - P.begin();
      ind[k] = make_pair(i, j);
    }
  }
  for (int i = 0; i < n; i++) if (!inside[i]) {
    for (int j = 0; j < int(intervals[i].size()); j++) if (!seen[i][j]) {
      pt last = intervals[i][j].first.p; for (int a = i, b = j; !seen[a][b];) {
        seen[a][b] = true; pt cur = intervals[a][b].second.p;
        ret += cross(last, cur) / T(2);
        ret += circleHalfPlaneIntersectionArea(circles[a], Line(cur, last));
        int k = lower_bound(P.begin(), P.end(), cur) - P.begin();
        a = ind[k].first; b = ind[k].second; last = cur;
      }
    }
  }
  return ret;
}
