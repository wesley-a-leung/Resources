#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point.h"
#include "ConvexHull.h"
using namespace std;

// Computes the farthest pair of points out of a set of N points
// Constructor Arguments:
//   P: the vector of points
// Fields:
//   best1: the first point in the pair of farthest point
//   best2: the second point in the pair of farthest point
//   bestDistSq: the distance squared between best1 and best2 
//   hull: the points in the convex hull of P 
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz Tested
struct FarthestPair {
  pt best1, best2; T bestDistSq; vector<pt> hull;
  FarthestPair(const vector<pt> &P) : bestDistSq(0), hull(convexHull(P)) {
    int H = hull.size(); pt o(0, 0);
    for (int i = 0, j = H < 2 ? 0 : 1; i < j; i++) for (;; j = (j + 1) % H) {
      T dSq = distSq(hull[i], hull[j]); if (lt(bestDistSq, dSq)) {
        bestDistSq = dSq; best1 = hull[i]; best2 = hull[j];
      }
      pt a = hull[i + 1] - hull[i], b = hull[(j + 1) % H] - hull[j];
      if (ccw(o, a, b) <= 0) break;
    }
  }
};
