#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
#include "ConvexHull.h"
using namespace std;

// Computes the largest area from 3 distinct points out of a set of N points
// Time Complexity: O(N^2)
// Memory Complexity: O(N)
template <const int MAXN> struct LargestTriangularArea {
    ConvexHull<MAXN> ch; pt P[MAXN], PA, PB, PC; T largestArea2;
    T solve(int N) {
        largestArea2 = 0; copy(P, P + N, ch.P); ch.run(N); int H = ch.hull.size();
        if (H < 3) return largestArea2;
        int a = 0, b = 1, c = 2; largestArea2 = area2(PA = ch.hull[a], PB = ch.hull[b], PC = ch.hull[c]);
        while (true) {
            while (c != a) {
                T A = area2(ch.hull[a], ch.hull[b], ch.hull[c]), B;
                while ((B = area2(ch.hull[a], ch.hull[b], ch.hull[(c + 1) % H])) >= A) { c = (c + 1) % H; A = B; }
                if (largestArea2 < A) { largestArea2 = A; PA = ch.hull[a]; PB = ch.hull[b]; PC = ch.hull[c]; }
                b = (b + 1) % H;
            }
            a = (a + 1) % H;
            if (a == 0) break;
            b = (a + 1) % H; c = (b + 1) % H;
        }
        return largestArea2;
    }
};
