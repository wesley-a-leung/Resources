#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given a set of Q queries, for each query q, find the first value x in the range
// [lo, hi] such that f(q, x) is true
// Time Complexity:
//   If f(q, x) takes time proportional to x to compute, and f(q, x + 1) can be compute from f(q, x)
//   then the naive complexity would be O(Q * log(hi) * (time to compute f(q, hi)))
//   for Q independent binary searches, or O(Q * (hi - lo) * (time to compute f(q, x + 1) from f(q, x)))
//   Parallel Binary Search reduces this runtime to:
//   O(Q * log(hi - lo) * (time to compute f(q, x + 1) from f(q, x)) + Q log Q * (time to compute f(q, x) if precomputed))
template <const int MAXQ, const bool ONE_INDEXED> struct ParallelBinarySearch {
    struct Query { int ans; } queries[MAXQ]; // to be completed
    void add(int l, int r); // to be implemented
    void rem(int l, int r); // to be implemented
    bool f(int i); // to be impemented
    void solve(vector<int> &qs, int l, int r) {
        if (qs.empty()) return;
        if (l == r) { for (int i: qs) queries[i].ans = l; return; }
        int m = l + (r - l) / 2; vector<int> left, right; add(l, m);
        for (int i : qs) {
            if (f(i)) left.push_back(i);
            else right.push_back(i);
        }
        solve(right, m + 1, r); rem(l, m); solve(left, l, m);
    }
    void solve(int Q, int lo, int hi) { vector<int> qs(Q); iota(qs.begin(), qs.end(), int(ONE_INDEXED)); solve(qs, lo, hi); }
    void clear() { queries.clear(); }
};
