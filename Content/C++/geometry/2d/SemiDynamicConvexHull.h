#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
#include "Line.h"
#include "Circle.h"
using namespace std;

// A ray represented by a point and line with a direction
// Fields:
//   p: the point
//   l: a line passing through p, representing the direction
struct Ray {
  pt p; Line l; Ray(pt p, Line l = Line()) : p(p), l(l) {}
  virtual bool cmp(const Ray &l) const {
    Angle::setPivot(pt(0, 0)); return Angle(l.l.v) < Angle(this->l.v);
  }
  bool operator < (const Ray &r) const { return r.cmp(*this); }
};

// Helper struct for isIn
struct IsInCmp : public Ray {
  pt q, r; IsInCmp(pt p, pt q, pt r) : Ray(p), q(q), r(r) {}
  virtual bool cmp(const Ray &l) const {
    return q != l.p && ccw(p, l.p + l.l.v, r) > 0;
  }
};

// Helper struct for pointTangents
struct PointTangentCmp : public Ray {
  pt q, r; bool left, farSide;
  PointTangentCmp(pt p, pt q, pt r, bool left, bool farSide)
      : Ray(p), q(q), r(r), left(left), farSide(farSide) {}
  virtual bool cmp(const Ray &l) const {
    if (farSide != left && l.p == p) return true;
    if (farSide == left && l.p == q) return false;
    if (ccw(r, p, l.p) == (left ? -1 : 1)) return farSide != left;
    else return (ccw(l.p, l.p + l.l.v, r) < 0) != left;
  }
};

// Helper struct for circleTangents
struct CircleTangentCmp : public Ray {
  pt q; Circle c; bool inner, h, farSide;
  CircleTangentCmp(pt p, pt q, Circle c, bool inner, bool h, bool farSide)
      : Ray(p), q(q), c(c), inner(inner), h(h), farSide(farSide) {}
  virtual bool cmp(const Ray &l) const {
    if (farSide == h && l.p == p) return true;
    if (farSide != h && l.p == q) return false;
    vector<pair<pt, pt>> t;
    assert(circleCircleTangent(Circle(l.p, 0), c, inner, t) == 1);
    pt q = t[h].second;
    if (ccw(q, p, l.p) == (h ? 1 : -1)) return farSide == h;
    else return (ccw(l.p, l.p + l.l.v, q) < 0) == h;
  }
};

