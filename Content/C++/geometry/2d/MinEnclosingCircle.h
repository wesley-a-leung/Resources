#pragma once
#include <bits/stdc++.h>
#include "../../utils/Random.h"
#include "Point.h"
#include "Circle.h"
using namespace std;

// Determines the minimum enclosing circle of a set of points
// Function Arguments:
//   P: the points
// Return Value: the minimum enclosing circle of P
// In practice, has a large constant
// Time Complexity: O(N) expected
// Memory Complexity: O(N)
// Tested:
//   https://www.spoj.com/problems/QCJ4/
//   https://open.kattis.com/problems/starsinacan
Circle minEnclosingCircle(vector<pt> P) {
  shuffle(P.begin(), P.end(), rng); Circle c(P[0], 0);
  for (int i = 0; i < int(P.size()); i++) if (lt(c.r, dist(P[i], c.o))) {
    c = Circle(P[i], 0);
    for (int j = 0; j < i; j++) if (lt(c.r, dist(P[j], c.o))) {
      pt p = (P[i] + P[j]) / T(2); c = Circle(p, dist(P[i], p));
      for (int k = 0; k < j; k++) if (lt(c.r, dist(P[k], c.o)))
        c = circumcircle(P[i], P[j], P[k]); 
    }
  }
  return c;
}
