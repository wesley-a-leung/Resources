#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
using namespace std;

// Computes the minimum area from 3 distinct points out of a set of N points
// Angle::pivot is modified
// Constructor Arguments:
//   P: the vector of points
// Fields:
//   PA: the first point in the minimum area triangle
//   PB: the second point in the minimum area triangle
//   PC: the third point in the minimum area triangle
//   minArea2: twice the area of the minimum area triangle
//   hull: the points in the convex hull of P 
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N^2 log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz Tested
struct MinTriangleArea {
  pt PA, PB, PC; T minArea2;
  MinTriangleArea(const vector<pt> &P) : minArea2(numeric_limits<T>::max()) {
    int n = P.size(); if (n < 3) return;
    vector<Angle> A(n); for (int i = 0; i < n; i++) A[i] = Angle(P[i]);
    for (int i = 0; i < n; i++) {
      Angle::setPivot(P[i]); sort(A.begin(), A.end());
      for (int j = 0; j < n - 1; j++) {
        pt b = A[j].p, c = A[(j + 1) % (n - 1)].p;
        T a2 = area2(P[i], b, c); if (a2 < 0) { swap(b, c); a2 = -a2; }
        if (a2 < minArea2) { PA = P[i]; PB = b; PC = c; minArea2 = a2; }
      }
    }
  }
};
