#pragma once
#include <bits/stdc++.h>
using namespace std;

// Radix Priority Queue
// All new keys pushed to the queue must be less than the last call to top
// top and pop return the key-value pair with the maximum key
// Comparator convention is same as priority_queue in STL
// In practice, has a small constant
// Time Complexity:
//   top: O(B) where B is the number of bits in Key
//   empty, size: O(1)
//   pop, push: O(1) amortized
// Memory Complexity: O(NB)
// Tested:
//   https://judge.yosupo.jp/problem/shortest_path
template <class Key, class Val, class Comparator = less<Key>>
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
        if (Comparator()(last, x[i][j].first)) last = x[i][j].first;
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
