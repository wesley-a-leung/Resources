#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sum over subsets (or supersets) (similar to FST.h)
// Transforms an array A into in array A' such that A'[i] is equal to the sum
//   of all A[j] such that i | j == i if TYPE == SUBSET or i & j == i if
//   TYPE == SUPERSET
// Template Arguments:
//   TYPE: SUBSET or SUPERSET
//   T: the type of each element
// Function Arguments:
//   A: a reference to the vector of type T to transform
//   inv: a boolean indicating whether the inverse transform should be
//     performed or not
// Time Complexity: O(N 2^N) where 1 << N == size(A)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/bitwise_and_convolution
//   https://csacademy.com/contest/round-53/task/maxor/statement/
const bool SUBSET = true, SUPERSET = false;
template <const bool TYPE, class T> void sos(vector<T> &A, bool inv) {
  int S = A.size(), N = __lg(S); assert((1 << N) == S);
  for (int i = 0; i < N; i++) for (int mask = 0; mask < S; mask++)
    if (((mask >> i) & 1) == TYPE) {
      if (inv) A[mask] -= A[mask ^ (1 << i)];
      else A[mask] += A[mask ^ (1 << i)];
    }
}
