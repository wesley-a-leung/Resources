#pragma once
#include <bits/stdc++.h>
using namespace std;

// Wavelet Tree (using a persistent segment tree)
//   supporting select and rank operations for a subarray
// boolean flag to specify mode of the wavelet tree (rank or select)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a large constant, slower than Merge Sort Trees for
//   rank queries, but much faster for select queries
// Time Complexity:
//   constructor, rank, select: O(N log N)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.spoj.com/problems/KQUERY/ (rank)
//   https://www.spoj.com/problems/KQUERYO/ (rank)
//   https://codeforces.com/contest/1284/problem/D (rank)
//   https://www.spoj.com/problems/MKTHNUM/ (select)
//   https://judge.yosupo.jp/problem/range_kth_smallest (select)
const bool Rank = true, Select = false;
template <const bool MODE, class T, class Comparator = less<T>>
struct WaveletTree {
#define def(mode, ret) template <const bool _ = MODE>\
typename enable_if<_ == mode, ret>::type
  Comparator cmp; int N, curNode; vector<int> P, aux, roots; vector<T> A;
  struct Node { int l, r, val; }; vector<Node> TR;
  int build(int tl, int tr) {
    int x = curNode++; if (tl == tr) { TR[x].val = 0; return x; }
    int m = tl + (tr - tl) / 2;
    TR[x].l = build(tl, m); TR[x].r = build(m + 1, tr);
    TR[x].val = TR[TR[x].l].val + TR[TR[x].r].val; return x;
  }
  int update(int x, int tl, int tr, int i) {
    if (i < tl || tr < i) return x;
    int y = curNode++; if (tl == tr) { TR[y].val = 1; return y; }
    int m = tl + (tr - tl) / 2; TR[y].l = update(TR[x].l, tl, m, i);
    TR[y].r = update(TR[x].r, m + 1, tr, i);
    TR[y].val = TR[TR[y].l].val + TR[TR[y].r].val; return y;
  }
  def(Select, int) select(int x, int y, int tl, int tr, int k) {
    if (tl == tr) return tl;
    int m = tl + (tr - tl) / 2, t = TR[TR[y].l].val - TR[TR[x].l].val;
    if (k < t) return select(TR[x].l, TR[y].l, tl, m, k);
    else return select(TR[x].r, TR[y].r, m + 1, tr, k - t);
  }
  def(Rank, int) rank(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return 0;
    if (l <= tl && tr <= r) return TR[x].val;
    int m = tl + (tr - tl) / 2;
    return rank(TR[x].l, tl, m, l, r) + rank(TR[x].r, m + 1, tr, l, r);
  }
  template <class It> WaveletTree(It st, It en)
      : N(en - st), curNode(0), P(N), aux(N), roots(N + 1), A(st, en),
        TR(N * (__lg(N * 2 - 1) + 3) - 1) {
    iota(P.begin(), P.end(), 0);
    stable_sort(P.begin(), P.end(), [&] (int i, int j) {
      return cmp(A[i], A[j]);
    });
    for (int i = 0; i < N; i++) {
      if (MODE == Rank) aux[i] = P[i];
      else aux[P[i]] = i;
    }
    roots[0] = build(0, N - 1); for (int i = 0; i < N; i++)
      roots[i + 1] = update(roots[i], 0, N - 1, aux[i]);
  }
  def(Select, T) select(int l, int r, int k) {
    return A[P[select(roots[l], roots[r + 1], 0, N - 1, k)]];
  }
  def(Rank, int) rank(int l, int r, T k) {
    return rank(roots[lower_bound(P.begin(), P.end(), N, [&] (int i, int) {
                  return cmp(A[i], k);
                }) - P.begin()], 0, N - 1, l, r);
  }
#undef def
};
