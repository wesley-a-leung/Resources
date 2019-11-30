#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; using F = long double; const F EPS = 1e-9;
static_assert(is_floating_point<F>::value, "F must be a a floating point type");

template <class U> bool lt(const U &a, const U &b) { return a < b - U(EPS); }
template <class U> bool le(const U &a, const U &b) { return !lt(b, a); }
template <class U> bool gt(const U &a, const U &b) { return lt(b, a); }
template <class U> bool ge(const U &a, const U &b) { return !lt(a, b); }
template <class U> bool eq(const U &a, const U &b) { return !lt(a, b) && !lt(b, a); }
template <class U> bool ne(const U &a, const U &b) { return lt(a, b) || lt(b, a); }

struct Point {
    T x, y;
    Point(const T &x = 0, const T &y = 0) : x(x), y(y) {}
    F r() const { return sqrt(F(x * x + y * y)); }
    T rSq() const { return x * x + y * y; }
    F theta() const { return atan2(F(y), F(x)); }
    F ang(const Point &p) const { return atan2(F(p.y - y), F(p.x - x)); }
    // sign of ang matches sign of ccw
    static F ang(const Point &a, const Point &b, const Point &c) { return remainder(b.ang(a) - b.ang(c), 2 * acos(F(-1))); } 
    static int ccw(const Point &a, const Point &b, const Point &c) { // -1 if clockwise, 0 if collinear, +1 if counterclockwise
        T area2 = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (lt(area2, T(0))) return -1;
        else if (gt(area2, T(0))) return +1;
        else return 0;
    }
    static T area2(const Point &a, const Point &b, const Point &c) { return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }
    Point rotate(const Point &p, F theta) const { // this point rotated theta radians around p
        return Point(p.x + (x - p.x) * cos(theta) - (y - p.y) * sin(theta), p.y + (x - p.x) * sin(theta) + (y - p.y) * cos(theta));
    }
    F dist(const Point &p) const { T dx = x - p.x, dy = y - p.y; return sqrt(F(dx * dx + dy * dy)); }
    T distSq(const Point &p) const { T dx = x - p.x, dy = y - p.y; return dx * dx + dy * dy; }
    bool onSegment(const Point &a, const Point &b) const { return eq(dist(a) + dist(b), a.dist(b)); }
    static bool lineSegmentIntersects(const Point &a, const Point &b, const Point &p, const Point &q) {
        int o1 = Point::ccw(a, b, p), o2 = Point::ccw(a, b, q), o3 = Point::ccw(p, q, a), o4 = Point::ccw(p, q, b);
        if (o1 != o2 && o3 != o4) return true;
        if (o1 == 0 && p.onSegment(a, b)) return true;
        if (o2 == 0 && q.onSegment(a, b)) return true;
        if (o3 == 0 && a.onSegment(p, q)) return true;
        if (o4 == 0 && b.onSegment(p, q)) return true;
        return false;
    }
    static Point lineIntersection(const Point &a, const Point &b, const Point &p, const Point &q) {
        T A1 = b.y - a.y, B1 = a.x - b.x, C1 = A1 * a.x + B1 * a.y;
        T A2 = q.y - p.y, B2 = p.x - q.x, C2 = A2 * p.x + B2 * p.y, D = A1 * B2 - A2 * B1;
        if (eq(D, T(0))) throw runtime_error("The lines do not intersect or are collinear");
        return Point((B2 * C1 - B1 * C2) / D, (A1 * C2 - A2 * C1) / D);
    }
    Point project(const Point &a, const Point &b) const {
        T A1 = b.x - a.x, A2 = b.y - a.y, B1 = x - a.x, B2 = y - a.y, C = A1 * B1 + A2 * B2, D = A1 * A1 + A2 * A2;
        return Point(a.x + A1 * C / D, a.y + A2 * C / D);
    }
    Point reflect(const Point &a, const Point &b) const { Point p = project(a, b); return Point(2 * p.x - x, 2 * p.y - y); }
    bool operator == (const Point &a) const { return eq(x, a.x) && eq(y, a.y); }
    bool operator != (const Point &a) const { return !(*this == a); }
    static bool xOrderLt(const Point &a, const Point &b) { return lt(a.x, b.x); }
    static bool xOrderLe(const Point &a, const Point &b) { return !xOrderLt(b, a); }
    static bool xOrderGt(const Point &a, const Point &b) { return xOrderLt(b, a); }
    static bool xOrderGe(const Point &a, const Point &b) { return !xOrderLt(a, b); }
    static bool yOrderLt(const Point &a, const Point &b) { return lt(a.y, b.y); }
    static bool yOrderLe(const Point &a, const Point &b) { return !yOrderLt(b, a); }
    static bool yOrderGt(const Point &a, const Point &b) { return yOrderLt(b, a); }
    static bool yOrderGe(const Point &a, const Point &b) { return !yOrderLt(a, b); }
    static bool xyOrderLt(const Point &a, const Point &b) { return eq(a.x, b.x) ? lt(a.y, b.y) : lt(a.x, b.x); }
    static bool xyOrderLe(const Point &a, const Point &b) { return !xyOrderLt(b, a); }
    static bool xyOrderGt(const Point &a, const Point &b) { return xyOrderLt(b, a); }
    static bool xyOrderGe(const Point &a, const Point &b) { return !xyOrderLt(a, b); }
    static bool yxOrderLt(const Point &a, const Point &b) { return eq(a.y, b.y) ? lt(a.x, b.x) : lt(a.y, b.y); }
    static bool yxOrderLe(const Point &a, const Point &b) { return !yxOrderLt(b, a); }
    static bool yxOrderGt(const Point &a, const Point &b) { return yxOrderLt(b, a); }
    static bool yxOrderGe(const Point &a, const Point &b) { return !yxOrderLt(a, b); }
    static bool rOrderLt(const Point &a, const Point &b) { return lt(a.rSq(), b.rSq()); }
    static bool rOrderLe(const Point &a, const Point &b) { return !rOrderLt(b, a); }
    static bool rOrderGt(const Point &a, const Point &b) { return rOrderLt(b, a); }
    static bool rOrderGe(const Point &a, const Point &b) { return !rOrderLt(a, b); }
    static bool ccwOrderLt(const Point &p, const Point &a, const Point &b) { return ccw(p, a, b) > 0; }
    static bool ccwOrderLe(const Point &p, const Point &a, const Point &b) { return !ccwOrderLt(p, b, a); }
    static bool ccwOrderGt(const Point &p, const Point &a, const Point &b) { return ccwOrderLt(p, b, a); }
    static bool ccwOrderGe(const Point &p, const Point &a, const Point &b) { return !ccwOrderLt(p, a, b); }
    static bool distOrderLt(const Point &p, const Point &a, const Point &b) { return lt(p.distSq(a), b.distSq(b)); }
    static bool distOrderLe(const Point &p, const Point &a, const Point &b) { return !distOrderLt(p, b, a); }
    static bool distOrderGt(const Point &p, const Point &a, const Point &b) { return distOrderLt(p, b, a); }
    static bool distOrderGe(const Point &p, const Point &a, const Point &b) { return !distOrderLt(p, a, b); }
};

