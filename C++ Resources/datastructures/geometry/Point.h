#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = double; using F = double; const F EPS = 1e-9;

struct Point {
    T x, y;
    Point(T x = 0, T y = 0) : x(x), y(y) {}
    F polarRadius() const { return sqrt(x * x + y * y); }
    F theta() const { return atan2(y, x); }
    F angleTo(const Point &that) const { return atan2(that.y - y, that.x - x); }
    static int ccw(const Point &a, const Point &b, const Point &c) { // -1 if clockwise, 0 if colinear, +1 if counterclockwise
        T area2 = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (area2 < -T(EPS)) return -1;
        else if (area2 > T(EPS)) return +1;
        else return 0;
    }
    static T area2(const Point &a, const Point &b, const Point &c) { return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }
    Point rotate(const Point &that, F theta) const { // this point rotated theta radians around that point
        T x = that.x + (this->x - that.x) * cos(theta) - (this->y - that.y) * sin(theta);
        T y = that.y + (this->x - that.x) * sin(theta) + (this->y - that.y) * cos(theta);
        return Point(x, y);
    }
    F distanceTo(const Point &that) const { T dx = x - that.x, dy = y - that.y; return sqrt(dx * dx + dy * dy); }
    T distanceSquaredTo(const Point &that) const { T dx = x - that.x, dy = y - that.y; return dx * dx + dy * dy; }
    bool onSegment(const Point &p, const Point &q) const {
        return abs(distanceSquaredTo(p) + distanceSquaredTo(q) - p.distanceSquaredTo(q)) <= EPS;
    }
    static bool lineSegmentIntersects(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
        int o1 = Point::ccw(p1, q1, p2), o2 = Point::ccw(p1, q1, q2), o3 = Point::ccw(p2, q2, p1), o4 = Point::ccw(p2, q2, q1);
        if (o1 != o2 && o3 != o4) return true;
        if (o1 == 0 && p2.onSegment(p1, q1)) return true;
        if (o2 == 0 && q2.onSegment(p1, q1)) return true;
        if (o3 == 0 && p1.onSegment(p2, q2)) return true;
        if (o4 == 0 && q1.onSegment(p2, q2)) return true;
        return false;
    }
    static Point lineIntersection(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
        T A1 = q1.y - p1.y, B1 = p1.x - q1.x, C1 = A1 * p1.x + B1 * p1.y;
        T A2 = q2.y - p2.y, B2 = p2.x - q2.x, C2 = A2 * p2.x + B2 * p2.y, det = A1 * B2 - A2 * B1;
        if (abs(det) <= T(EPS)) throw runtime_error("The lines do not intersect");
        return Point((B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det);
    }
    int compareXThenY(const Point &that) const {
        if (x < that.x - T(EPS)) return -1;
        if (x > that.x + T(EPS)) return +1;
        if (y < that.y - T(EPS)) return -1;
        if (y > that.y + T(EPS)) return +1;
        return 0;
    }
    int compareYThenX(const Point &that) const {
        if (y < that.y - T(EPS)) return -1;
        if (y > that.y + T(EPS)) return +1;
        if (x < that.x - T(EPS)) return -1;
        if (x > that.x + T(EPS)) return +1;
        return 0;
    }
    bool operator == (const Point &other) const { return abs(x - other.x) <= T(EPS) && abs(y - other.y) <= T(EPS); }
    bool operator != (const Point &other) const { return abs(x - other.x) > T(EPS) || abs(y - other.y) > T(EPS); }
    static bool xOrderLt(const Point &p, const Point &q) { return p.x < q.x - T(EPS); }
    static bool xOrderLe(const Point &p, const Point &q) { return p.x <= q.x + T(EPS); }
    static bool xOrderGt(const Point &p, const Point &q) { return p.x > q.x + T(EPS); }
    static bool xOrderGe(const Point &p, const Point &q) { return p.x >= q.x - T(EPS); }
    static bool yOrderLt(const Point &p, const Point &q) { return p.y < q.y - T(EPS); }
    static bool yOrderLe(const Point &p, const Point &q) { return p.y <= q.y + T(EPS); }
    static bool yOrderGt(const Point &p, const Point &q) { return p.y > q.y + T(EPS); }
    static bool yOrderGe(const Point &p, const Point &q) { return p.y >= q.y - T(EPS); }
    static bool rOrderLt(const Point &p, const Point &q) { return (p.x * p.x + p.y * p.y) < (q.x * q.x + q.y * q.y) - T(EPS); }
    static bool rOrderLe(const Point &p, const Point &q) { return (p.x * p.x + p.y * p.y) <= (p.x * p.x + p.y * p.y) + T(EPS); }
    static bool rOrderGt(const Point &p, const Point &q) { return (p.x * p.x + p.y * p.y) > (q.x * q.x + q.y * q.y) + T(EPS); }
    static bool rOrderGe(const Point &p, const Point &q) { return (p.x * p.x + p.y * p.y) >= (q.x * q.x + q.y * q.y) - T(EPS); }
    bool polarOrderLt(const Point &p, const Point &q) const { return ccw(*this, p, q) > 0; }
    bool polarOrderLe(const Point &p, const Point &q) const { return ccw(*this, p, q) >= 0; }
    bool polarOrderGt(const Point &p, const Point &q) const { return ccw(*this, p, q) < 0; }
    bool polarOrderGe(const Point &p, const Point &q) const { return ccw(*this, p, q) <= 0; }
    bool atan2OrderLt(const Point &p, const Point &q) const { return angleTo(p) < angleTo(q) - T(EPS); }
    bool atan2OrderLe(const Point &p, const Point &q) const { return angleTo(p) <= angleTo(q) + T(EPS); }
    bool atan2OrderGt(const Point &p, const Point &q) const { return angleTo(p) > angleTo(q) + T(EPS); }
    bool atan2OrderGe(const Point &p, const Point &q) const { return angleTo(p) >= angleTo(q) - T(EPS); }
    bool distanceToOrderLt(const Point &p, const Point &q) { return distanceSquaredTo(p) < distanceSquaredTo(q) - T(EPS); }
    bool distanceToOrderLe(const Point &p, const Point &q) { return distanceSquaredTo(p) <= distanceSquaredTo(q) + T(EPS); }
    bool distanceToOrderGt(const Point &p, const Point &q) { return distanceSquaredTo(p) > distanceSquaredTo(q) + T(EPS); }
    bool distanceToOrderGe(const Point &p, const Point &q) { return distanceSquaredTo(p) >= distanceSquaredTo(q) - T(EPS); }
};

struct Point_hash { size_t operator ()(const Point &p) const { return 31 * hash<T> {}(p.x) + hash<T> {}(p.y); } };
