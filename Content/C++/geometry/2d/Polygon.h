#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "../../datastructures/IntervalUnion.h"
using namespace std;

// Functions for 2D polygons, represented by a vector of the N points in
//   the polygon
int mod(int i, int n) { return i < n ? i : i - n; }

// Determines twice the signed area of a simple polygon
// Function Arguments:
//   poly: the points of the simple polygon
// Return Value: twice the signed area of the polygon, positive if
//   ccw, negative if cw
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/polygonarea
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

// Determines the orientation of a convex polygon
// Function Arguments:
//   poly: the points of the convex polygon
// Return Value: 1 if ccw, -1 if cw, 0 if a point or a line
// Time Complexity: O(1)
// Memory Complexity: O(1)
int isCcwConvexPolygon(const vector<pt> &poly) {
  return ccw(poly.back(), poly[0], poly[mod(1, poly.size())]);
}

// Determines the orientation of a simple polygon
// Function Arguments:
//   poly: the points of the simple polygon
// Return Value: 1 if ccw, -1 if cw, 0 if a point or a line
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/abstractart
int isCcwPolygon(const vector<pt> &poly) {
  int n = poly.size();
  int i = min_element(poly.begin(), poly.end()) - poly.begin();
  return ccw(poly[mod(i + n - 1, n)], poly[i], poly[mod(i + 1, n)]);
}

// Determines whether a point is inside a convex polygon
// Function Arguments:
//   poly: the points of the convex polygon in ccw order or cw order
//   p: the point to check
// Return Value: -1 if inside the polygon, 0 if on the edge, 1 if outside
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/166/problem/B
int isInConvexPolygon(const vector<pt> &poly, pt p) {
  int n = poly.size(), a = 1, b = n - 1;
  if (n < 3) return onSeg(p, poly[0], poly.back()) ? 0 : 1;
  if (ccw(poly[0], poly[a], poly[b]) > 0) swap(a, b);
  int o1 = ccw(poly[0], poly[a], p), o2 = ccw(poly[0], poly[b], p);
  if (o1 > 0 || o2 < 0) return 1;
  if (o1 == 0 || o2 == 0) return 0;
  while (abs(a - b) > 1) {
    int c = (a + b) / 2; (ccw(poly[0], poly[c], p) > 0 ? b : a) = c;
  }
  return ccw(poly[a], poly[b], p);
}

// Determines whether a point is inside a simple polygon
// Function Arguments:
//   poly: the points of the simple polygon in ccw order or cw order
//   p: the point to check
// Return Value: -1 if inside the polygon, 0 if on the edge, 1 if outside
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/pointinpolygon
int isInPolygon(const vector<pt> &poly, pt p) {
  int n = poly.size(), windingNumber = 0; for (int i = 0; i < n; i++) {
    pt a = poly[i], b = poly[mod(i + 1, n)]; if (lt(b.y, a.y)) swap(a, b);
    if (onSeg(p, a, b)) return 0;
    windingNumber ^= (!lt(p.y, a.y) && lt(p.y, b.y) && ccw(p, a, b) > 0);
  }
  return windingNumber == 0 ? 1 : -1;
}

