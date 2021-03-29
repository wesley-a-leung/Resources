#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for a static array A of length N using
//   a merge sort fenwick tree
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a vector of type T of the values in the array
// Functions:
//   query(l, r): returns the number of distinct values in the range [l, r]
// In practice, has a small constant, slower than the persistent
//   tree version and slightly slower than the merge sort segment tree version,
//   but uses less memory
// Time Complexity:
//   constructor: O(N log N)
//   query: O((log N)^2)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.acmicpc.net/problem/14898
template <class T> struct CountDistinctOnlineMergeSortFenwickTree {
  int N; vector<vector<int>> BIT;
  int qry(int r, int v) {
    int ret = 0; for (r++; r > 0; r -= r & -r)
      ret += lower_bound(BIT[r].begin(), BIT[r].end(), v) - BIT[r].begin();
    return ret;
  }
  CountDistinctOnlineMergeSortFenwickTree(const vector<T> &A)
      : N(A.size()), BIT(N + 1) {
    vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> S(N + 1, 1), last(temp.size(), -1);
    for (int i = 1; i <= N; i++) {
      BIT[i].reserve(S[i]); int j = i + (i & -i); if (j <= N) S[j] += S[i];
    }
    for (int i = 1; i <= N; i++) {
      int c = lower_bound(temp.begin(), temp.end(), A[i - 1]) - temp.begin();
      auto it = lower_bound(BIT[i].begin(), BIT[i].end(), last[c]);
      BIT[i].insert(it, last[c]); int j = i + (i & -i); if (j <= N) {
        int k = BIT[j].size();
        BIT[j].insert(BIT[j].end(), BIT[i].begin(), BIT[i].end());
        inplace_merge(BIT[j].begin(), BIT[j].begin() + k, BIT[j].end());
      }
      last[c] = i - 1;
    }
  }
  int query(int l, int r) { return qry(r, l) - qry(l - 1, l); }
};
