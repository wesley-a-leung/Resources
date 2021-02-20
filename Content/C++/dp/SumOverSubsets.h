#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sum over subsets (or supersets) (similar to FST.h)
// Transforms an array a into an array a' such that a'[i] is equal to the sum
//   of all a[j] such that i | j == i if TYPE == SUBSET or i & j == i if
//   TYPE == SUPERSET
// Template Arguments:
//   TYPE: SUBSET or SUPERSET
//   T: the type of each element
// Function Arguments:
//   a: a reference to the vector of type T to transform
//   inv: a boolean indicating whether the inverse transform should be
//     performed or not
// Time Complexity: O(N log N) where N = size(a)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/bitwise_and_convolution
//   https://csacademy.com/contest/round-53/task/maxor/
const bool SUBSET = true, SUPERSET = false;
template <const bool TYPE, class T> void sos(vector<T> &a, bool inv) {
  int N = a.size(); assert(!(N & (N - 1))); for (int m = 1; m < N; m <<= 1)
    for (int mask = 0; mask < N; mask++) if (bool(mask & m) == TYPE) {
      if (inv) a[mask] -= a[mask ^ m];
      else a[mask] += a[mask ^ m];
    }
}
