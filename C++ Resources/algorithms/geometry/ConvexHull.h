#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
using namespace std;
using namespace std::placeholders;

// Computes the convex hull of points
// Time Complexity:
//   run: O(N log N)
//   getArea2: O(N)
// Memory Complexity: O(N)
template <const int MAXN> struct ConvexHull {
    Point P[MAXN]; vector<Point> hull;
    void clear() { hull.clear(); }
    void run(int N) {
        sort(P, P + N, [&] (const Point &p, const Point &q) { return p.y == q.y ? p.x < q.x : p.y < q.y; });
        if (N > 1) stable_sort(P + 1, P + N, bind(&Point::polarOrderLt, P[0], _1, _2));
        hull.push_back(P[0]);
        int k1, k2;
        for (k1 = 1; k1 < N; k1++) if (P[0] != P[k1]) break;
        if (k1 == N) return;
        for (k2 = k1 + 1; k2 < N; k2++) if (Point::ccw(P[0], P[k1], P[k2]) != 0) break;
        hull.push_back(P[k2 - 1]);
        for (int i = k2; i < N; i++) {
            while (hull.size() >= 2 && Point::ccw(hull[hull.size() - 2], hull[hull.size() - 1], P[i]) <= 0)
                hull.pop_back();
            hull.push_back(P[i]);
        }
    }
    template <class T> T getArea2() { // returns twice the area of the convex hull
        T ret = 0;
        int H = int(hull.size());
        for (int i = 0; i < H; i++) ret += hull[i].x * hull[(i + 1) % H].y - hull[i].y * hull[(i + 1) % H].x;
        return ret;
    }
};
