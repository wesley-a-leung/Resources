#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; using IndexType = long long; const T NEG_INF = numeric_limits<T>::lowest() / 2;
static_assert(is_integral<IndexType>::value, "IndexType must be integeral");

struct Line {
    T m, b;
    Line(T m = 0, T b = NEG_INF) : m(m), b(b) {}
    T eval(IndexType x) const { return b == NEG_INF ? NEG_INF : m * x + b; }
    bool majorize(const Line &line, IndexType l, IndexType r) const { return eval(l) >= line.eval(l) && eval(r) >= line.eval(r); }
};

// Supports adding lines in the form f(x) = mx + b and finding the maximum value of f(x) at any value of x
// along with line segments where l <= x <= r, in any order
// Time Complexity:
//   addLine: O(log (MX - MN))
//   addLineSegment: O((log (MX - MN))^2)
//   getMax: O(log (MX - MN))
// Memory Complexity: O(N log (MX - MN)) where N is the number of lines added
template <const bool maxHull> struct DynamicLiChaoTree {
    struct Node {
        Line line; IndexType l, r;
        Node(const Line &line = Line()) : line(line), l(-1), r(-1) {}
    };
    int root = -1; vector<Node> TR; IndexType MN, MX;
    DynamicLiChaoTree(IndexType MN, IndexType MX) : MN(MN), MX(MX) {}
    int addLine(int cur, IndexType tl, IndexType tr, Line line) {
        if (cur == -1) { cur = int(TR.size()); TR.emplace_back(); }
        if (line.majorize(TR[cur].line, tl, tr)) swap(line, TR[cur].line);
        if (TR[cur].line.majorize(line, tl, tr)) return cur;
        if (TR[cur].line.eval(tl) < line.eval(tl)) swap(line, TR[cur].line);
        IndexType m = tl + (tr - tl) / 2;
        if (line.eval(m) >= TR[cur].line.eval(m)) { swap(line, TR[cur].line); int cl = addLine(TR[cur].l, tl, m, line); TR[cur].l = cl; }
        else { int cr = addLine(TR[cur].r, m + 1, tr, line); TR[cur].r = cr; }
        return cur;
    }
    int addLineSegment(int cur, IndexType tl, IndexType tr, IndexType l, IndexType r, Line line) {
        if (r < tl || tr < l) return cur;
        if (l <= tl && tr <= r) return addLine(cur, tl, tr, line);
        if (cur == -1) { cur = int(TR.size()); TR.emplace_back(); }
        IndexType m = tl + (tr - tl) / 2;
        int cl = addLineSegment(TR[cur].l, tl, m, l, r, line), cr = addLineSegment(TR[cur].r, m + 1, tr, l, r, line);
        TR[cur].l = cl; TR[cur].r = cr; return cur;
    }
    T getMax(int cur, IndexType tl, IndexType tr, IndexType x) {
        if (cur == -1) return NEG_INF;
        T ret = TR[cur].line.eval(x); IndexType m = tl + (tr - tl) / 2;
        if (x <= m) return max(ret, getMax(TR[cur].l, tl, m, x));
        else return max(ret, getMax(TR[cur].r, m + 1, tr, x));
    }
    void addLine(Line line) { root = addLine(root, MN, MX, maxHull ? line : Line(-line.m, -line.b)); }
    void addLineSegment(Line line, IndexType l, IndexType r) { root = addLineSegment(root, MN, MX, l, r, maxHull ? line : Line(-line.m, -line.b)); }
    T getMax(IndexType x) { T ret = getMax(root, MN, MX, x); return maxHull ? ret : -ret; }
};