// Finds an extreme vertex of a convex polygon (a vertex that is the furthest
//   point in that direction, selecting the rightmost vertex if there are
//   multiple)
// Function Arguments:
//   poly: the points of the convex polygon in ccw order
//   dir: the direction
// Return Value: the index of an extreme vertex
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/799/problem/G
//   https://www.acmicpc.net/problem/4225
int extremeVertex(const vector<pt> &poly, pt dir) {
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
//   l: the line
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
                                             const Line &l) {
  int n = poly.size(), endA = extremeVertex(poly, -perp(l.v));
  int endB = extremeVertex(poly, perp(l.v));
  auto cmpL = [&] (int i) { return l.onLeft(poly[i]); };
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

// Finds a single tangent of a convex polygon and a point strictly outside
//   the polygon
// Function Arguments:
//   poly: the points of the convex polygon in ccw order
//   p: the point strictly outside the polygon
//   left: whether the left or right tangent line is found if p is
//     considered to be below the polygon
// Return Value: the tangent index, with the closest point to p being
//   selected if there are multiple indices
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/coci19c2p5
//   Fuzz Tested
int convexPolygonPointSingleTangent(const vector<pt> &poly, pt p, bool left) {
  int n = poly.size(), o = ccw(p, poly[0], poly.back());
  bool farSide = o ? o < 0 : lt(distSq(p, poly.back()), distSq(p, poly[0]));
  int lo = farSide != left, hi = lo + n - 2; while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (ccw(p, poly[0], poly[mid]) == (left ? -1 : 1)) {
      if (farSide == left) hi = mid - 1;
      else lo = mid + 1;
    } else {
      if ((ccw(poly[mid], poly[mod(mid + 1, n)], p) < 0) == left) hi = mid - 1;
      else lo = mid + 1;
    }
  }
  return mod(lo, n);
}

// Finds the tangent of a convex polygon and a point strictly outside
//   the polygon
// Function Arguments:
//   poly: the points of the convex polygon in ccw order
//   p: the point strictly outside the polygon
// Return Value: a pair containing the tangent indices, with the first index
//   being the left tangent point and the second index being the right tangent
//   if p is considered to be below the polygon; all points strictly between
//   the tangent indices are strictly within the tangent lines, while all other
//   points are on or outside the tangent lines
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/coci19c2p5
//   Fuzz Tested
pair<int, int> convexPolygonPointTangent(const vector<pt> &poly, pt p) {
  return make_pair(convexPolygonPointSingleTangent(poly, p, true),
                   convexPolygonPointSingleTangent(poly, p, false));
}

// Finds the tangent of a convex polygon and a circle strictly and completely
//   outside the polygon
// Function Arguments:
//   poly: the points of the convex polygon in ccw order
//   c: the circle strictly and completely outside the polygon
//   inner: whether to find the inner or outer tangents
// Return Value: a pair containing the tangent indices, with the first index
//   being the left tangent point and the second index being the right tangent
//   if c is considered to be below the polygon; all points strictly between
//   the tangent indices are strictly within the tangent lines, while all other
//   points are on or outside the tangent lines (same index means all points
//   are inside or on the tangent lines)
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
pair<int, int> convexPolygonCircleTangent(const vector<pt> &poly,
                                          const Circle &c, bool inner) {
  int n = poly.size(), a = 0, b = 0; vector<pair<pt, pt>> t;
  for (int h = 0; h < 2; h++) {
    assert(circleCircleTangent(Circle(poly[0], 0), c, inner, t) == 1);
    pt q = t[h].second; int o = ccw(q, poly[0], poly.back());
    bool farSide = o ? o < 0 : lt(distSq(q, poly.back()), distSq(q, poly[0]));
    int lo = farSide == h, hi = lo + n - 2; while (lo <= hi) {
      int mid = lo + (hi - lo) / 2; t.clear();
      assert(circleCircleTangent(Circle(poly[mid], 0), c, inner, t) == 1);
      q = t[h].second; if (ccw(q, poly[0], poly[mid]) == (h ? 1 : -1)) {
        if (farSide != h) hi = mid - 1;
        else lo = mid + 1;
      } else {
        if ((ccw(poly[mid], poly[mod(mid + 1, n)], q) < 0) != h) hi = mid - 1;
        else lo = mid + 1;
      }
    }
    (h ? b : a) = mod(lo, n); t.clear();
  }
  return make_pair(a, b);
}

