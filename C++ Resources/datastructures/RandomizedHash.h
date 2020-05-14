#pragma once
#include <bits/stdc++.h>
#include "../utils/Random.h"
using namespace std;

// Randomized hash
// Tested:
//   https://dmoj.ca/problem/set
//   https://dmoj.ca/problem/fallingsnowflakes
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
const size_t RANDOM = uniform_int_distribution<size_t>(0, numeric_limits<size_t>::max())(rng64);
template <class T, class H = hash<T>> struct rand_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator () (const T &x) const { return splitmix64(H()(x) + RANDOM); }
};

// Randomized hash for a pair
// Tested:
//   https://dmoj.ca/problem/fallingsnowflakes
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
template <class T1, class T2, class H1 = rand_hash<T1>, class H2 = rand_hash<T2>> struct pair_hash {
    size_t operator () (const pair<T1, T2> &p) const { return 31 * H1()(p.first) + H2()(p.second); }
};

// unordered_set and unordered_map with a randomized hash
// Tested:
//   https://dmoj.ca/problem/set
//   https://dmoj.ca/problem/fallingsnowflakes
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
template <class K, class H = rand_hash<K>, class ...Ts> using uset = unordered_set<K, H, Ts ...>;
template <class K, class V, class H = rand_hash<K>, class ...Ts> using umap = unordered_map<K, V, H, Ts ...>;
