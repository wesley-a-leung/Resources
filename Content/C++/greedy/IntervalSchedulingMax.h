#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the interval scheduling maximization problem
// Given a set of intervals in the form [l, r) which are sorted by r,
//   find the maximum number of non overlapping intervals
// Template Arguments:
//   It: the type of the iterator for the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the exclusive right bound of the interval
//   Cmp: the comparator to compare two indices,
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   st: an iterator pointing to the first element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the exclusive right bound of the interval
//   en: an iterator pointing to after the last element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the exclusive right bound of the interval
//   cmp: an instance of the Cmp struct
// Return Value: an iterator to after the last disjoint interval after the
//   array is modified
// In practice, has a very small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/1141/problem/F2
template <
    class It,
    class Cmp = less<typename iterator_traits<It>::value_type::first_type>>
It intervalSchedulingMax(It st, It en, Cmp cmp = Cmp()) {
  using Pair = typename iterator_traits<It>::value_type;
  assert(is_sorted(st, en, [&] (const Pair &a, const Pair &b) {
    return cmp(a.second, b.second);
  }));
  It cur = st; for (It l = st, r = st; l != en; l = r, cur++) {
    *cur = *l; for (r = l + 1; r != en && cmp(r->first, cur->second); r++);
  }
  return cur;
}
