#pragma once
#include <bits/stdc++.h>
using namespace std;

// Wavelet Tree (using a persistent segment tree)
//   supporting rank and select operations for a subarray
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the element of the array
//   Cmp: the comparator to compare two elements
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   rank(l, r, k): returns the number of elements less than k (using the
//     comparator) in the range [l, r]
//   count(l, r, lo, hi) returns the number of elements not less than lo and
//     not greater than hi (using the comparator) in the range [l, r]
//   select(l, r, k): selects the kth element sorted by the comparator if the
//     range [l, r] was sorted
// In practice, has a moderate constant, slightly faster than MergeSortTree for
//   rank and count queries, much faster for select queries,
//   but uses more memory
// Time Complexity:
//   constructor, rank, count select: O(N log N)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.spoj.com/problems/KQUERY/ (rank/count)
//   https://www.spoj.com/problems/KQUERYO/ (rank/count)
//   https://codeforces.com/contest/1284/problem/D (rank/count)
//   https://www.spoj.com/problems/MKTHNUM/ (select)
//   https://judge.yosupo.jp/problem/range_kth_smallest (select)
template <class T, class Cmp = less<T>> struct WaveletTree {
  struct Node { int l, r, val; Node() : l(-1), r(-1), val(0) {} };
  int N; vector<int> ind, rnk, roots; vector<T> A; vector<Node> TR;
  int update(int x, int tl, int tr, int i) {
    TR.push_back(~x ? TR[x] : Node()); TR[x = int(TR.size()) - 1].val++;
    if (tl == tr) return x;
    int m = tl + (tr - tl) / 2;
    if (i <= m) TR[x].l = update(TR[x].l, tl, m, i);
    else TR[x].r = update(TR[x].r, m + 1, tr, i);
    return x;
  }
  int count(int x, int y, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return 0;
    if (l <= tl && tr <= r) return (~y ? TR[y].val : 0) - (~x ? TR[x].val : 0);
    int m = tl + (tr - tl) / 2;
    return count(~x ? TR[x].l : x, ~y ? TR[y].l : y, tl, m, l, r)
        + count(~x ? TR[x].r : x, ~y ? TR[y].r : y, m + 1, tr, l, r);
  }
  int select(int x, int y, int tl, int tr, int k) {
    if (tl == tr) return tl;
    int m = tl + (tr - tl) / 2; int t = (~y && ~TR[y].l ? TR[TR[y].l].val : 0)
        - (~x && ~TR[x].l ? TR[TR[x].l].val : 0);
    if (k < t) return select(~x ? TR[x].l : x, ~y ? TR[y].l : y, tl, m, k);
    else return select(~x ? TR[x].r : x, ~y ? TR[y].r : y, m + 1, tr, k - t);
  }
  template <class F> WaveletTree(int N, F f)
      : N(N), ind(N), rnk(N), roots(N + 1) {
    TR.reserve(N == 0 ? 0 : N * __lg(N * 4 - 1)); A.reserve(N);
    for (int i = 0; i < N; i++) A.push_back(f());
    iota(ind.begin(), ind.end(), 0);
    stable_sort(ind.begin(), ind.end(), [&] (int i, int j) {
      return Cmp()(A[i], A[j]);
    });
    for (int i = 0; i < N; i++) rnk[ind[i]] = i;
    roots[0] = -1; for (int i = 0; i < N; i++)
      roots[i + 1] = update(roots[i], 0, N - 1, rnk[i]);
  }
  template <class It> WaveletTree(It st, It en)
      : WaveletTree(en - st, [&] { return *st++; }) {}
  int rank(int l, int r, T v) {
    int j = lower_bound(ind.begin(), ind.end(), N, [&] (int i, int) {
                          return Cmp()(A[i], v);
                        }) - ind.begin() - 1;
    return j < 0 ? 0 : count(roots[l], roots[r + 1], 0, N - 1, 0, j);
  }
  int count(int l, int r, T lo, T hi) {
    int a = lower_bound(ind.begin(), ind.end(), N, [&] (int i, int) {
                          return Cmp()(A[i], lo);
                        }) - ind.begin();
    int b = upper_bound(ind.begin(), ind.end(), N, [&] (int, int i) {
                          return Cmp()(hi, A[i]);
                        }) - ind.begin() - 1;
    return a > b ? 0 : count(roots[l], roots[r + 1], 0, N - 1, a, b);
  }
  T select(int l, int r, int k) {
    return A[ind[select(roots[l], roots[r + 1], 0, N - 1, k)]];
  }
};
