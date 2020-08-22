#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

struct Line {
    pt v; T c;
    Line(T a = 0, T b = 0, T c = 0) : v(b, -a), c(c) {}
    Line(ref v, T c) : v(v), c(c) {}
    Line(ref p, ref q) : v(q - p), c(cross(v, p)) {}
    T eval(ref p) const { return cross(v, p) - c; }
    // +1 if left of line, 0 if on line, -1 if right of line
    int onLeft(ref p) const { return sgn(eval(p)); }
    T distSq(ref p) const { T e = eval(p); return e * e / norm(v); }
    T dist(ref p) const { return abs(eval(p) / abs(v)); }
    Line perpThrough(ref p) const { return Line(p, p + perp(v)); }
    // sort by othogonal projection
    function<bool(ref, ref)> cmpProjLt() const { return [=] (ref p, ref q) { return lt(dot(v, p), dot(v, q)); }; }
    function<bool(ref, ref)> cmpProjLe() const { return [=] (ref p, ref q) { return le(dot(v, p), dot(v, q)); }; }
    function<bool(ref, ref)> cmpProjGt() const { return [=] (ref p, ref q) { return gt(dot(v, p), dot(v, q)); }; }
    function<bool(ref, ref)> cmpProjGe() const { return [=] (ref p, ref q) { return ge(dot(v, p), dot(v, q)); }; }
    Line translate(ref p) const { return Line(v, c + cross(v, p)); }
    Line shiftLeft(T d) const { return Line(v, c + d * abs(v)); }
    pt proj(ref p) const { return p - perp(p) * eval(p) / norm(v); }
    pt refl(ref p) const { return p - perp(p) * T(2) * eval(p) / norm(v); }
};
int lineIntersection(const Line &l1, const Line &l2, pt &res) { // returns 0 if no intersection, 1 if proper intersection, 2 otherwise
    T d = cross(l1.v, l2.v);
    if (eq(d, 0)) return l2.v * l1.c == l1.v * l2.c ? 2 : 0;
    res = (l2.v * l1.c - l1.v * l2.c) / d; return 1;
}
Line bisector(const Line &l1, const Line &l2, bool interior) {
    T s = interior ? 1 : -1; return Line(l2.v / abs(l2.v) + l1.v / abs(l1.v) * s, l2.c / abs(l2.v) + l1.c / abs(l1.v) * s);
}

// returns true iff p is on the line segment a-b
bool onSeg(ref p, ref a, ref b) { return ccw(p, a, b) == 0 && le(dot(a - p, b - p), 0); }
int lineSegIntersects(ref a, ref b, ref p, ref q) { // returns 0 if no intersection, 1 if proper intersection, 2 otherwise
    int o1 = ccw(a, b, p), o2 = ccw(a, b, q), o3 = ccw(p, q, a), o4 = ccw(p, q, b);
    if (o1 != o2 && o3 != o4) return 1;
    else if ((o1 == 0 && onSeg(p, a, b)) || (o2 == 0 && onSeg(q, a, b)) || (o3 == 0 && onSeg(a, p, q)) || (o4 == 0 && onSeg(b, p, q))) return 2;
    else return 0;
}
vector<pt> lineSegIntersection(ref a, ref b, ref p, ref q) {
    int intersects = lineSegIntersects(a, b, p, q);
    if (intersects == 0) return vector<pt>();
    else if (intersects == 1) { T c1 = cross(p - a, b - a), c2 = cross(q - a, b - a); return vector<pt>{(c1 * q - c2 * p) / (c1 - c2)}; }
    vector<pt> ret;
    if (onSeg(p, a, b)) ret.push_back(p);
    if (onSeg(q, a, b)) ret.push_back(q);
    if (onSeg(a, p, q)) ret.push_back(a);
    if (onSeg(b, p, q)) ret.push_back(b);
    sort(ret.begin(), ret.end(), xyOrdLt); ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end()); return ret;
}
T segPtDist(ref p, ref a, ref b) {
    if (a != b) {
        Line l(a, b);
        if (l.cmpProjLt()(a, p) && l.cmpProjLt()(p, b)) return l.dist(p);
    }
    return min(dist(p, a), dist(p, b));
}
pt closestPtToSeg(ref p, ref a, ref b) {
    if (a != b) {
        Line l(a, b);
        if (l.cmpProjLt()(a, p) && l.cmpProjLt()(p, b)) return l.proj(p);
    }
    return lt(dist(p, a), dist(p, b)) ? a : b;
}
T segSegDist(ref a, ref b, ref p, ref q) {
    return lineSegIntersects(a, b, p, q) > 0 ? 0 : min({segPtDist(p, a, b), segPtDist(q, a, b), segPtDist(a, p, q), segPtDist(b, p, q)});
}
