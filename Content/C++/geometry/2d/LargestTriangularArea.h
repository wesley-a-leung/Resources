#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "ConvexHull.h"
using namespace std;

// Computes the largest area from 3 distinct points out of a set of N points
// Constructor Arguments:
//   P: the vector of points
// Fields:
//   PA: the first point in the largest triangle
//   PB: the second point in the largest triangle
//   PC: the third point in the largest triangle
//   largestArea2: twice the area of the largest triangle
//   hull: the points in the convex hull of P 
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N^2)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/dmpg18g5
struct LargestTriangularArea {
  pt PA, PB, PC; T largestArea2; vector<pt> hull;
  LargestTriangularArea(const vector<pt> &P)
      : largestArea2(0), hull(convexHull(P)) {
    int H = hull.size(), a = 0, b = 1, c = 2; if (H < 3) return;
    largestArea2 = area2(PA = hull[a], PB = hull[b], PC = hull[c]);
    for (;; b = (a + 1) % H, c = (b + 1) % H) {
      for (; c != a; b = (b + 1) % H) {
        T A = area2(hull[a], hull[b], hull[c]), B = A;
        while ((B = area2(hull[a], hull[b], hull[(c + 1) % H])) >= A) {
          c = (c + 1) % H; A = B;
        }
        if (largestArea2 < A) {
          largestArea2 = A; PA = hull[a]; PB = hull[b]; PC = hull[c];
        }
      }
      if ((a = (a + 1) % H) == 0) break;
    }
  }
};
