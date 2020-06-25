#pragma once
#include <bits/stdc++.h>
#include "../utils/Random.h"
using namespace std;

const size_t RANDOM = uniform_int_distribution<size_t>(
    0, numeric_limits<size_t>::max())(rng64);

// Randomized hash for types with std::hash defined and for pairs
// Tested:
//   https://dmoj.ca/problem/set
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
//   https://judge.yosupo.jp/problem/associative_array
struct rand_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb; return x ^ (x >> 31);
  }
  template <class T> size_t operator () (const T &x) const {
    return splitmix64(hash<T>()(x) + RANDOM);
  }
  template <class T1, class T2>
  size_t operator () (const pair<T1, T2> &p) const {
    return 31 * operator ()(p.first) + operator ()(p.second);
  }
};

// unordered_set and unordered_map with a randomized hash
// In practice, has a moderate constant, but still slower than
//   pbds::gp_hash_table
// Tested:
//   https://dmoj.ca/problem/set
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
//   https://judge.yosupo.jp/problem/associative_array
template <class K, class H = rand_hash, class ...Ts>
using uset = unordered_set<K, H, Ts ...>;
template <class K, class V, class H = rand_hash, class ...Ts>
using umap = unordered_map<K, V, H, Ts ...>;
