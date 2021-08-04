#pragma once
#include <bits/stdc++.h>
using namespace std;

// A map that supports insertions of a key value pair (k, v) and
//   queries for the maximum value (based on VCmp) of v for
//   all keys less than or equal to (based on KCmp) k
// Template Arguments:
//   K: the type of the key
//   V: the type of the value
//   KCmp: the comparator for the key, convention is the same as
//       std::map in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   VCmp: the comparator for the value, convention is the same as
//       std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   NEG_INF: a value for negative infinity
// Functions:
//   add(k, v): adds the key value pair (k, v) and maintains the pareto
//     optimums
//   queryPrefix(k): returns the maximum value (based on VCmp) of v for
//     all keys less than or equal to (based on KCmp) k, or NEG_INF if no such
//     keys exist
//   queryProperPrefix(k): returns the maximum value (based on VCmp) of v for
//     all keys less than or equal to (based on KCmp) k, or NEG_INF if no such
//     keys exist
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1)
//   add: O(log N) amortized for N elements in the pareto optimum
//   queryPrefix, queryProperPrefix: O(N) for N elements in the pareto optimum
// Memory Complexity: O(N) for N elements in the pareto optimum
// Tested:
//   https://dmoj.ca/problem/dpq
template <class K, class V, class KCmp = less<K>, class VCmp = less<V>>
struct ParetoMap : public map<K, V, KCmp> {
  using M = map<K, V, KCmp>; V NEG_INF;
  ParetoMap(V NEG_INF = numeric_limits<V>::lowest()) : NEG_INF(NEG_INF) {}
  void add(K k, V v) {
    auto it = M::lower_bound(k);
    if (it != M::begin() && !VCmp()(prev(it)->second, v)) return;
    while (it != M::end() && !VCmp()(v, it->second)) it = M::erase(it);
    M::emplace_hint(it, k, v);
  }
  V queryPrefix(K k) const {
    auto it = M::upper_bound(k);
    return it == M::begin() ? NEG_INF : prev(it)->second;
  }
  V queryProperPrefix(K k) const {
    auto it = M::lower_bound(k);
    return it == M::begin() ? NEG_INF : prev(it)->second;
  }
};
