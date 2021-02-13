#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
using namespace std;

// Help struct to compare two pairs
struct EpsPairCmp {
  bool operator () (const pair<T, T> &a, const pair<T, T> &b) const {
    return eq(a.first, b.first) ? lt(a.second, b.second)
                                : lt(a.first, b.first);
  } 
};

// Adding and removing intervals from a set
// Functions:
//   addInterval(L, R): adds an interval [L, R) to the set
//   removeInterval(L, R): removes the interval [L, R) from the set
// In practice, has a moderate constant
// Time Complexity:
//   addInterval, removeInterval: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/art6
struct IntervalUnion : public set<pair<T, T>, EpsPairCmp> {
  set<pair<T, T>, EpsPairCmp>::iterator addInterval(T L, T R) {
    if (eq(L, R)) return end();
    auto it = lower_bound(make_pair(L, R)), before = it;
    while (it != end() && !lt(R, it->first)) {
      R = max(R, it->second); before = it = erase(it);
    }
    if (it != begin() && !lt((--it)->second, L)) {
      L = min(L, it->first); R = max(R, it->second); erase(it);
    }
    return emplace_hint(before, L, R);
  }
  void removeInterval(T L, T R) {
    if (eq(L, R)) return;
    auto it = addInterval(L, R); auto r2 = it->second;
    if (eq(it->first, L)) erase(it);
    else (T &) it->second = L;
    if (!eq(R, r2)) emplace(R, r2);
  }
};

// Given a set of sorted intervals (by the EpsPairCmp struct), combine
//   them into disjoint intervals of the form [L, R)
// Function Arguments:
//   st: an iterator pointing to the first element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the exclusive right bound of the interval
//   en: an iterator pointing to after the last element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the exclusive right bound of the interval
// Return Value: an iterator to right after the last disjoint interval
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/art6
template <class It> It intervalUnion(It st, It en) {
  assert(is_sorted(st, en, EpsPairCmp()));
  It cur = st; for (It l = st, r; l < en; l = r, cur++) {
    *cur = *l; for (r = l + 1; r < en && !lt(cur->second, r->first); r++)
      cur->second = max(cur->second, r->second);
  }
  return cur;
}
