#pragma once
#include <bits/stdc++.h>
using namespace std;

// Divide and Conquer Dynamic Programming Optimization
// Must satisfy dp[k][i] = max(dp[k - 1][j] + cost(k, j, i))
//   for 0 <= j <= i <= N and opt[k][i] <= opt[k][i + 1] where opt[k][i] is
//   the optimal value of j for dp[k][i]
// Template Arguments:
//   T: the type of the return value of the cost function
//   F: the type of the function used to compute the cost function
//   Cmp: the comparator to compare two T values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   K: the number of iterations, must be positive
//   N: the size of the array, must be positive
//   f(k, j, i): computes the cost function for the kth iteration (1-indexed)
//     for the transition from j to i (0 <= j <= i <= N)
//   cmp: an instance of the Cmp struct
// Return Value: a vector of length K + 1 with the value of dp[k][N]
//   for 1 <= k <= N with dp[0][N] = 0
// In practice, has a small constant
// Time Complexity: O(KN log N)
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/321/problem/E
//   https://www.acmicpc.net/problem/7052
template <class T, class F, class Cmp = less<T>>
vector<T> divideAndConquer(int K, int N, F f, Cmp cmp = Cmp()) {
  assert(N > 0 && K > 0); vector<vector<T>> dp(2, vector<T>(N + 1, T()));
  for (int i = 0; i <= N; i++) dp[1][i] = f(1, 0, i);
  vector<T> ret(K + 1, T()); ret[1] = dp[1][N]; vector<int> mid(N + 1);
  vector<pair<int, int>> bnd(N + 1, make_pair(0, N)), opt = bnd;
  for (int i = 0, b = 1; i <= N; i++) {
    int l, r; tie(l, r) = bnd[i]; int m = mid[i] = l + (r - l) / 2;
    if (l <= m - 1) bnd[b++] = make_pair(l, m - 1);
    if (m + 1 <= r) bnd[b++] = make_pair(m + 1, r);
  }
  for (int k = 2; k <= K; k++) {
    int cur = k % 2, prv = 1 - cur; for (int i = 0, b = 1; i <= N; i++) {
      int l, r, m = mid[i], optl, optr;
      tie(l, r) = bnd[i]; tie(optl, optr) = opt[i]; int optm = optl;
      for (int j = optl, en = min(m, optr); j <= en; j++) {
        T cost = dp[prv][j] + f(k, j, m);
        if (j == optl || cmp(dp[cur][m], cost)) {
          dp[cur][m] = cost; optm = j;
        }
      }
      if (l <= m - 1) opt[b++] = make_pair(optl, optm);
      if (m + 1 <= r) opt[b++] = make_pair(optm, optr);
    }
    ret[k] = dp[cur][N];
  }
  return ret;
}
