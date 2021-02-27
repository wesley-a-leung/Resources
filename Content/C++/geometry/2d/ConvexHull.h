#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Computes the convex hull of a set of N points
// Function Arguments:
//   P: the vector of points
// Return Value: a vector of points in the convex hull in ccw order
// In practice, has a small constant
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://open.kattis.com/problems/convexhull
vector<pt> convexHull(vector<pt> P) {
  vector<pt> hull; sort(P.begin(), P.end(), pt_lt());
  for (int phase = 0; phase < 2; phase++) {
    for (int i = 0, st = int(hull.size()); i < int(P.size()); i++) {
      while (int(hull.size()) >= st + 2
          && ccw(hull[hull.size() - 2], hull[hull.size() - 1], P[i]) <= 0)
        hull.pop_back();
      hull.push_back(P[i]);
    }
    hull.pop_back(); reverse(P.begin(), P.end());
  }
  if (int(hull.size()) == 2 && pt_eq()(hull[0], hull[1])) hull.pop_back();
  if (hull.empty() && !P.empty()) hull.push_back(P[0]);
  return hull;
}
