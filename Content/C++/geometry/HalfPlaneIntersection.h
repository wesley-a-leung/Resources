#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point.h"
#include "Line.h"
using namespace std;

// Computes the intersection of half-planes defined by the left side of
//   a set of lines
// Function Arguments:
//   lines: a vector of lines representing the half-planes defined by the
//     left side
// Return Value: a vector of points representing the intersection of the
//   half planes
// In practice, has a moderate constant
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://open.kattis.com/problems/bigbrother
vector<pt> halfPlaneIntersection(vector<Line> lines) {
  sort(lines.begin(), lines.end(), [&] (const Line &a, const Line &b) {
    int s = sgn(arg(a.v) - arg(b.v));
    return (s == 0 ? a.onLeft(b.proj(pt(0, 0))) : s) < 0;
  });
  int N = lines.size(), front = 0, back = 0; vector<Line> dq(N + 1, lines[0]);
  vector<pt> ret(N); for (int i = 1; i <= N; i++) {
    if (i == N) lines.push_back(dq[front]);
    if (eq(arg(lines[i - 1].v), arg(lines[i].v))) continue;
    while (front < back && lines[i].onLeft(ret[back - 1]) < 0) back--;
    while (i != N && front < back && lines[i].onLeft(ret[front]) < 0) front++;
    pt inter; if (lineIntersection(lines[i], dq[back], inter) != 1) continue;
    ret[back++] = inter; dq[back] = lines[i];
  }
  if (back - front <= 2) return vector<pt>();
  return vector<pt>(ret.begin() + front, ret.begin() + back);
}
