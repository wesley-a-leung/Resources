#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include "RandomizedHash.h"
using namespace std;
using namespace __gnu_pbds;

// Policy-based data structures for ordered and unordered sets and maps

// Unordered hashmap with randomized hash
// API is similar to unordered_set and unordered_map in C++ 03,
//   except there is no count function (use member function find instead)
// In practice, has a large constant, but faster than unordered_set
//   and unordered_map (at the expense of using more memory)
// Time Complexity: O(1) on average, O(N) worst case
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/set
//   https://dmoj.ca/problem/fallingsnowflakes
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
template <class K, class H = rand_hash<K>, class ...Ts>
  using hashset = gp_hash_table<K, null_type, H, Ts ...>;
template <class K, class V, class H = rand_hash<K>, class ...Ts>
  using hashmap = gp_hash_table<K, V, H, Ts ...>;

// Ordered treeset and treemap
// API is similar to set and map in C++ 03
// There are additional functions to find the kth element with find_by_order(k)
//   and find the 0-indexed rank of a key with order_of_key(key)
// In practice, has a moderate constant, slower that set and map as well as
//   custom written balanced binary search trees
// Time Complexity: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds4
//   https://codeforces.com/contest/1093/problem/E
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=898
template <class K, class C = less<K>, class ...Ts>
  using treeset = tree<K, null_type, C, rb_tree_tag,
                       tree_order_statistics_node_update, Ts ...>;
template <class K, class V, class C = less<K>, class ...Ts>
  using treemap = tree<K, V, C, rb_tree_tag,
                       tree_order_statistics_node_update, Ts ...>;
