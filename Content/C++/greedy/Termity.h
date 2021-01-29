#pragma once
#include <bits/stdc++.h>
using namespace std;

// Two players alternate taking coins from either end of a row. If both players
//   play optimally, what is the value of player 1's total minus
//   player 2's total?
// The standard dp recurrence is
//   dp[l][r] = l > r ? 0 : max(A[l] - dp[l + 1][r], A[r] - dp[l][r - 1])
// This implementation is generalized to support any number of rows, possibly
//   with a fixed endpoint on the right
// The value of a single player can be recovered by solving the system of
//   equations: P1 + P2 = A, P1 - P2 = B
// Template Arguments:
//   T: the type of the value of each coin
// Function Arguments:
//   A: a vector of vectors of type T, with each vector representing a row
//   fixed: a vector of booleans of the same length of A representing whether
//     the right endpoint of that row is fixed
// Return Value: player 1's total minus player 2's total
// In practice, has a small constant
// Time Complexity: O(N log N) for N total coins or O(N) if there is a
//                  single row
// Memory Complexity: O(N) for N total coins
// Tested:
//   https://dmoj.ca/problem/ahardergame
//   https://atcoder.jp/contests/dp/tasks/dp_l
template <class T>
T termity(const vector<vector<T>> &A, const vector<bool> &fixed) {
  int N = 0; T ret = T(); for (auto &&a : A) N += a.size();
  int mult = N % 2 == 0 ? 1 : -1; vector<T> B; B.reserve(N);
  for (int i = 0; i < int(A.size()); i++) {
    int front = B.size(); for (auto &&a : A[i]) {
      B.push_back(a); while (int(B.size()) - front >= 3
          && B[B.size() - 3] <= B[B.size() - 2]
          && B[B.size() - 2] >= B[B.size() - 1]) {
        B[B.size() - 3] -= B[B.size() - 2] - B[B.size() - 1];
        B.pop_back(); B.pop_back();
      }
    }
    if (fixed[i]) while (int(B.size()) - front >= 2
        && B[B.size() - 2] <= B[B.size() - 1]) {
      ret += mult * (B[B.size() - 2] - B[B.size() - 1]);
      B.pop_back(); B.pop_back();
    }
  }
  if (int(A.size()) == 1) {
    for (int l = 0, r = int(B.size()) - 1, m = 1; l <= r; m *= -1) {
      if (B[l] >= B[r]) ret += m * B[l++];
      else ret += m * B[r--];
    }
  } else {
    sort(B.begin(), B.end(), greater<T>());
    for (int i = 0, m = 1; i < int(B.size()); i++, m *= -1) ret += m * B[i];
  }
  return ret;
}
