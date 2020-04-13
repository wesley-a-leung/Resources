#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "ConvexHull.h"
using namespace std;

// Computes the farthest pair of points out of a set of N points
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct FarthestPair {
    ConvexHull<MAXN> ch; pt P[MAXN], best1, best2; T bestDistSquared;
    T solve(int N) {
        bestDistSquared = numeric_limits<T>::lowest(); copy(P, P + N, ch.P); ch.run(N); int H = ch.hull.size(); pt o(0, 0);
        for (int i = 0, j = H < 2 ? 0 : 1; i < j; i++) for (;; j = (j + 1) % H) {
            T dSq = distSq(ch.hull[i], ch.hull[j]);
            if (gt(dSq, bestDistSquared)) { bestDistSquared = dSq; best1 = ch.hull[i]; best2 = ch.hull[j]; }
            if (le(ccw(o, ch.hull[i + 1] - ch.hull[i], ch.hull[(j + 1) % H] - ch.hull[j]), 0)) break;
        }
        return bestDistSquared;
    }
};