// Finds the tangents of two convex polygons that do not intersect
// Function Arguments:
//   poly1: the first convex polygon
//   poly2: the second convex polygon that does not intersect with the first
//   inner: whether to find the inner or outer tangents
// Return Value: a vector of pairs containing the tangent indices, with the
//   first element in each pair being the index in the first polygon, and the
//   second element being the index in the second polygon; first point in
//   each pair is the left tangent point of the first polygon if the second
//   polygon is considered to be below the first polygon
// Time Complexity: O(log N log M)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
vector<pair<int, int>> convexPolygonConvexPolygonTangent(
    const vector<pt> &poly1, const vector<pt> &poly2, bool inner) {
  int n = poly1.size(), a = 0, b = 0, c = 0, d = 0; vector<pair<int, int>> ret;
  for (int h = 0; h < 2; h++) {
    pt q = poly2[convexPolygonPointSingleTangent(poly2, poly1[0], inner ^ h)];
    int o = ccw(q, poly1[0], poly1.back());
    bool farSide = o ? o < 0
                     : lt(distSq(q, poly1.back()), distSq(q, poly1[0]));
    int lo = farSide == h, hi = lo + n - 2; while (lo <= hi) {
      int mid = lo + (hi - lo) / 2;
      q = poly2[convexPolygonPointSingleTangent(poly2, poly1[mid], inner ^ h)];
      if (ccw(q, poly1[0], poly1[mid]) == (h ? 1 : -1)) {
        if (farSide != h) hi = mid - 1;
        else lo = mid + 1;
      } else {
        if ((ccw(poly1[mid], poly1[mod(mid + 1, n)], q) < 0) != h)
          hi = mid - 1;
        else lo = mid + 1;
      }
    }
    (h ? b : a) = lo = mod(lo, n);
    (h ? d : c) = convexPolygonPointSingleTangent(poly2, poly1[lo], inner ^ h);
  }
  ret.emplace_back(a, c); ret.emplace_back(b, d); return ret;
}

// Finds the closest point on the edge of the polygon to a point strictly
//   outside the polygon
// Function Arguments:
//   poly: the points of the convex polygon in ccw order
//   p: the point strictly outside the polygon
// Return Value: the closest point on the edge of the polygon to the point p
// Time Complexity: O(log N)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
pt closestPointOnConvexPolygon(const vector<pt> &poly, pt p) {
  pair<int, int> tangent = convexPolygonPointTangent(poly, p);
  int n = poly.size(), len = tangent.second - tangent.first;
  if (len < 0) len += n;
  if (len == 0) return poly[tangent.first];
  int lo = 0, hi = len - 2; while (lo <= hi) {
    int mid = lo + (hi - lo) / 2, i = mod(tangent.first + mid, n);
    if (ptSegDist(p, poly[i], poly[mod(i + 1, n)])
        < ptSegDist(p, poly[mod(i + 1, n)], poly[mod(i + 2, n)]))
      hi = mid - 1;
    else lo = mid + 1;
  }
  int i = mod(tangent.first + lo, n);
  return closestPtOnSeg(p, poly[i], poly[mod(i + 1, n)]);
}

// Determines the intersection of a simple polygon and a half-plane defined
//   by the left side of a line (including the line itself)
// Function Arguments:
//   poly: the points of the simple polygon in ccw order
//   l: the line with the half-plane defined by the left side
// Return Value: the polygon defined by the intersection of the simple polygon
//   and the half-plane, assuming the result is a single simple polyon
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/utso15p6
vector<pt> polygonHalfPlaneIntersection(const vector<pt> &poly,
                                        const Line &l) {
  int n = poly.size(); vector<pt> ret; for (int i = 0; i < n; i++) {
    int j = mod(i + 1, n), o1 = l.onLeft(poly[i]), o2 = l.onLeft(poly[j]);
    if (o1 >= 0) ret.push_back(poly[i]);
    if (o1 && o2 && o1 != o2) {
      pt p; if (lineLineIntersection(l, Line(poly[i], poly[j]), p) == 1)
        ret.push_back(p);
    }
  }
  return ret;
}

