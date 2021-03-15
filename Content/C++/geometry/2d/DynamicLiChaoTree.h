#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b, or line segments
//   in the form of f(x) = mx + b over l <= x <= r, and finding
//   the maximum value of f(x) at an integral point x where MN <= x <= MX
// Template Arguments:
//   T: the type of the slope (m) and intercept (b) of the line, as well as
//     the type of the function argument (x)
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   MN: the minimum bound (inclusive) for the value of x, allowing queries
//     where MN <= x
//   MX: the maximum bound (inclusive) for the value of x, allowing queries
//     where x <= MX
//   INF: a value for positive infinity, must be negatable
// Functions:
//   addLine(m, b): adds a line in the form f(x) = mx + b to the set of lines
//   addLine(m, b, l, r): adds a line segment in the form f(x) = mx + b
//     where l <= x <= r, to the set of lines
//   getMax(x): finds the maximum value of f(x) (based on the comparator)
//     for all inserted lines
//   clear(): removes all lines from the seg
// In practice, has a moderate constant, performance compared to
//   DynamicConvexHullTrick (which uses multiset) and
//   DynamicConvexHullTrickSqrtBuffer can vary, slower than LiChaoTree
// Time Complexity:
//   constructor: O(1)
//   addLine, getMax: O(log(MX - MN)) for the range [MX, MN]
//   addLineSegment: O(log(MX - MN) ^ 2) for the range [MX, MN]
//   clear: O(Q log(MX - MN)) for the range [MX, MN] and
//            Q addLine queries,
//          O(Q log(MX - MN)^2) for the range [MX, MN] and
//            Q addLineSegmentqueries
// Memory Complexity: O(Q log(MX - MN)) for the range [MX, MN] and
//                      Q addLine queries,
//                    O(Q log(MX - MN)^2) for the range [MX, MN] and
//                      Q addLineSegment queries
// Tested:
//   https://judge.yosupo.jp/problem/line_add_get_min
//   https://judge.yosupo.jp/problem/segment_add_get_min
//   https://open.kattis.com/problems/longestlife
//   https://www.spoj.com/problems/CHTPRAC/
//   https://csacademy.com/contest/round-70/task/squared-ends/
template <class IndexType, class T, class Cmp = less<T>>
struct DynamicLiChaoTree {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  using Line = pair<T, T>;
  struct Node {
    Line line; int l, r; Node(T m, T b) : line(m, b), l(-1), r(-1) {}
  };
  IndexType MN, MX; T INF; int root; vector<Node> TR;
  T eval(const Line &l, IndexType x) const {
    return l.second == INF ? INF : l.first * x + l.second;
  }
  bool majorize(const Line &a, const Line &b, IndexType l, IndexType r) {
    return !Cmp()(eval(a, l), eval(b, l)) && !Cmp()(eval(a, r), eval(b, r));
  }
  DynamicLiChaoTree(IndexType MN, IndexType MX,
                    T inf = numeric_limits<T>::max())
      : MN(MN), MX(MX), INF(min(inf, -inf, Cmp())), root(-1) {}
  int addLine(int k, IndexType tl, IndexType tr, Line line) {
    if (k == -1) { k = TR.size(); TR.emplace_back(T(), INF); }
    if (majorize(line, TR[k].line, tl, tr)) swap(line, TR[k].line);
    if (majorize(TR[k].line, line, tl, tr)) return k;
    if (Cmp()(eval(TR[k].line, tl), eval(line, tl))) swap(line, TR[k].line);
    IndexType m = tl + (tr - tl) / 2;
    if (!Cmp()(eval(line, m), eval(TR[k].line, m))) {
      swap(line, TR[k].line);
      int nl = addLine(TR[k].l, tl, m, line); TR[k].l = nl;
    } else { int nr = addLine(TR[k].r, m + 1, tr, line); TR[k].r = nr; }
    return k;
  }
  int addLineSegment(int k, IndexType tl, IndexType tr,
                      IndexType l, IndexType r, Line line) {
    if (r < tl || tr < l) return k;
    if (l <= tl && tr <= r) return addLine(k, tl, tr, line);
    if (k == -1) { k = TR.size(); TR.emplace_back(T(), INF); }
    IndexType m = tl + (tr - tl) / 2;
    int nl = addLineSegment(TR[k].l, tl, m, l, r, line); TR[k].l = nl;
    int nr = addLineSegment(TR[k].r, m + 1, tr, l, r, line); TR[k].r = nr;
    return k;
  }
  T getMax(int k, IndexType tl, IndexType tr, IndexType x) const {
    if (k == -1) return INF;
    T ret = eval(TR[k].line, x); IndexType m = tl + (tr - tl) / 2;
    if (x <= m) return max(ret, getMax(TR[k].l, tl, m, x), Cmp());
    else return max(ret, getMax(TR[k].r, m + 1, tr, x), Cmp());
  }
  void addLine(T m, T b) { root = addLine(root, MN, MX, Line(m, b)); }
  void addLineSegment(T m, T b, IndexType l, IndexType r) {
    root = addLineSegment(root, MN, MX, l, r, Line(m, b));
  }
  T getMax(IndexType x) const { return getMax(root, MN, MX, x); }
  void clear() { root = -1; TR.clear(); }
};
