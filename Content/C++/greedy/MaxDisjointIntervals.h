#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the maximum disjoint intervals problem
// Given a set of intervals in the form [L, R], find the maximum number of
//   disjoint intervals
// Maximum number of disjoint intervals is equivalent to the minimum number of
//   points to cover each interval (with the points being the right endpoints
//   of the disjoint intervals)
// Range is modified in-place
// Template Arguments:
//   T: the type of the endpoints of the intervals
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   A: a reference to a vector of pairs with the first element being the
//     inclusive left bound of the interval and the second element being the
//     inclusive right bound of the interval
//   cmp: an instance of the Cmp struct
// Return Value: a reference to the modified vector
// In practice, has a very small constant
// Time Complexity: O(N log N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/1141/problem/F2
//   https://oj.uz/problem/view/COCI21_planine
template <class T, class Cmp = less<T>>
vector<pair<T, T>> &maxDisjointIntervals(vector<pair<T, T>> &A,
                                         Cmp cmp = Cmp()) {
  sort(A.begin(), A.end(), [&] (const pair<T, T> &a, const pair<T, T> &b) {
    return cmp(a.second, b.second);
  });
  int i = 0; for (int l = 0, r = 0, N = A.size(); l < N; l = r, i++) {
    A[i] = A[l]; for (r = l + 1; r < N && !cmp(A[i].second, A[r].first); r++);
  }
  A.erase(A.begin() + i, A.end()); return A;
}
