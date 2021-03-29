#pragma once
#include <bits/stdc++.h>
#include "../datastructures/trees/fenwicktrees/FenwickTree1D.h"
using namespace std;

// Counts inversions and sorts an array using a Fenwick Tree with a comparator
// Template Arguments:
//   It: the type of the iterator
//   Cmp: the comparator to compare two values
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   cmp: an instance of the Cmp struct
// Return Value: the number of inversions in the array
// In practice, has a moderate constant, slower than the merge sort version
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://www.spoj.com/problems/INVCNT/
//   https://codeforces.com/problemsets/acmsguru/problem/99999/180
template <class It, class Cmp = less<typename iterator_traits<It>::value_type>>
long long countInversionsFenwick(It st, It en, Cmp cmp = Cmp()) {
  vector<typename iterator_traits<It>::value_type> tmp(st, en);
  int N = en - st; long long ret = 0; sort(tmp.begin(), tmp.end(), cmp);
  tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
  FenwickTree1D<int> FT(tmp.size()); for (int i = N - 1; i >= 0; i--) {
    int c = lower_bound(tmp.begin(), tmp.end(), st[i], cmp) - tmp.begin();
    ret += FT.query(c - 1); FT.update(c, 1);
  }
  return ret;
}