// Computes the area of union of multiple polygons
// Function Arguments:
//   polys: a vector of the polygons represented by a vector of points given in
//     ccw order
// Return Value: the area of the union of all the polygons
// Time Complexity: O(N^2 log N) for N total points
// Memory Complexity: O(N) for N total points
// Tested:
//   https://open.kattis.com/problems/abstractart
T polygonUnion(const vector<vector<pt>> &polys) {
  auto rat = [&] (pt p, pt q) { return sgn(q.x) ? p.x / q.x : p.y / q.y; };
  T ret = 0; for (int i = 0; i < int(polys.size()); i++)
    for (int v = 0; v < int(polys[i].size()); v++) {
      pt a = polys[i][v], b = polys[i][mod(v + 1, polys[i].size())];
      vector<pair<T, int>> segs{make_pair(0, 0), make_pair(1, 0)};
      for (int j = 0; j < int(polys.size()); j++) if (i != j)
        for (int w = 0; w < int(polys[j].size()); w++) {
          pt c = polys[j][w], d = polys[j][mod(w + 1, polys[j].size())];
          int sc = ccw(a, b, c), sd = ccw(a, b, d); if (sc != sd) {
            T sa = area2(c, d, a), sb = area2(c, d, b); if (lt(min(sc, sd), 0))
              segs.emplace_back(sa / (sa - sb), sgn(sc - sd));
          } else if (!sc && !sd && j < i && sgn(dot(b - a, d - c)) > 0) {
            segs.emplace_back(rat(c - a, b - a), 1);
            segs.emplace_back(rat(d - a, b - a), -1);
          }
        }
      sort(segs.begin(), segs.end()); T sm = 0;
      for (auto &&s : segs) s.first = min(max(s.first, T(0)), T(1));
      for (int j = 1, cnt = segs[0].second; j < int(segs.size()); j++) {
        if (!cnt) sm += segs[j].first - segs[j - 1].first;
        cnt += segs[j].second;
      }
      ret += cross(a, b) * sm;
    }
  return ret / 2;
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
//   https://open.kattis.com/problems/pizzacutting
//   https://open.kattis.com/problems/birthdaycake
T polygonCircleIntersectionArea(const vector<pt> &poly, const Circle &c) {
  T r2 = c.r * c.r / 2;
  auto f = [&] (pt p, pt q) { return atan2(cross(p, q), dot(p, q)); };
  auto tri = [&] (pt p, pt q) {
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

// Computes the area of union of multiple polygons and multiple circles
// Assertion failure likely means there is precision error
// Function Arguments:
//   polys: a vector of the polygons represented by a vector of points given in
//     ccw order
//   circles: a vector of the circles
// Return Value: the area of the union of all the polygons and all the circles
// Time Complexity: O(K (N + M) log (N + M)) for N total points, M circles, and
//                  K unique x coordinates of points of intersections
// Memory Complexity: O((N + M)^2) for N total points and M circles
// Tested:
//   https://open.kattis.com/problems/abstractart
//   https://dmoj.ca/problem/noi05p6
T polygonCircleUnionArea(const vector<vector<pt>> &polys,
                         const vector<Circle> &circles) {
  using Pair = pair<T, pair<int, int>>; vector<T> xs;
  int n = polys.size(), m = circles.size(); for (int i1 = 0; i1 < n; i1++) {
    for (int j1 = 0; j1 < int(polys[i1].size()); j1++) {
      xs.push_back(polys[i1][j1].x);
      pt a = polys[i1][j1], b = polys[i1][mod(j1 + 1, polys[i1].size())];
      for (int i2 = 0; i2 < i1; i2++) {
        for (int j2 = 0; j2 < int(polys[i2].size()); j2++) {
          pt c = polys[i2][j2], d = polys[i2][mod(j2 + 1, polys[i2].size())];
          if (segSegIntersects(a, b, c, d) == 1) {
            vector<pt> inter = segSegIntersection(a, b, c, d);
            assert(int(inter.size()) == 1); xs.push_back(inter[0].x);
          }
        }
      }
      for (int i2 = 0; i2 < m; i2++)
        for (auto &&p : circleSegIntersection(circles[i2], a, b))
          xs.push_back(p.x);
    }
  }
  for (int i1 = 0; i1 < m; i1++) {
    Circle c1 = circles[i1]; xs.push_back(c1.o.x - c1.r);
    xs.push_back(c1.o.x + c1.r); for (int i2 = 0; i2 < i1; i2++) {
      Circle c2 = circles[i2]; vector<pt> inter;
      circleCircleIntersection(c1, c2, inter);
      for (auto &&p : inter) xs.push_back(p.x);
    }
  }
  sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
  T ret = 0; for (int k = 0; k < int(xs.size()) - 1; k++) {
    T l = xs[k], r = xs[k + 1]; if (eq(l, r)) continue;
    T mid = l + (r - l) / 2; Line mLine(pt(mid, 0), pt(mid, 1));
    vector<pair<Pair, Pair>> intervals; for (int i = 0; i < n; i++) {
      vector<pair<T, int>> ys; for (int j = 0; j < int(polys[i].size()); j++) {
        pt a = polys[i][j], b = polys[i][mod(j + 1, polys[i].size())];
        assert(mLine.eval(a) != 0 && mLine.eval(b) != 0);
        if ((mLine.eval(a) < 0) != (mLine.eval(b) < 0)) {
          pt p; assert(lineLineIntersection(Line(a, b), mLine, p) == 1);
          ys.emplace_back(p.y, j);
        }
      }
      assert(int(ys.size()) % 2 == 0); sort(ys.begin(), ys.end());
      for (int j = 0; j < int(ys.size()); j += 2) {
        Pair a = Pair(ys[j].first, make_pair(i, ys[j].second));
        Pair b = Pair(ys[j + 1].first, make_pair(i, ys[j + 1].second));
        intervals.emplace_back(a, b);
      }
    }
    for (int i = 0; i < m; i++) {
      vector<pt> inter = circleLineIntersection(circles[i], mLine);
      if (int(inter.size()) == 2) {
        Pair a = Pair(inter[0].y, make_pair(i, -1));
        Pair b = Pair(inter[1].y, make_pair(i, -2));
        intervals.emplace_back(a, b);
      }
    }
    auto getPts = [&] (const Pair &p) {
      int i = p.second.first, j = p.second.second;
      Line lLine(pt(l, 0), pt(l, 1)), rLine(pt(r, 0), pt(r, 1));
      if (j < 0) {
        Circle c = circles[i];
        vector<pt> left = circleLineIntersection(c, lLine);
        vector<pt> right = circleLineIntersection(c, rLine);
        if (left.empty()) left.push_back(c.o - pt(c.r, 0));
        if (right.empty()) right.push_back(c.o + pt(c.r, 0));
        if (j == -1) return make_pair(left[0], right[0]);
        else return make_pair(left.back(), right.back());
      } else {
        pt a = polys[i][j], b = polys[i][mod(j + 1, polys[i].size())], p1, p2;
        assert(lineLineIntersection(Line(a, b), lLine, p1) == 1);
        assert(lineLineIntersection(Line(a, b), rLine, p2) == 1);
        return make_pair(p1, p2);
      }
    };
    for (auto &&i : intervalUnion(intervals)) {
      pt a, b, c, d; tie(a, b) = getPts(i.first); tie(c, d) = getPts(i.second);
      ret += (r - l) * ((c.y - a.y) + (d.y - b.y)) / 2;
      if (i.first.second.second < 0) {
        Circle circ = circles[i.first.second.first]; assert(a != b);
        ret += circleHalfPlaneIntersectionArea(circ, Line(b, a));
      }
      if (i.second.second.second < 0) {
        Circle circ = circles[i.second.second.first]; assert(c != d);
        ret += circleHalfPlaneIntersectionArea(circ, Line(c, d));
      }
    }
  }
  return ret;
}
