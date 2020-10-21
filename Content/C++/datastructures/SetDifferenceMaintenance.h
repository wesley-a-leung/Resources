#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the difference between adjacent elements in a sorted multiset
// Template Arguments:
//   T: the type of the elements being stored
// Fields:
//   vals: a mapping from the values in the multiset to its frequency
//   diffs: a mapping from the difference of each pair of adjacent elements in
//     the multiset to its frequency
// Functions:
//   insert(v): inserts the value v into the multiset
//   erase(v); erases the value of v from the multiset assuming v exists
// In practice, has a moderate constant
// Time Complexity:
//   insert, erase: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1418/problem/D
//   https://dmoj.ca/problem/coci19c3p3
//   https://dmoj.ca/problem/year2018p6
template <class T> struct SetDifferenceMaintenance {
  map<T, int> vals, diffs;
  void insert(T v) {
    auto nxt = vals.lower_bound(v); if (nxt != vals.end() && nxt->first == v) {
      ++nxt->second; ++diffs[T()]; return;
    }
    if (nxt != vals.begin()) {
      auto prv = prev(nxt); diffs[v - prv->first]++; if (nxt != vals.end()) {
        auto it = diffs.find(nxt->first - prv->first);
        if (--it->second == 0) diffs.erase(it);
      }
    }
    if (nxt != vals.end()) ++diffs[nxt->first - v];
    vals.emplace_hint(nxt, v, 1);
  }
  void erase(T v) {
    auto cur = vals.find(v); if (cur->second >= 2) {
      --cur->second; auto it = diffs.find(T());
      if (--it->second == 0) diffs.erase(it);
      return;
    }
    auto nxt = vals.erase(cur); if (nxt != vals.end()) {
      auto it = diffs.find(nxt->first - v);
      if (--it->second == 0) diffs.erase(it);
    }
    if (nxt != vals.begin()) {
      auto prv = prev(nxt); auto it = diffs.find(v - prv->first);
      if (--it->second == 0) diffs.erase(it);
      if (nxt != vals.end()) ++diffs[nxt->first - prv->first];
    }
  }
};
