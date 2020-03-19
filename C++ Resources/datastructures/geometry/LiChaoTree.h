#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long long; const T NEG_INF = numeric_limits<T>::lowest() / 2;

struct Line {
    T m, b;
    Line(T m = 0, T b = NEG_INF) : m(m), b(b) {}
    T eval(int x) const { return b == NEG_INF ? NEG_INF : m * x + b; }
    bool majorize(const Line &line, int l, int r) const { return eval(l) >= line.eval(l) && eval(r) >= line.eval(r); }
};

// Supports adding lines in the form f(x) = mx + b and finding the maximum value of f(x) at integer values of x
// along with line segments where l <= x <= r, in any order
// Time Complexity:
//   addLine: O(log N)
//   addLineSegment: O((log N)^2)
//   getMax: O(log N)
// Memory Complexity: O(N) where N is the range of x
template <const int MAXN, const bool ONE_INDEXED, const bool maxHull> struct LiChaoTree {
    int N; Line TR[MAXN * 2];
    void init(int size) { N = size; }
    void addLine(int cur, int tl, int tr, Line line) {
        if (line.majorize(TR[cur], tl, tr)) swap(line, TR[cur]);
        if (TR[cur].majorize(line, tl, tr)) return;
        if (TR[cur].eval(tl) < line.eval(tl)) swap(line, TR[cur]);
        int m = tl + (tr - tl) / 2, rc = cur + (m - tl + 1) * 2;
        if (line.eval(m) >= TR[cur].eval(m)) { swap(line, TR[cur]); addLine(cur + 1, tl, m, line); }
        else addLine(rc, m + 1, tr, line);
    }
    void addLineSegment(int cur, int tl, int tr, int l, int r, Line line) {
        if (r < tl || tr < l) return;
        if (l <= tl && tr <= r) { addLine(cur, tl, tr, line); return; }
        int m = tl + (tr - tl) / 2, rc = cur + (m - tl + 1) * 2;
        addLineSegment(cur + 1, tl, m, l, r, line); addLineSegment(rc, m + 1, tr, l, r, line);
    }
    T getMax(int cur, int tl, int tr, int x) {
        T ret = TR[cur].eval(x);
        if (tl == tr) return ret;
        int m = tl + (tr - tl) / 2, rc = cur + (m - tl + 1) * 2;
        if (x <= m) return max(ret, getMax(cur + 1, tl, m, x));
        else return max(ret, getMax(rc, m + 1, tr, x));
    }
    void addLine(Line line) { addLine(0, ONE_INDEXED, N - !ONE_INDEXED, maxHull ? line : Line(-line.m, -line.b)); }
    void addLineSegment(Line line, int l, int r) {
        addLineSegment(0, ONE_INDEXED, N - !ONE_INDEXED, l, r, maxHull ? line : Line(-line.m, -line.b));
    }
    T getMax(int x) { T ret = getMax(0, ONE_INDEXED, N - !ONE_INDEXED, x); return maxHull ? ret : -ret; }
};
