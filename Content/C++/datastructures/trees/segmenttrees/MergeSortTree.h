#pragma once
#include <bits/stdc++.h>
#include "../../../search/BinarySearch.h"
using namespace std;

// Merge Sort Tree supporting select and rank operations for a subarray
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
//   select(l, r, k): selects the kth element sorted by the comparator if the
//     range [l, r] was sorted
// In practice, has a moderate constant, faster than Wavelet Trees for
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
template <class T, class Cmp = less<T>> struct MergeSortTree {
  int N; vector<T> sorted; vector<vector<T>> TR;
  template <class F> MergeSortTree(int N, F f) : N(N), TR(N * 2) {
    sorted.reserve(N); for (int i = 0; i < N; i++) {
      sorted.push_back(f()); TR[N + i] = vector<T>(1, sorted.back());
    }
    sort(sorted.begin(), sorted.end(), Cmp());
    for (int i = N - 1; i > 0; i--) {
      TR[i].reserve(TR[i * 2].size() + TR[i * 2 + 1].size());
      merge(TR[i * 2].begin(), TR[i * 2].end(), TR[i * 2 + 1].begin(),
            TR[i * 2 + 1].end(), back_inserter(TR[i]), Cmp());
    }
  }
  template <class It> MergeSortTree(It st, It en)
      : MergeSortTree(en - st, [&] { return *st++; }) {}
  int rank(int l, int r, T k) {
    int ret = 0; for (l += N, r += N; l <= r; l /= 2, r /= 2) {
      if (l & 1) {
        ret += lower_bound(TR[l].begin(), TR[l].end(), k, Cmp())
            - TR[l].begin();
        l++;
      }
      if (!(r & 1)) {
        ret += lower_bound(TR[r].begin(), TR[r].end(), k, Cmp())
            - TR[r].begin();
        r--;
      }
    }
    return ret;
  }
  T select(int l, int r, int k) {
    return sorted[bsearch<LAST>(0, N, [&] (int mid) {
      return rank(l, r, sorted[mid]) <= k;
    })];
  }
};
