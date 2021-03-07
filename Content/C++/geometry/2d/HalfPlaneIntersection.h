#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
#include "Line.h"
using namespace std;

// Computes the intersection of half-planes defined by the left side of
//   a set of lines
// Angle::pivot is set to (0, 0)
// Function Arguments:
//   lines: a vector of lines representing the half-planes defined by the
//     left side
// Return Value: a vector of points representing the intersection of the
//   half planes, or an empty vector if the intersection is infinite or empty
// In practice, has a moderate constant
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://ncpc20.kattis.com/problems/bigbrother
//   https://maps19.kattis.com/problems/marshlandrescues
//   https://dmoj.ca/problem/ccoprep3p3
vector<pt> halfPlaneIntersection(vector<Line> lines) {
  Angle::setPivot(pt(0, 0));
  sort(lines.begin(), lines.end(), [&] (const Line &a, const Line &b) {
    Angle angA(a.v), angB(b.v);
    return angA == angB ? a.onLeft(b.proj(pt(0, 0))) < 0 : angA < angB;
  });
  int N = lines.size(), front = 0, back = 0; vector<Line> q(N + 1, lines[0]);
  vector<pt> ret(N); pt inter; for (int i = 1; i <= N; i++) {
    if (i == N) lines.push_back(q[front]);
    if (Angle(lines[i - 1].v) == Angle(lines[i].v)) continue;
    while (front < back && lines[i].onLeft(ret[back - 1]) < 0) back--;
    while (i != N && front < back && lines[i].onLeft(ret[front]) < 0) front++;
    if (lineLineIntersection(lines[i], q[back], inter) != 1) continue;
    ret[back++] = inter; q[back] = lines[i];
  }
  if (back - front <= 2) return vector<pt>();
  return vector<pt>(ret.begin() + front, ret.begin() + back);
}
