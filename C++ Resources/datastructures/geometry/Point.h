#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; using F = long double; using pt = complex<T>; const F EPS = 1e-9;
#define x real()
#define y imag()
#define ref const pt &  
static_assert(is_floating_point<F>::value, "F must be a a floating point type");

template <class T> bool lt(const T &a, const T &b) { return a < b - T(EPS); }
template <class T> bool le(const T &a, const T &b) { return !lt(b, a); }
template <class T> bool gt(const T &a, const T &b) { return lt(b, a); }
template <class T> bool ge(const T &a, const T &b) { return !lt(a, b); }
template <class T> bool eq(const T &a, const T &b) { return !lt(a, b) && !lt(b, a); }
template <class T> bool ne(const T &a, const T &b) { return lt(a, b) || lt(b, a); }
template <class T> int sgn(const T &a) { return lt(a, T(0)) ? -1 : gt(a, T(0)) ? +1 : 0; }

bool operator == (ref a, ref b) { return eq(a.x, b.x) && eq(a.y, b.y); }
bool operator != (ref a, ref b) { return !(a == b); }
struct pt_eq { bool operator () (ref a, ref b) const { return a == b; } };
struct pt_ne { bool operator () (ref a, ref b) const { return a != b; } }; 

T dot(ref a, ref b) { return (conj(a) * b).x; }
T cross(ref a, ref b) { return (conj(a) * b).y; }
T norm(ref a) { return dot(a, a); }
F abs(ref a) { return sqrt(F(norm(a))); }
F arg(ref a) { return atan2(F(a.y), F(a.x)); }
T distSq(ref a, ref b) { return norm(b - a); }
F dist(ref a, ref b) { return abs(b - a); }
F ang(ref a, ref b) { return arg(b - a); }
// sign of ang, area2, ccw: -1 if clockwise, 0 if collinear, +1 if counterclockwise
F ang(ref a, ref b, ref c) { return remainder(ang(b, a) - ang(b, c), 2 * acos(F(-1))); }
T area2(ref a, ref b, ref c) { return cross(b - a, c - a); }
int ccw(ref a, ref b, ref c) { return sgn(area2(a, b, c)); }
// a rotated theta radians around p
pt rot(ref a, ref p, const F &theta) { return (a - p) * pt(polar(F(1), theta)) + p; }

bool xOrderLt(ref a, ref b) { return lt(a.x, b.x); }
bool xOrderLe(ref a, ref b) { return !xOrderLt(b, a); }
bool xOrderGt(ref a, ref b) { return xOrderLt(b, a); }
bool xOrderGe(ref a, ref b) { return !xOrderLt(a, b); }
bool yOrderLt(ref a, ref b) { return lt(a.y, b.y); }
bool yOrderLe(ref a, ref b) { return !yOrderLt(b, a); }
bool yOrderGt(ref a, ref b) { return yOrderLt(b, a); }
bool yOrderGe(ref a, ref b) { return !yOrderLt(a, b); }
bool xyOrderLt(ref a, ref b) { return eq(a.x, b.x) ? lt(a.y, b.y) : lt(a.x, b.x); }
bool xyOrderLe(ref a, ref b) { return !xyOrderLt(b, a); }
bool xyOrderGt(ref a, ref b) { return xyOrderLt(b, a); }
bool xyOrderGe(ref a, ref b) { return !xyOrderLt(a, b); }
bool yxOrderLt(ref a, ref b) { return eq(a.y, b.y) ? lt(a.x, b.x) : lt(a.y, b.y); }
bool yxOrderLe(ref a, ref b) { return !yxOrderLt(b, a); }
bool yxOrderGt(ref a, ref b) { return yxOrderLt(b, a); }
bool yxOrderGe(ref a, ref b) { return !yxOrderLt(a, b); }
bool rOrderLt(ref a, ref b) { return lt(norm(a), norm(b)); }
bool rOrderLe(ref a, ref b) { return !rOrderLt(b, a); }
bool rOrderGt(ref a, ref b) { return rOrderLt(b, a); }
bool rOrderGe(ref a, ref b) { return !rOrderLt(a, b); }
bool ccwOrderLt(ref p, ref a, ref b) { return ccw(p, a, b) > 0; }
bool ccwOrderLe(ref p, ref a, ref b) { return !ccwOrderLt(p, b, a); }
bool ccwOrderGt(ref p, ref a, ref b) { return ccwOrderLt(p, b, a); }
bool ccwOrderGe(ref p, ref a, ref b) { return !ccwOrderLt(p, a, b); }
bool distOrderLt(ref p, ref a, ref b) { return lt(distSq(p, a), distSq(p, b)); }
bool distOrderLe(ref p, ref a, ref b) { return !distOrderLt(p, b, a); }
bool distOrderGt(ref p, ref a, ref b) { return distOrderLt(p, b, a); }
bool distOrderGe(ref p, ref a, ref b) { return !distOrderLt(p, a, b); }

template <class Func> struct PtCmp {
    pt pivot; Func cmp; PtCmp(ref pivot, Func cmp) : pivot(pivot), cmp(cmp) {}
    bool operator () (ref a, ref b) { return cmp(pivot, a, b); }
};

template <class Func> PtCmp<Func> makePointCmp(ref pivot, Func cmp) { return PtCmp<Func>(pivot, cmp); }

// returns iterator to first element equal to pivot
// cmp is the angle comparison function (ccwOrderLt or ccwOrderGt)
// rot is the rotation comparison function (xyOrderLt, xyOrderGt, yxOrderLt, yxOrderGt)
// points p that return true for rot(p, pivot) will appear before those that do not
// points that are equal to pivot appear after all other points
template <class It, class F1 = function<bool(ref, ref, ref)>, class F2 = function<bool(ref, ref)>>
        It sortByAng(ref pivot, It st, It en, F1 cmp = ccwOrderLt, F2 rot = yxOrderLt) {
    en = partition(st, en, [&] (ref p) { return p != pivot; });
    It mid = partition(st, en, [&] (ref p) { return rot(p, pivot); });
    PtCmp<F1> pc(pivot, cmp); sort(st, mid, pc); sort(mid, en, pc); return en;
}

// returns true iff p is on the line segment a-b
bool onSeg(ref p, ref a, ref b) { return ccw(p, a, b) == 0 && le(dot(a - p, b - p), T(0)); }
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
    sort(ret.begin(), ret.end(), xyOrderLt); ret.erase(unique(ret.begin(), ret.end(), pt_eq()), ret.end());
    return ret;
}
