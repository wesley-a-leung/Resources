#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
#include "Line.h"
using namespace std;

// Helper struct for SemiDynamicHalfPlaneIntersection
struct LineCmp {
  bool operator () (const Line &a, const Line &b) const {
    Angle::setPivot(pt(0, 0)); return Angle(a.v) < Angle(b.v);
  }
};

// Computes the intersection (and area) of half-planes defined by the left side
//   of a set of lines (including the line itself) where half-planes are
//   added at any time
// An initial bounded rectangle is provided to ensure the intersection area
//   is never infinity
// Lines are stores in a map, with the associated point being the intersection
//   with the previous line in the map (assuming the map is circular)
// Points on the intersection may be identical
// Angle::pivot is set to (0, 0) after every operation
// Constructor Arguments:
//   lowerLeft: the lower left corner of the initial bounding rectangle
//   upperRight: the upper right corner of the initial bounding rectangle
// Fields:
//   a2: twice the area of the intersection of the half-planes
// Functions:
//   addHalfPlane(l): adds the half-plane defined by the left side of l
//   empty(): returns whether the intersection is empty or not
// In practice, has a moderate constant, can be faster than
//   HalfPlaneIntersection, but appears to have more precision issues
// Time Complexity:
//   constructor, empty: O(1)
//   addHalfPlane: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://open.kattis.com/problems/bigbrother
//   https://open.kattis.com/problems/marshlandrescues
//   https://dmoj.ca/problem/ccoprep3p3
struct SemiDynamicHalfPlaneIntersection : public map<Line, pt, LineCmp> {
  using iter = map<Line, pt, LineCmp>::iterator; T a2;
  SemiDynamicHalfPlaneIntersection(pt lowerLeft, pt upperRight)
      : a2((upperRight.x - lowerLeft.x) * (upperRight.y - lowerLeft.y) * 2) {
    assert(lt(lowerLeft.x, upperRight.x) && lt(lowerLeft.y, upperRight.y));
    emplace(Line(pt(1, 0), lowerLeft.y), lowerLeft);
    emplace(Line(pt(0, 1), -upperRight.x), pt(upperRight.x, lowerLeft.y));
    emplace(Line(pt(-1, 0), -upperRight.y), upperRight);
    emplace(Line(pt(0, -1), lowerLeft.x), pt(lowerLeft.x, upperRight.y));
  }
  iter prv(iter it) { return prev(it == begin() ? end() : it); }
  iter nxt(iter it) { ++it; return it == end() ? begin() : it; }
  iter rem(iter it) {
    iter a = prv(it), b = nxt(it); a2 += cross(a->second, b->second);
    a2 -= cross(a->second, it->second) + cross(it->second, b->second);
    erase(it); return b;
  }
  void addHalfPlane(const Line &l) {
    if (empty()) return;
    iter b = lower_bound(l); if (b == end()) b = begin();
    if (l.onLeft(b->second) >= 0) return;
    iter a = prv(b); while (a != b && l.onLeft(a->second) < 0) a = prv(a);
    if (a == b) { clear(); a2 = 0; return; }
    iter c = nxt(a); while (c != b) c = rem(c); 
    c = nxt(b); while (l.onLeft(c->second) < 0) c = nxt(b = rem(b));
    pt p, q; bool add = 1; if (lineLineIntersection(a->first, l, p) != 1
        || lineLineIntersection(l, b->first, q) != 1) {
      add = 0; assert(lineLineIntersection(a->first, b->first, q) == 1);
    }
    a2 -= cross(a->second, b->second) + cross(b->second, c->second);
    a2 += cross(a->second, q) + cross(q, c->second); b->second = q; if (add) {
      a2 -= cross(a->second, q);
      a2 += cross(a->second, p) + cross(p, q); emplace_hint(b, l, p);
    }
  }
};
