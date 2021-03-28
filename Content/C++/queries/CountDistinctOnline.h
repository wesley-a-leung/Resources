#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for a static array A of length N
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a vector of type T of the values in the array
// Functions:
//   query(l, r): returns the number of distinct values in the range [l, r]
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N log N)
//   query: O(log N)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.acmicpc.net/problem/14898
template <class T> struct CountDistinctOnline {
  struct Node { int l, r, val; Node() : l(-1), r(-1), val(0) {} };
  int N; vector<int> roots; vector<Node> TR;
  int update(int x, int tl, int tr, int i) {
    TR.push_back(~x ? TR[x] : Node()); TR[x = int(TR.size()) - 1].val++;
    int m = tl + (tr - tl) / 2; if (i == m) return x;
    if (i < m) TR[x].l = update(TR[x].l, tl, m - 1, i);
    else TR[x].r = update(TR[x].r, m + 1, tr, i);
    return x;
  }
  int query(int x, int tl, int tr, int l, int r) {
    if (!~x || r < tl || tr < l) return 0;
    if (l <= tl && tr <= r) return TR[x].val;
    int m = tl + (tr - tl) / 2, ret = 0;
    if (l <= m && m <= r) ret += TR[x].val - (~TR[x].l ? TR[TR[x].l].val : 0)
        - (~TR[x].r ? TR[TR[x].r].val : 0);
    return ret + query(TR[x].l, tl, m - 1, l, r)
        + query(TR[x].r, m + 1, tr, l, r);
  }
  CountDistinctOnline(const vector<T> &A) : N(A.size()), roots(N) {
    TR.reserve(1 + (N + 1) * __lg(N * 2 + 1) - (1 << __lg(N * 2 + 1)));
    vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> C(N), last(temp.size(), -1); for (int i = 0; i < N; i++)
      C[i] = lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
    for (int i = 0, rt = -1; i < N; i++) {
      if (last[C[i]] != -1) rt = update(rt, 0, N - 1, last[C[i]]);
      roots[last[C[i]] = i] = rt;
    }
  }
  int query(int l, int r) {
    return r - l + 1 - query(roots[r], 0, N - 1, l, r);
  }
};
