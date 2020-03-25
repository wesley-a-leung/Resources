#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Line.h"
using namespace std;

struct Circle {
    pt o; T r;
    Circle(T r = 0) : r(r) {}
    Circle(ref o, T r) : o(o), r(r) {}
};
vector<pt> circleLineIntersection(const Circle &c, const Line &l) {
    vector<pt> ret; T h2 = c.r * c.r - l.distSq(c.o);
    if (ge(h2, 0)) { pt p = l.proj(c.o), h = l.v * sqrt(h2) / abs(l.v); ret.push_back(p - h); ret.push_back(p + h); }
    sort(ret.begin(), ret.end(), xyOrdLt); ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end()); return ret;
}
// aborts if circles are identical
vector<pt> circleCircleIntersection(const Circle &c1, const Circle &c2) {
    vector<pt> ret; pt d = c2.o - c1.o; T d2 = norm(d);
    if (eq(d2, 0)) { assert(ne(c1.r, c2.r)); return ret; }
    T pd = (d2 + c1.r * c1.r - c2.r * c2.r) / 2, h2 = c1.r * c1.r - pd * pd / d2;
    if (ge(h2, 0)) { pt p = c1.o + d * pd / d2, h = perp(d) * sqrt(h2 / d2); ret.push_back(p - h); ret.push_back(p + h); }
    sort(ret.begin(), ret.end(), xyOrdLt); ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end()); return ret;
}
// if the circles have a single tangent line, then all 4 points returned are equal
// and the tangent line can be found with line(o1, p).perpThrough(p)
// aborts if circles are identical
vector<pair<pt, pt>> circleTangentPoints(const Circle &c1, const Circle &c2, bool inner) {
    vector<pair<pt, pt>> ret; pt d = c2.o - c1.o; T r2 = inner ? -c2.r : c2.r, dr = c1.r - r2, d2 = norm(d), h2 = d2 - dr * dr;
    if (eq(d2, 0) || lt(h2, 0)) { assert(ne(h2, 0)); return ret; }
    for (T sign : {T(-1), T(1)}) { pt v = (d * dr + perp(d) * sqrt(h2) * sign) / d2; ret.emplace_back(c1.o + v * c1.r, c2.o + v * r2); }
    return ret;
}
