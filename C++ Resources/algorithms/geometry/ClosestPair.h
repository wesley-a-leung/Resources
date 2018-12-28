#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
using namespace std;

// Computes the closest pair of points
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct ClosestPair {
    Point P[MAXN], pointsByY[MAXN], aux[MAXN], best1, best2; double bestDist;
    void merge(int lo, int mid, int hi) {
        for (int k = lo; k <= hi; k++) aux[k] = pointsByY[k];
        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid) pointsByY[k] = aux[j++];
            else if (j > hi) pointsByY[k] = aux[i++];
            else if (aux[j].compareTo(aux[i]) < 0) pointsByY[k] = aux[j++];
            else pointsByY[k] = aux[i++];
        }
    }
    double closest(int lo, int hi) {
        if (hi <= lo) return numeric_limits<double>::infinity();
        int mid = lo + (hi - lo) / 2; Point median = P[mid];
        double delta = min(closest(lo, mid), closest(mid + 1, hi)); merge(lo, mid, hi); int m = 0;
        for (int i = lo; i <= hi; i++) if (abs(pointsByY[i].x - median.x) < delta) aux[m++] = pointsByY[i];
        for (int i = 0; i < m; i++) {
            for (int j = i + 1; (j < m) && (aux[j].y - aux[i].y < delta); j++) {
                double distance = aux[i].distanceTo(aux[j]);
                if (distance < delta) {
                    delta = distance;
                    if (distance < bestDist) { bestDist = delta; best1 = aux[i]; best2 = aux[j]; }
                }
            }
        }
        return delta;
    }
    double solve(int N) {
        if (N <= 1) return bestDist;
        sort(P, P + N, Point::xOrderLt);
        for (int i = 0; i < N - 1; i++) if (P[i] == P[i + 1]) { bestDist = 0; best1 = P[i]; best2 = P[i + 1]; return bestDist; }
        for (int i = 0; i < N; i++) pointsByY[i] = P[i];
        closest(0, N - 1); return bestDist;
    }
};