template <class Func> struct PointCmp {
    Point pivot; Func cmp;
    PointCmp(const Point &pivot, Func cmp) : pivot(pivot), cmp(cmp) {}
    bool operator () (const Point &a, const Point &b) { return cmp(pivot, a, b); }
};

template <class Func> auto makePointCmp(const Point &pivot, Func cmp) { return PointCmp<Func>(pivot, cmp); }

// returns iterator to first element equal to pivot
// cmp is the angle comparison function (ccwOrderLt or ccwOrderGt)
// rot is the rotation comparison function (xyOrderLt, xyOrderGt, yxOrderLt, yxOrderGt)
// points p that return true for rot(p, pivot) will appear before those that do not
// points that are equal to pivot appear after all other points
template <class It, class F1 = function<bool(const Point&, const Point&, const Point&)>,
        class F2 = function<bool(const Point&, const Point&)>>
        It sortByAng(const Point &pivot, It st, It en, F1 cmp = Point::ccwOrderLt, F2 rot = Point::yxOrderLt) {
    en = partition(st, en, [&] (const Point &p) { return p != pivot; });
    It mid = partition(st, en, [&] (const Point &p) { return rot(p, pivot); });
    PointCmp<F1> pc(pivot, cmp); sort(st, mid, pc); sort(mid, en, pc); return en;
}
