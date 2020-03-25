#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; using pt = complex<T>; const T EPS = 1e-9;
#define x real()
#define y imag()
#define ref const pt &  
static_assert(is_floating_point<T>::value, "F must be a a floating point type");

istream &operator >> (istream &stream, pt &p) { T X, Y; stream >> X >> Y; p.real(X); p.imag(Y); return stream; }
ostream &operator << (ostream &stream, ref p) { return stream << fixed << setprecision(9) << p.x << ' ' << p.y; }

bool lt(T a, T b) { return a < b - EPS; }
bool le(T a, T b) { return !lt(b, a); }
bool gt(T a, T b) { return lt(b, a); }
bool ge(T a, T b) { return !lt(a, b); }
bool eq(T a, T b) { return !lt(a, b) && !lt(b, a); }
bool ne(T a, T b) { return lt(a, b) || lt(b, a); }
int sgn(T a) { return lt(a, 0) ? -1 : gt(a, 0) ? +1 : 0; }

bool operator == (ref a, ref b) { return eq(a.x, b.x) && eq(a.y, b.y); }
bool operator != (ref a, ref b) { return !(a == b); }
struct pt_eq { bool operator () (ref a, ref b) const { return a == b; } };
struct pt_ne { bool operator () (ref a, ref b) const { return a != b; } }; 

// abs gets polar distance, arg gets polar angle
T dot(ref a, ref b) { return a.x * b.x + a.y * b.y; }
T cross(ref a, ref b) { return a.x * b.y - a.y * b.x; }
T norm(ref a) { return dot(a, a); }
T distSq(ref a, ref b) { return norm(b - a); }
T dist(ref a, ref b) { return abs(b - a); }
T ang(ref a, ref b) { return arg(b - a); }
// sign of ang, area2, ccw: -1 if clockwise, 0 if collinear, +1 if counterclockwise
T ang(ref a, ref b, ref c) { return remainder(ang(b, a) - ang(b, c), 2 * acos(T(-1))); }
T area2(ref a, ref b, ref c) { return cross(b - a, c - a); }
int ccw(ref a, ref b, ref c) { return sgn(area2(a, b, c)); }
// a rotated theta radians around p
pt rot(ref a, ref p, const T &theta) { return (a - p) * pt(polar(T(1), theta)) + p; }
pt perp(ref a) { return pt(-a.y, a.x); }

bool xOrdLt(ref a, ref b) { return lt(a.x, b.x); }
bool xOrdLe(ref a, ref b) { return !xOrdLt(b, a); }
bool xOrdGt(ref a, ref b) { return xOrdLt(b, a); }
bool xOrdGe(ref a, ref b) { return !xOrdLt(a, b); }
bool yOrdLt(ref a, ref b) { return lt(a.y, b.y); }
bool yOrdLe(ref a, ref b) { return !yOrdLt(b, a); }
bool yOrdGt(ref a, ref b) { return yOrdLt(b, a); }
bool yOrdGe(ref a, ref b) { return !yOrdLt(a, b); }
bool xyOrdLt(ref a, ref b) { return eq(a.x, b.x) ? lt(a.y, b.y) : lt(a.x, b.x); }
bool xyOrdLe(ref a, ref b) { return !xyOrdLt(b, a); }
bool xyOrdGt(ref a, ref b) { return xyOrdLt(b, a); }
bool xyOrdGe(ref a, ref b) { return !xyOrdLt(a, b); }
bool yxOrdLt(ref a, ref b) { return eq(a.y, b.y) ? lt(a.x, b.x) : lt(a.y, b.y); }
bool yxOrdLe(ref a, ref b) { return !yxOrdLt(b, a); }
bool yxOrdGt(ref a, ref b) { return yxOrdLt(b, a); }
bool yxOrdGe(ref a, ref b) { return !yxOrdLt(a, b); }
bool rOrdLt(ref a, ref b) { return lt(norm(a), norm(b)); }
bool rOrdLe(ref a, ref b) { return !rOrdLt(b, a); }
bool rOrdGt(ref a, ref b) { return rOrdLt(b, a); }
bool rOrdGe(ref a, ref b) { return !rOrdLt(a, b); }
function<bool(ref, ref)> ccwOrdLt(ref p) { return [=] (ref a, ref b) { return 0 < ccw(p, a, b); }; }
function<bool(ref, ref)> ccwOrdLe(ref p) { return [=] (ref a, ref b) { return 0 <= ccw(p, a, b); }; }
function<bool(ref, ref)> ccwOrdGt(ref p) { return [=] (ref a, ref b) { return 0 > ccw(p, a, b); }; }
function<bool(ref, ref)> ccwOrdGe(ref p) { return [=] (ref a, ref b) { return 0 >= ccw(p, a, b); }; }
function<bool(ref, ref)> distOrdLt(ref p) { return [=] (ref a, ref b) { return lt(distSq(p, a), distSq(p, b)); }; }
function<bool(ref, ref)> distOrdLe(ref p) { return [=] (ref a, ref b) { return le(distSq(p, a), distSq(p, b)); }; }
function<bool(ref, ref)> distOrdGt(ref p) { return [=] (ref a, ref b) { return gt(distSq(p, a), distSq(p, b)); }; }
function<bool(ref, ref)> distOrdGe(ref p) { return [=] (ref a, ref b) { return ge(distSq(p, a), distSq(p, b)); }; }

// returns iterator to first element equal to pivot
// cmp is the angle comparison function (ccwOrdLt or ccwOrdGt)
// rot is the rotation comparison function (xyOrdLt, xyOrdGt, yxOrdLt, yxOrdGt)
// points p that return true for rot(p, pivot) will appear before those that do not
// points that are equal to pivot appear after all other points
template <class It, class F1 = function<function<bool(ref, ref)>(ref)>, class F2 = function<bool(ref, ref)>>
        It sortByAng(ref pivot, It st, It en, F1 cmp = ccwOrdLt, F2 rot = yxOrdLt) {
    en = partition(st, en, [&] (ref p) { return p != pivot; });
    It mid = partition(st, en, [&] (ref p) { return rot(p, pivot); });
    function<bool(ref, ref)> pc = cmp(pivot); sort(st, mid, pc); sort(mid, en, pc); return en;
}
