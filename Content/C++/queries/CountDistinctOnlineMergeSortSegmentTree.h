#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for a static array A of length N using
//   a merge sort segment tree
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a vector of type T of the values in the array
// Functions:
//   query(l, r): returns the number of distinct values in the range [l, r]
// In practice, has a small constant, slightly slower than the persistent
//   tree version, but uses less memory, slightly faster than the merge sort
//   fenwick tree version, but uses more memory
// Time Complexity:
//   constructor: O(N log N)
//   query: O((log N)^2)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.acmicpc.net/problem/14898
template <class T> struct CountDistinctOnlineMergeSortSegmentTree {
  int N; vector<vector<int>> TR;
  CountDistinctOnlineMergeSortSegmentTree(const vector<T> &A)
      : N(A.size()), TR(N * 2) {
    vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> last(temp.size(), -1); for (int i = 0; i < N; i++) {
      int c = lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
      TR[N + i] = vector<int>{last[c]}; last[c] = i;
    }
    for (int i = N - 1; i > 0; i--) {
      TR[i].reserve(TR[i * 2].size() + TR[i * 2 + 1].size());
      merge(TR[i * 2].begin(), TR[i * 2].end(), TR[i * 2 + 1].begin(),
            TR[i * 2 + 1].end(), back_inserter(TR[i]));
    }
  }
  int query(int l, int r) {
    int ret = 0, k = l; for (l += N, r += N; l <= r; l /= 2, r /= 2) {
      if (l & 1) {
        ret += lower_bound(TR[l].begin(), TR[l].end(), k) - TR[l].begin();
        l++;
      }
      if (!(r & 1)) {
        ret += lower_bound(TR[r].begin(), TR[r].end(), k) - TR[r].begin();
        r--;
      }
    }
    return ret;
  }
};
