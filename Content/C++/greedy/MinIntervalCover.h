#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the minimum interval cover problem
// Given a set of intervals in the form [L, R], find the minimum number of
//   intervals to cover a target interval
// Range is modified in-place
// Template Arguments
//   T: the type of the endpoints of the intervals
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   T: the type of the points
// Function Arguments:
//   A: a reference to a vector of pairs with the first element being the
//     inclusive left bound of the interval and the second element being the
//     inclusive right bound of the interval
//   target: the target interval to cover with the first element being the
//     inclusive left bound of the target interval and the second element
//     being the inclusive right bound of the interval
//   cmp: an instance of the Cmp struct
// Return Value: a reference to the modified vector
// In practice, has a very small constant
// Time Complexity: O(N log N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/intervalcover
template <class T, class Cmp = less<T>>
vector<pair<T, T>> &minIntervalCover(vector<pair<T, T>> &A,
                                     pair<T, T> target, Cmp cmp = Cmp()) {
  sort(A.begin(), A.end(), [&] (const pair<T, T> &a, const pair<T, T> &b) {
    return cmp(a.first, b.first);
  });
  bool first = true; int i = 0, N = A.size();
  for (int j = 0; j < N && (first || cmp(target.first, target.second));) {
    if (cmp(target.first, A[j].first)) { A.clear(); return A; }
    else {
      for (A[i] = A[j]; j < N && !cmp(target.first, A[j].first); j++)
        if (cmp(A[i].second, A[j].second)) A[i] = A[j];
      target.first = A[i++].second; first = false;
    }
  }
  if (first || cmp(target.first, target.second)) i = 0;
  A.erase(A.begin() + i, A.end()); return A;
}
