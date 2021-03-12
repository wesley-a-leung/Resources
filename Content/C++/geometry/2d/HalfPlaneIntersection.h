#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
#include "Line.h"
using namespace std;

// Computes the intersection of half-planes defined by the left side of
//   a set of lines (including the line itself)
// Points on the intersection are given in ccw order and may be identical
// Angle::pivot is set to (0, 0)
// Function Arguments:
//   lines: a vector of lines representing the half-planes defined by the
//     left side
// Return Value: a pair containing a return code and a vector of points with
//     the return codes defined as follows:
//   code == 0: the intersection is finite (possibly empty),
//     with the vector containing the points on the intersection;
//   code == 1: the intersection is infinite and bounded by a series of
//     line segments and rays (possibly a single ray), with the
//     vector P[0], P[1], ..., P[k] where the first ray is represented by
//     the point P[1] in the direction towards P[0], and the second ray is
//     represented by the point P[k - 1] in the direction towards P[k];
//     the remaining points P[1], ..., P[k - 1] represent the intersections
//     between the line segments and rays
//   code == 2: the intersection if infinite and bounded by two lines
//     (possibly coincident lines in opposite directions), with the vector
//     containing exactly 4 points (P[0], P[1], P[2], P[3]), where P[0] is a
//     point on the first bounding line, P[1] is another point on the first
//     line in the positive direction (based on the direction vector of the
//     line) from P[0], P[2] is a point on the second line, and P[3] is another
//     point on the second line in its positive direction from P[2]
//   code == 3: the intersection is the entire plane, with an empty vector
// In practice, has a moderate constant, can be slower than
//   DynamicHalfPlaneIntersection, but appears to have less precision issues
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://open.kattis.com/problems/bigbrother
//   https://maps19.kattis.com/problems/marshlandrescues
//   https://dmoj.ca/problem/ccoprep3p3
pair<int, vector<pt>> halfPlaneIntersection(vector<Line> lines) {
  Angle::setPivot(pt(0, 0));
  sort(lines.begin(), lines.end(), [&] (const Line &a, const Line &b) {
    Angle angA(a.v), angB(b.v);
    return angA == angB ? a.onLeft(b.proj(pt(0, 0))) < 0 : angA < angB;
  });
  lines.erase(unique(lines.begin(), lines.end(), [&] (const Line &a,
                                                      const Line &b) {
    return Angle(a.v) == Angle(b.v); 
  }), lines.end());
  int N = lines.size(); if (N == 0) return make_pair(3, vector<pt>());
  if (N == 1) {
    pt p = lines[0].proj(pt(0, 0));
    return make_pair(1, vector<pt>{p, p + lines[0].v});
  }
  int code = 0; for (int i = 0; code == 0 && i < N; i++) {
    Angle diff = Angle(lines[i].v) - Angle(lines[i == 0 ? N - 1 : i - 1].v);
    if (diff < Angle(pt(1, 0))) {
      rotate(lines.begin(), lines.begin() + i, lines.end()); code = 1;
      if (N == 2 && diff == Angle(pt(-1, 0))) {
        pt p = lines[0].proj(pt(0, 0));
        if (lines[1].onLeft(p) < 0) return make_pair(0, vector<pt>());
        pt q = lines[1].proj(pt(0, 0));
        return make_pair(2, vector<pt>{p, p + lines[0].v, q, q + lines[1].v});
      }
    }
  }
  vector<Line> q(N + 1, lines[0]); vector<pt> ret(N); pt inter;
  int front = 0, back = 0; for (int i = 1; i <= N - code; i++) {
    if (i == N) lines.push_back(q[front]);
    while (front < back && lines[i].onLeft(ret[back - 1]) < 0) back--;
    while (i != N && front < back && lines[i].onLeft(ret[front]) < 0) front++;
    if (lineLineIntersection(lines[i], q[back], inter) != 1) continue;
    ret[back++] = inter; q[back] = lines[i];
  }
  if (code == 0 && back - front < 3) return make_pair(code, vector<pt>());
  vector<pt> P(ret.begin() + front, ret.begin() + back); if (code == 1) {
    P.insert(P.begin(), P[0] - q[front].v); P.push_back(P.back() + q[back].v);
  }
  return make_pair(code, P);
}
