#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the minimum interval cover problem
// Given a set of intervals in the form [L, R] which are sorted by L,
//   find the minimum number of intervals to cover a target interval
// Assumes range is sorted, similar to std::unique
// Template Arguments
//   It: the type of the iterator for the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the inclusive right bound of the interval
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   T: the type of the points
// Function Arguments:
//   st: an iterator pointing to the first element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the inclusive right bound of the interval
//   en: an iterator pointing to after the last element in the array of pairs
//     with the first element being the inclusive left bound of the interval
//     and the second element being the inclusive right bound of the interval
//   target: the target interval to cover with the first element being the
//     inclusive left bound of the target interval and the second element
//     being the inclusive right bound of the interval
//   cmp: an instance of the Cmp struct
// Return Value: an iterator to after the last interval in the minimum
//   cover after the array is modified, or st if no cover exists
// In practice, has a very small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/intervalcover
template <
    class It,
    class Cmp = less<typename iterator_traits<It>::value_type::first_type>,
    class T = typename iterator_traits<It>::value_type::first_type>
It minIntervalCover(It st, It en, pair<T, T> target, Cmp cmp = Cmp()) {
  using Pair = typename iterator_traits<It>::value_type;
  assert(is_sorted(st, en, [&] (const Pair &a, const Pair &b) {
    return cmp(a.first, b.first);
  }));
  bool first = true; It cur = st;
  for (It i = st; i != en && (first || cmp(target.first, target.second));) {
    if (cmp(target.first, i->first)) return st;
    else {
      for (*cur = *i; i != en && !cmp(target.first, i->first); i++)
        if (cmp(cur->second, i->second)) *cur = *i;
      target.first = cur++->second; first = false;
    }
  }
  return first || cmp(target.first, target.second) ? st : cur;
}
