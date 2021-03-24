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
  vector<pt> hull; sort(P.begin(), P.end()); for (int h = 0; h < 2; h++) {
    int st = hull.size(); for (auto &&p : P) {
      while (int(hull.size()) >= st + 2
          && ccw(hull[hull.size() - 2], hull.back(), p) <= 0)
        hull.pop_back();
      hull.push_back(p);
    }
    hull.pop_back(); reverse(P.begin(), P.end());
  }
  if (int(hull.size()) == 2 && hull[0] == hull[1]) hull.pop_back();
  if (hull.empty() && !P.empty()) hull.push_back(P[0]);
  return hull;
}
