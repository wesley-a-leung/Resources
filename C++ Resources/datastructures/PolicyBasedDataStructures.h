#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include "RandomizedHash.h"
using namespace std;
using namespace __gnu_pbds;

// Policy-based data structures for ordered and unordered sets and maps,
//   as well as priority queues

// Unordered hashmap with randomized hash
// API is similar to unordered_set and unordered_map in C++ 03,
//   except there is no count function (use member function find instead)
// https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/gp_hash_table.html
//   and https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/container_base.html
//   contain most of the public methods
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
// https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/tree.html
//   https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/basic_tree.html
//   and https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/container_base.html
//   contain most of the public methods
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

// Priority Queue
// API is similar to std::priority_queue in C++ 03
// There are additional functions to modify an existing key with
//   PQ.modify(iter, key) where ptr is an iterator returned from push,
//   and merge heap B into A with A.join(B)
// https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/priority_queue.html
//   contains most of the public methods
// There are 5 different types of tags to specify the underlying data structure
//   of the heap, which can be seen at
//   https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/pq_design.html
// https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/pq_performance_tests.html
//   contains a summary of the time complexities of each tag
// On average, pairing_heap_tag performs the best out of all tags
// In practice, has a large constant, slower than std::priority_queue
//   join is only slightly faster than small to large merging when
//   amortization applies
// Time Complexity (for pairing_heap_tag):
//   push, join: O(1)
//   pop: O(log N)
//   modify, erase: O(log N) amortized, O(N) worst case
// Memory Complexity (for pairing_heap_tag): O(N)
// Tested:
//   https://dmoj.ca/problem/apio16p2
template <class T, class C = less<T>, class Tag = pairing_heap_tag>
using pbdsheap = __gnu_pbds::priority_queue<T, C, Tag>;
template <class T, class Tag = pairing_heap_tag>
using minpbdsheap = pbdsheap<T, greater<T>, Tag>;
template <class T, class Tag = pairing_heap_tag>
using maxpbdsheap = pbdsheap<T, less<T>, Tag>;