// Maintains the convex hull of points were points can be added at any time
// Points are stored as a circular ordered set of Rays, with an associated
//   Line in the direction towards the next point
// Points are strictly convex
// Fields:
//   a2: twice the area of the convex hull
// Functions:
//   clear(): clears the points in the convex hull
//   isIn(p): returns -1 if inside the polygon, 0 if on the edge, 1 if outside
//   singlePointTangent, (p, left): returns an iterator pointing to the left or
//     right tangent, with the closest point to p being selected if there are
//     multiple points
//   pointTangents(p): returns a pair of iterators, with the first iterator
//     being the left tangent point and the second iterator being the right
//     tangent if p is considered to be below the polygon; all points strictly
//     between the tangent iterator are strictly within the tangent lines,
//     while all other points are on or outside the tangent lines
//   circleTangents(c, inner): returns a pair of iterators, with the first
//     iterator being the left inner or outer tangent point and the second
//     iterator being the right tangent if c is considered to be below the
//     polygon; all points strictly between the tangent iterator are strictly
//     within the tangent lines, while all other points are on or outside the
//     tangent lines (same iterator means all points are inside or on the
//     tangent lines)
//   hullTangents(hull, inner): returns a vector of pair of iterators, with the
//     first element in each pair being the iterator in the first polygon,
//     and the second element being the iteraetor in the second polygon; first
//     point in each pair is the left inner/outer tangent point of the first
//     polygon if the second polygon is considered to be below the
//     first polygon
//   addPoint(p): adds the point p and removes any points that are no longer
//     in the convex hull; returns true if p is in the hull, false otherwise;
//     Angle::pivot is set to (0, 0)
//   extremeVertex(dir): returns an iterator pointing to an extreme vertex
//     in the direction dir (a vertex that is the furthest point in that
//     direction, selecting the rightmost vertex if there are multiple);
//     Angle::pivot is set to (0, 0)
//   halfPlaneIntersection(l): intersects this convex hull with the half-plane
//     specified by the left side of l (including l itself); resulting hull
//     is strictly convex; Angle::pivot is set to (0, 0)
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1)
//   clear: O(N)
//   isIn, singlePointTangent, pointTangents, circleTangents: O(log N)
//   hullTangents: O(log N log M)
//   addPoint: O(log N + K) for K removed points
//   extremeVertex: O(log N)
//   halfPlaneIntersection: O(log N + K) for K removed points
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/problemsets/acmsguru/problem/99999/277
//   https://open.kattis.com/problems/bigbrother
//   https://open.kattis.com/problems/marshlandrescues
//   https://dmoj.ca/problem/ccoprep3p3
//   https://www.acmicpc.net/problem/4225
struct SemiDynamicConvexHull : public set<Ray> {
  using SS = set<Ray>; using iter = SS::iterator;
  T a2; SemiDynamicConvexHull() : a2(0) {}
  iter mod(iter it) const { return !empty() && it == end() ? begin() : it; }
  iter prv(iter it) const { return prev(it == begin() ? end() : it); }
  iter nxt(iter it) const { return mod(next(it)); }
  iter rem(iter it) {
    iter a = prv(it), b = nxt(it); a2 += cross(a->p, b->p);
    a2 -= cross(a->p, it->p) + cross(it->p, b->p); erase(it); return b;
  }
  void clear() { SS::clear(); a2 = 0; }
  int isIn(pt p) const {
    if (empty()) return 1;
    pt a = begin()->p, b = prev(end())->p; if (onSeg(p, a, b)) return 0;
    auto it = lower_bound(IsInCmp(a, b, p)); pt q = nxt(it)->p;
    if (onSeg(p, it->p, q)) return 0;
    vector<pt> P{a, it->p, q}; sort(P.begin(), P.end());
    P.erase(unique(P.begin(), P.end()), P.end());
    if (int(P.size()) < 3) return onSeg(p, P[0], P.back()) ? 0 : 1;
    return ccw(it->p, q, p) >= 0 ? -1 : 1;
  }
  iter singlePointTangent(pt p, bool left) const {
    pt a = begin()->p, b = prev(end())->p; int o = ccw(p, a, b);
    bool farSide = o ? o < 0 : lt(distSq(p, b), distSq(p, a));
    return mod(lower_bound(PointTangentCmp(a, b, p, left, farSide)));
  }
  pair<iter, iter> pointTangents(pt p) const {
    return make_pair(singlePointTangent(p, 1), singlePointTangent(p, 0));
  }
  pair<iter, iter> circleTangents(Circle c, bool inner) const {
    pair<iter, iter> ret; pt a = begin()->p, b = prev(end())->p;
    for (int h = 0; h < 2; h++) {
      vector<pair<pt, pt>> t;
      assert(circleCircleTangent(Circle(a, 0), c, inner, t) == 1);
      pt q = t[h].second; int o = ccw(q, a, b);
      bool farSide = o ? o < 0 : lt(distSq(q, b), distSq(q, a));
      (h ? ret.second : ret.first) = mod(lower_bound(CircleTangentCmp(
          a, b, c, inner, h, farSide)));
    }
    return ret;
  }
  vector<pair<iter, iter>> hullTangents(const SemiDynamicConvexHull &hull,
                                        bool inner) const;
  bool addPoint(pt p) {
    if (empty()) { emplace(p, Line(p, p)); return true; }
    if (isIn(p) <= 0) return false;
    iter l, r; tie(l, r) = pointTangents(p); l = prv(l);
    if (ccw(l->p, nxt(l)->p, p) > 0) l = nxt(l);
    if (ccw(p, r->p, nxt(r)->p) == 0) r = nxt(r);
    for (l = nxt(l); l != r; l = rem(l));
    l = prv(l); pt a = l->p, b = r->p;
    a2 += cross(a, p) + cross(p, b) - cross(a, b);
    erase(l); emplace(a, Line(a, p)); emplace(p, Line(p, b)); return true;
  }
  iter extremeVertex(pt dir) const {
    return mod(lower_bound(Ray(pt(0, 0), Line(perp(dir), 0))));
  }
  void halfPlaneIntersection(Line l) {
    if (empty()) return;
    iter b = mod(lower_bound(Ray(pt(0, 0), l)));
    if (l.onLeft(b->p) >= 0) return;
    iter a = prv(b); while (a != b && l.onLeft(a->p) < 0) a = prv(a);
    if (a == b) { clear(); return; }
    iter c = nxt(a); while (c != b) c = rem(c);
    c = nxt(b); while (l.onLeft(c->p) < 0) c = nxt(b = rem(b));
    pt p, q, r = a->p, s = c->p; Line la = a->l, lb = b->l;
    if (lineLineIntersection(la, l, p) != 1
        || lineLineIntersection(l, lb, q) != 1) {
      assert(lineLineIntersection(la, lb, p) == 1); q = p;
    }
    a2 -= cross(r, b->p) + cross(b->p, s); erase(a); erase(b);
    a2 += cross(r, p) + cross(p, q) + cross(q, s);
    if (r != p) emplace(r, la);
    if (q != s) emplace(q, lb);
    if ((r == p && q == s) || p != q) emplace(p, l);
  }
};

// Helper struct for hullTangents
struct HullTangentCmp : public Ray {
  pt q; const SemiDynamicConvexHull &hull; bool inner, h, farSide;
  HullTangentCmp(pt p, pt q, const SemiDynamicConvexHull &hull,
                 bool inner, bool h, bool farSide)
      : Ray(p), q(q), hull(hull), inner(inner), h(h), farSide(farSide) {}
  virtual bool cmp(const Ray &l) const {
    if (farSide == h && l.p == p) return true;
    if (farSide != h && l.p == q) return false;
    pt q = hull.singlePointTangent(l.p, inner ^ h)->p;
    if (ccw(q, p, l.p) == (h ? 1 : -1)) return farSide == h;
    else return (ccw(l.p, l.p + l.l.v, q) < 0) == h;
  }
};

vector<pair<SemiDynamicConvexHull::iter, SemiDynamicConvexHull::iter>>
    SemiDynamicConvexHull::hullTangents(const SemiDynamicConvexHull &hull,
                                        bool inner) const {
  vector<pair<iter, iter>> ret(2); pt a = begin()->p, b = prev(end())->p;
  for (int h = 0; h < 2; h++) {
    pt q = hull.singlePointTangent(a, inner ^ h)->p; int o = ccw(q, a, b);
    bool farSide = o ? o < 0 : lt(distSq(q, b), distSq(q, a));
    ret[h].first = mod(lower_bound(HullTangentCmp(
          a, b, hull, inner, h, farSide)));
    ret[h].second = hull.singlePointTangent(ret[h].first->p, inner ^ h);
  }
  return ret;
}
