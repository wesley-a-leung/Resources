#pragma once
#include <bits/stdc++.h>
using namespace std;

// Wavelet Tree (using a persistent segment tree)
//   supporting select and rank operations for a subarray
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
template <class T, class Comparator = less<T>> struct WaveletTree {
  Comparator cmp; int N, curNode; vector<int> ind, rnk, roots; vector<T> A;
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
  int select(int x, int y, int tl, int tr, int k) {
    if (tl == tr) return tl;
    int m = tl + (tr - tl) / 2, t = TR[TR[y].l].val - TR[TR[x].l].val;
    if (k < t) return select(TR[x].l, TR[y].l, tl, m, k);
    else return select(TR[x].r, TR[y].r, m + 1, tr, k - t);
  }
  int rank(int x, int y, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return 0;
    if (l <= tl && tr <= r) return TR[y].val - TR[x].val;
    int m = tl + (tr - tl) / 2; return rank(TR[x].l, TR[y].l, tl, m, l, r)
        + rank(TR[x].r, TR[y].r, m + 1, tr, l, r);
  }
  template <class It> WaveletTree(It st, It en)
      : N(en - st), curNode(0), ind(N), rnk(N), roots(N + 1), A(st, en),
        TR(N * (__lg(N * 2 - 1) + 3) - 1) {
    iota(ind.begin(), ind.end(), 0);
    stable_sort(ind.begin(), ind.end(), [&] (int i, int j) {
      return cmp(A[i], A[j]);
    });
    for (int i = 0; i < N; i++) rnk[ind[i]] = i;
    roots[0] = build(0, N - 1); for (int i = 0; i < N; i++)
      roots[i + 1] = update(roots[i], 0, N - 1, rnk[i]);
  }
  T select(int l, int r, int k) {
    return A[ind[select(roots[l], roots[r + 1], 0, N - 1, k)]];
  }
  int rank(int l, int r, T k) {
    int j = lower_bound(ind.begin(), ind.end(), N, [&] (int i, int) {
                  return cmp(A[i], k);
                }) - ind.begin() - 1;
    return j < 0 ? 0 : rank(roots[l], roots[r + 1], 0, N - 1, 0, j);
  }
#undef def
};
