#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/geometry/Point.h"
using namespace std;

// Computes the convex hull of a set of N points
// Time Complexity:
//   run: O(N log N)
//   getArea2: O(N)
// Memory Complexity: O(N)
template <const int MAXN> struct ConvexHull {
    Point P[MAXN]; vector<Point> hull; // counterclockwise order
    void run(int N) {
        hull.clear(); sort(P, P + N, Point::xyOrderLt);
        for (int phase = 0; phase < 2; phase++) {
            for (int i = 0, st = int(hull.size()); i < N; i++) {
                while (int(hull.size()) >= st + 2 && Point::ccw(hull[hull.size() - 2], hull[hull.size() - 1], P[i]) <= 0) hull.pop_back();
                hull.push_back(P[i]);
            }
            hull.pop_back(); reverse(P, P + N);
        }
        if (int(hull.size()) == 2 && hull[0] == hull[1]) hull.pop_back();
        if (hull.empty() && N > 0) hull.push_back(P[0]);
    }
    T getArea2() { // returns twice the area of the convex hull
        T ret = 0; int H = int(hull.size());
        for (int i = 0; i < H; i++) ret += hull[i].x * hull[(i + 1) % H].y - hull[i].y * hull[(i + 1) % H].x;
        return ret;
    }
};
