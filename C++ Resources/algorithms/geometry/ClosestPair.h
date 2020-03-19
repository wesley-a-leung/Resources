#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
using namespace std;

// Computes the closest pair of points out of a set of N points
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct ClosestPair {
    pt P[MAXN], aux[MAXN], best1, best2; T bestDistSquared;
    void closest(int lo, int hi) {
        if (hi <= lo) return;
        int mid = lo + (hi - lo) / 2; pt median = P[mid]; closest(lo, mid); closest(mid + 1, hi);
        merge(P + lo, P + mid + 1, P + mid + 1, P + hi + 1, aux, yOrdLt); copy(aux, aux + hi - lo + 1, P + lo);
        for (int i = lo, k = 0; i <= hi; i++) {
            T dx = P[i].x - median.x, dx2 = dx * dx;
            if (lt(dx2, bestDistSquared)) {
                for (int j = k - 1; j >= 0; j--) {
                    T dy = P[i].y - aux[j].y, dy2 = dy * dy;
                    if (!lt(dy2, bestDistSquared)) break;
                    T dSq = distSq(P[i], aux[j]);
                    if (lt(dSq, bestDistSquared)) { bestDistSquared = dSq; best1 = P[i]; best2 = aux[j]; }
                }
                aux[k++] = P[i];
            }
        }
    }
    T solve(int N) {
        bestDistSquared = numeric_limits<T>::max(); sort(P, P + N, xOrdLt); closest(0, N - 1); return bestDistSquared;
    }
};
