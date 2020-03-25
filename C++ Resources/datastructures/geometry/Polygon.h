#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Line.h"
using namespace std;

using Polygon = vector<Point>;

// furthest point in the direction dir
int extremeVertex(const Polygon &poly, ref dir) {
    int n = int(poly.size()), lo = 0, hi = n; pt pp = perp(dir);
    auto mod = [&] (int i) { return i >= n ? i - n : i; };
    auto cmp = [&] (int i, int j) { return sgn(cross(pp, poly[mod(i)] - poly[mod(j)])); };
    auto extr = [&] (int i) { return cmp(i + 1, i) >= 0 && cmp(i, i + n - 1) < 0; };
    if (extr(0)) return 0;
    while (lo + 1 < hi) {
        int m = lo + (hi - lo) / 2;
        if (extr(m)) return m;
        int ls = cmp(lo + 1, lo), ms = cmp(m + 1, m);
        (ls < ms || (ls == ms && ls == cmp(lo, m)) ? hi : lo) = m;
    }
    return lo;
    return lo;
}
// returns (-1, -1) if no collision
//         (i, -1) if touching corner i
//         (i, i) if along side (i, i + 1)
//         (i, j) if crossing sides (i, i + 1) and (j, j + 1)
//                crossing corner i is treated as crossing side (i, i + 1)
// first index in pair is guaranteed to NOT be on the left side of the line 
pair<int, int> lineConvexPolygonIntersection(const Polygon &poly, const Line &line) {
    int n = int(poly.size()), endA = extremeVertex(poly, -perp(line.v)), endB = extremeVertex(poly, perp(line.v));
    auto mod = [&] (int i) { return i >= n ? i - n : i; };
    auto cmpL = [&] (int i) { return line.onLeft(poly[i]); };
    pair<int, int> ret(-1, -1);
    if (cmpL(endA) > 0 || cmpL(endB) < 0) return ret;
    for (int i = 0; i < 2; i++) {
        int lo = endB, hi = endA;
        while (mod(lo + 1) != hi) { int m = mod((lo + hi + (lo < hi ? 0 : n)) / 2); (cmpL(m) == cmpL(endB) ? lo : hi) = m; }
        (i == 0 ? ret.first : ret.second) = mod(lo + !cmpL(hi)); swap(endA, endB);
    }
    if (ret.first == ret.second) return make_pair(ret.first, -1);
    if (!cmpL(ret.first) && !cmpL(ret.second)) {
        switch ((ret.first - ret.second + n + 1) % n) {
            case 0: return make_pair(ret.first, ret.first);
            case 2: return make_pair(ret.second, ret.second);
        }
    }
    return ret;
}
