#pragma once
#include <bits/stdc++.h>
#include "../../../search/BinarySearch.h"
using namespace std;

// Merge Sort Tree supporting select and rank operations for a subarray
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// In practice, has a small constant, faster than Wavelet Trees for
//   rank queries, but much slower for select queries
// Time Complexity:
//   constructor: O(N log N)
//   rank: O((log N)^2)
//   select: O((log N)^3)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.spoj.com/problems/KQUERY/ (rank)
//   https://www.spoj.com/problems/KQUERYO/ (rank)
//   https://codeforces.com/contest/1284/problem/D (rank)
//   https://www.spoj.com/problems/MKTHNUM/ (select)
//   https://judge.yosupo.jp/problem/range_kth_smallest (select)
template <class T, class Comparator = less<T>> struct MergeSortTree {
  Comparator cmp; int N; vector<T> sorted; vector<vector<T>> TR;
  template <class It> MergeSortTree(It st, It en)
      : N(en - st), sorted(st, en), TR(N * 2) {
    for (int i = 0; i < N; i++) TR[N + i] = vector<T>(1, sorted[i]);
    sort(sorted.begin(), sorted.end(), cmp); for (int i = N - 1; i > 0; i--) {
      TR[i].reserve(TR[i * 2].size() + TR[i * 2 + 1].size());
      merge(TR[i * 2].begin(), TR[i * 2].end(), TR[i * 2 + 1].begin(),
            TR[i * 2 + 1].end(), back_inserter(TR[i]), cmp);
    }
  }
  int rank(int l, int r, T k) {
    int ret = 0; for (l += N, r += N; l <= r; l /= 2, r /= 2) {
      if (l & 1) {
        ret += lower_bound(TR[l].begin(), TR[l].end(), k, cmp) - TR[l].begin();
        l++;
      }
      if (!(r & 1)) {
        ret += lower_bound(TR[r].begin(), TR[r].end(), k, cmp) - TR[r].begin();
        r--;
      }
    }
    return ret;
  }
  T select(int l, int r, int k) {
    return sorted[bsearch<Last>(0, N, [&] (int mid) {
      return rank(l, r, sorted[mid]) <= k;
    })];
  }
};
