#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sum over subsets (or supersets) (similar to FST)
// Transforms an array a into an array a' such that a'[i] is equal to the
//   aggregate of all a[j] such that i | j == i if TYPE == SUBSET
//   or i & j == i if TYPE == SUPERSET
// Template Arguments:
//   TYPE: SUBSET or SUPERSET
//   T: the type of each element
// Function Arguments:
//   a: a reference to the vector of type T to transform
//   op: the op being performed (this can also be an inverse operation)
// Time Complexity: O(N log N) where N = size(a)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/bitwise_and_convolution
//   https://csacademy.com/contest/round-53/task/maxor/
const bool SUBSET = true, SUPERSET = false;
template <const bool TYPE, class T, class Op> void sos(vector<T> &a, Op op) {
  int N = a.size(); assert(!(N & (N - 1)));
  for (int m = 1; m < N; m <<= 1) for (int mask = 0; mask < N; mask++)
    if (bool(mask & m) == TYPE) a[mask] = op(a[mask], a[mask ^ m]);
}
