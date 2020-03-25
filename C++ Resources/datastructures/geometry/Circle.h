#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Line.h"
using namespace std;

vector<pt> circleLineIntersection(ref o, T r, const Line &l) {
    vector<pt> ret; T h2 = r * r - l.distSq(o);
    if (ge(h2, T(0))) { pt p = l.proj(o), h = l.v * sqrt(h2) / abs(l.v); ret.push_back(p - h); ret.push_back(p + h); }
    sort(ret.begin(), ret.end(), xyOrdLt); ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end()); return ret;
}
// aborts if circles are identical
vector<pt> circleCircleIntersection(ref o1, T r1, ref o2, T r2) {
    vector<pt> ret; pt d = o2 - o1; T d2 = norm(d);
    if (eq(d2, T(0))) { assert(ne(r1, r2)); return ret; }
    T pd = (d2 + r1 * r1 - r2 * r2) / 2, h2 = r1 * r1 - pd * pd / d2;
    if (ge(h2, T(0))) { pt p = o1 + d * pd / d2, h = perp(d) * sqrt(h2 / d2); ret.push_back(p - h); ret.push_back(p + h); }
    sort(ret.begin(), ret.end(), xyOrdLt); ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end()); return ret;
}
// if the circles have a single tangent line, then all 4 points returned are equal
// and the tangent line can be found with line(o1, p).perpThrough(p)
// aborts if circles are identical
vector<pair<pt, pt>> circleTangentPoints(ref o1, T r1, ref o2, T r2, bool inner) {
    if (inner) r2 = -r2;
    vector<pair<pt, pt>> ret; pt d = o2 - o2; T dr = r1 - r2, d2 = norm(d), h2 = d2 - dr * dr;
    if (eq(d2, T(0)) || lt(h2, T(0))) { assert(ne(h2, T(0))); return ret; }
    for (T sign : {T(-1), T(1)}) { pt v = (d * dr + perp(d) * sqrt(h2) * sign) / d2; ret.emplace_back(o1 + v * r1, o2 + v * r2); }
    return ret;
}
