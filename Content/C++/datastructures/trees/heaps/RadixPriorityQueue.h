#pragma once
#include <bits/stdc++.h>
using namespace std;

// Radix Priority Queue
// Template Arguments:
//   Key: the type of the key, must be an unsigned integral type
//   Val: the type of the value
//   Cmp: the comparator to compare two values of type Key,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Functions:
//   empty(): returns whether the priority queue is empty
//   size(): returns the number of elements in the priority queue
//   top(): returns the largest element in the priority queue based on
//     the comparator
//   pop(): pops the largest element in the priority queue based on
//     the comparator, and returns that element
//   push(key, val): pushes the key value pair onto the priority queue,
//     key must compare less than the the last call to top
// In practice, has a small constant
// Time Complexity:
//   empty, size: O(1)
//   top: O(B) where B is the number of bits in Key
//   pop, push: O(1) amortized
// Memory Complexity: O(NB)
// Tested:
//   https://judge.yosupo.jp/problem/shortest_path
template <class Key, class Val, class Cmp = less<Key>>
struct RadixPriorityQueue {
  static_assert(is_integral<Key>::value, "Key must be integral");
  static_assert(is_unsigned<Key>::value, "Key must be unsigned");
  static constexpr int B = __lg(numeric_limits<Key>::max()) + 1;
  int N; Key last; vector<vector<pair<Key, Val>>> x;
  RadixPriorityQueue() : N(0), last(0), x(B + 1) {}
  int lg(Key a) const { return a ? __lg(a) : -1; }
  void aux(const pair<Key, Val> &p) { x[lg(p.first ^ last) + 1].push_back(p); }
  bool empty() const { return N == 0; }
  int size() const { return N; }
  pair<Key, Val> top() {
    if (x[0].empty()) {
      int i = 0; while (x[i].empty()) i++;
      last = x[i][0].first; for (int j = 1; j < int(x[i].size()); j++)
        if (Cmp()(last, x[i][j].first)) last = x[i][j].first;
      for (auto &&p : x[i]) aux(p);
      x[i].clear();
    }
    return x[0].back();
  }
  pair<Key, Val> pop() {
    pair<Key, Val> ret = top(); N--; x[0].pop_back(); return ret;
  }
  void push(Key key, const Val &val) { N++; aux(make_pair(key, val)); }
};
