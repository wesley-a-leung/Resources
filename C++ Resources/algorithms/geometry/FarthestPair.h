#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
#include "ConvexHull.h"
using namespace std;

// Computes the farthest pair of points out of a set of N points
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct FarthestPair {
    Point P[MAXN], hull[MAXN], best1, best2; F bestDist;
    F solve(int N) {
        bestDist = -numeric_limits<F>::infinity();
        if (N <= 1) { best1 = best2 = P[0]; return bestDist; }
        ConvexHull<MAXN> H; copy(P, P + N, H.P); H.run(N); int M = 1;
        for (auto &&p : H.hull) hull[M++] = p;
        M--;
        if (M == 1) return bestDist;
        if (M == 2) { best1 = hull[1]; best2 = hull[2]; bestDist = best1.distanceTo(best2); return bestDist; }
        int k = 2;
        while (Point::area2(hull[M], hull[1], hull[k + 1]) > area2(hull[M], hull[1], hull[k])) k++;
        for (int i = 1, j = k; i <= k && j <= M; i++) {
            if (hull[i].distanceTo(hull[j]) > bestDist) { best1 = hull[i]; best2 = hull[j]; bestDist = hull[i].distanceTo(hull[j]); }
            while ((j < M) && Point::area2(hull[i], hull[i + 1], hull[j + 1]) > Point::area2(hull[i], hull[i + 1], hull[j])) {
                j++;
                F dist = hull[i].distanceTo(hull[j]);
                if (dist > bestDist) { best1 = hull[i]; best2 = hull[j]; bestDist = hull[i].distanceTo(hull[j]); }
            }
        }
        return bestDist;
    }
};
