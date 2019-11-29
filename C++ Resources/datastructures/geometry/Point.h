#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; using Point = complex<T>; const T EPS = 1e-9;

#define x(...) real(__VA_ARGS__)
#define y(...) imag(__VA_ARGS__)

bool eq(const T &a, const T &b) { return abs(a - b) <= EPS; }
bool ne(const T &a, const T &b) { return !eq(a, b); }
bool lt(const T &a, const T &b) { return a < b - EPS; }
bool le(const T &a, const T &b) { return !lt(b, a); }
bool gt(const T &a, const T &b) { return lt(b, a); }
bool ge(const T &a, const T &b) { return !lt(a, b); }

bool operator == (const Point &a, const Point &b) { return eq(x(a), x(b)) && eq(y(a), y(b)); }
bool operator == (const T &a, const Point &b) { return Point(a) == b; }
bool operator == (const Point &a, const T &b) { return a == Point(b); }
bool operator != (const Point &a, const Point &b) { return !(a == b); }
bool operator != (const T &a, const Point &b) { return !(a == b); }
bool operator != (const Point &a, const T &b) { return !(a == b); }
T dot(const Point &a, const Point &b) { return x(conj(a) * b); }
T cross(const Point &a, const Point &b) { return y(conj(a) * b); }
T r(const Point &a) { return abs(a); }
T rSq(const Point &a) { return norm(a); }
T theta(const Point &a) { return arg(a); }
T ang(const Point &a, const Point &b) { return arg(b - a); }
// sign of angle matches sign of ccw
T ang(const Point &a, const Point &b, const Point &c) { return remainder(arg(a - b) - arg(c - b), 2 * acos(T(-1))); }
T slope(const Point &a, const Point &b) { return tan(arg(b - a)); }
T dist(const Point &a, const Point &b) { return abs(b - a); }
T distSq(const Point &a, const Point &b) { return norm(b - a); }
T area2(const Point &a, const Point &b, const Point &c) { return cross(b - a, c - a); }
int ccw(const Point &a, const Point &b, const Point &c) { // -1 if clockwise, 0 if collinear, +1 if counterclockwise
    T a2 = cross(b - a, c - a);
    if (lt(a2, 0)) return -1;
    else if (gt(a2, 0)) return +1;
    return 0;
}
// point a rotated theta radians around p
Point rotate(const Point &a, const Point &p, const T &theta) { return p + (a - p) * polar(T(1), theta); }
bool onSeg(const Point &p, const Point &a, const Point &b) { return eq(dist(p, a) + dist(p, b), dist(a, b)); }   
bool intersects(const Point &a, const Point &b, const Point &p, const Point &q) {
    int o1 = ccw(a, b, p), o2 = ccw(a, b, q), o3 = ccw(p, q, a), o4 = ccw(p, q, b);
    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSeg(p, a, b)) return true;
    if (o2 == 0 && onSeg(q, a, b)) return true;
    if (o3 == 0 && onSeg(a, p, q)) return true;
    if (o4 == 0 && onSeg(b, p, q)) return true;
    return false;
}
Point intersection(const Point &a, const Point &b, const Point &p, const Point &q) {
    T c1 = cross(p - a, b - a), c2 = cross(q - a, b - a);
    if (eq(c1, c2)) throw runtime_error("The lines do not intersect or are collinear");
    return (c1 * q - c2 * p) / (c1 - c2);
}
Point projection(const Point &p, const Point &a, const Point &b) { Point ab = b - a; return a + ab * dot(p - a, ab) / norm(ab); }
Point reflection(const Point &p, const Point &a, const Point &b) { Point ab = b - a; return a + conj((p - a) / ab) * ab; }
