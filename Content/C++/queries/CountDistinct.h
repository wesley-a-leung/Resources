#pragma once
#include <bits/stdc++.h>
#include "../datastructures/trees/fenwicktrees/FenwickTree1D.h"
using namespace std;

// Supports offline queries for the number of distinct elements in the
//   range [l, r] for a static array A of length N
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a vector of type T of the values in the array
//   queries: a vector of pairs containing the inclusive endpoints of
//     the queries
// Fields:
//   ans: a vector of integers with the answer for each query
// In practice, has a small constant, faster than Mo
// Time Complexity:
//   constructor: O(N log N + Q (log Q + log N))
// Memory Complexity: O(N + Q)
// Tested:
//   https://www.spoj.com/problems/DQUERY/
//   https://atcoder.jp/contests/abc174/tasks/abc174_f
template <class T> struct CountDistinct {
  vector<int> ans;
  CountDistinct(const vector<T> &A, const vector<pair<int, int>> &queries)
      : ans(queries.size()) {
    int N = A.size(); vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> C(N), last(temp.size(), -1); for (int i = 0; i < N; i++)
      C[i] = lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
    vector<tuple<int, int, int>> q; int i = 0; FenwickTree1D<int> ft(N);
    for (auto &&qi : queries) q.emplace_back(qi.second, qi.first, i++);
    sort(q.begin(), q.end()); i = 0; for (auto &&qi : q) {
      int r, l, ind; tie(r, l, ind) = qi; while (i <= r) {
        int c = C[i]; if (last[c] != -1) ft.update(last[c], -1);
        ft.update(last[c] = i++, 1);
      }
      ans[ind] = ft.query(l, r);
    }
  }
};
