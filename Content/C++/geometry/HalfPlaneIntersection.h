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
  deque<Line> dq{lines[0]}; deque<pt> ret;
  int N = lines.size(); for (int i = 1; i <= N; i++) {
    if (i == N) lines.push_back(dq.front());
    if (eq(arg(lines[i - 1].v), arg(lines[i].v))) continue;
    while (!ret.empty() && lines[i].onLeft(ret.back()) < 0) {
      dq.pop_back(); ret.pop_back();
    }
    while (i != N && !ret.empty() && lines[i].onLeft(ret.front()) < 0) {
      dq.pop_front(); ret.pop_front();
    }
    pt inter; if (lineIntersection(lines[i], dq.back(), inter) != 1) continue;
    dq.push_back(lines[i]); ret.push_back(inter);
  }
  if (int(ret.size()) <= 2) return vector<pt>();
  return vector<pt>(ret.begin(), ret.end());
}
