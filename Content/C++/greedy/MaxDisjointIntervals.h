#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the maximum disjoint intervals problem
// Given a set of intervals in the form [L, R] which are sorted by R,
//   find the maximum number of disjoint intervals
// Maximum number of disjoint intervals is equivalent to the minimum number of
//   points to cover each interval (with the points being the right endpoints
//   of the disjoint intervals)
// Template Arguments:
//   It: the type of the iterator for the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the inclusive right bound of the interval
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   st: an iterator pointing to the first element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the inclusive right bound of the interval
//   en: an iterator pointing to after the last element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the inclusive right bound of the interval
//   cmp: an instance of the Cmp struct
// Return Value: an iterator to after the last disjoint interval after the
//   array is modified
// In practice, has a very small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/1141/problem/F2
//   https://oj.uz/problem/view/COCI21_planine
template <
    class It,
    class Cmp = less<typename iterator_traits<It>::value_type::first_type>>
It maxDisjointIntervals(It st, It en, Cmp cmp = Cmp()) {
  using Pair = typename iterator_traits<It>::value_type;
  assert(is_sorted(st, en, [&] (const Pair &a, const Pair &b) {
    return cmp(a.second, b.second);
  }));
  It cur = st; for (It l = st, r = st; l != en; l = r, cur++) {
    *cur = *l; for (r = l + 1; r != en && !cmp(cur->second, r->first); r++);
  }
  return cur;
}
