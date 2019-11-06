#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
#include "ConvexHull.h"
using namespace std;

// Computes the largest area from 3 distinct points out of a set of N points
// Time Complexity: O(N^2)
// Memory Complexity: O(N)
template <const int MAXN> struct LargestTriangularArea {
    Point P[MAXN], hull[MAXN], PA, PB, PC; F largestArea;
    F solve(int N) {
        largestArea = 0;
        if (N < 3) { PA = P[0]; PB = P[1 % N]; PC = P[2 % N]; return largestArea; }
        ConvexHull<MAXN> H; copy(P, P + N, H.P); H.run(N); int M = 0;
        for (auto &&p : H.hull) hull[M++] = p;
        if (M < 3) { PA = hull[0]; PB = hull[1 % N]; PC = hull[2 % N]; return largestArea; }
        int a = 0, b = 1, c = 2; largestArea = Point::area2(PA = hull[a], PB = hull[b], PC = hull[c]);
        while (true) {
            while (c != a) {
                T A = Point::area2(hull[a], hull[b], hull[c]), B;
                while ((B = Point::area2(hull[a], hull[b], hull[(c + 1) % M])) >= A) { c = (c + 1) % M; A = B; }
                if (largestArea < A) { largestArea = A; PA = hull[a]; PB = hull[b]; PC = hull[c]; }
                b = (b + 1) % M;
            }
            a = (a + 1) % M;
            if (a == 0) break;
            b = (a + 1) % M; c = (b + 1) % M;
        }
        return largestArea /= 2.0;
    }
};
