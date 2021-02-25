#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fast Subset Transform (similar to SumOverSubsets)
// Template Arguments:
//   TYPE: the type of susbet transform (OR, AND, or XOR)
//   T: the type of each element
// Function Arguments:
//   a: a reference to the vector of type T to transform
//   inv: a boolean indicating whether the inverse transform should be
//     performed or not
// In practice, has a small constant
// Time Complexity: O(N log N) where N = size(a)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/subset_convolution
//   https://judge.yosupo.jp/problem/bitwise_and_convolution
//   https://judge.yosupo.jp/problem/bitwise_xor_convolution
//   https://csacademy.com/contest/round-53/task/maxor/
const int OR = 0, AND = 1, XOR = 2;
template <const int TYPE, class T> void fst(vector<T> &a, bool inv) {
  int N = a.size(); assert(!(N & (N - 1)));
  for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1)
    for (int j = 0; j < len; j++) {
      T &u = a[i + j], &v = a[len + i + j];
      if (TYPE == OR) tie(u, v) = inv ? make_pair(v, u - v)
                                      : make_pair(u + v, u);
      else if (TYPE == AND) tie(u, v) = inv ? make_pair(v - u, u)
                                            : make_pair(v, u + v);
      else if (TYPE == XOR) tie(u, v) = make_pair(u + v, u - v);
      else assert(false);
    }
  if (TYPE == XOR && inv) for (auto &&ai : a) ai /= T(N);
}
