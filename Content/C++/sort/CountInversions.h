#pragma once
#include <bits/stdc++.h>
using namespace std;

// Helper function for inversion counting
template <class InIt, class OutIt, class Cmp>
long long countInversions(InIt st1, OutIt st2, int N, Cmp cmp, int len) {
  long long ret = 0; for (int lo = 0; lo < N; lo += len + len) {
    int mid = min(lo + len, N), hi = min(mid + len, N);
    for (int i = lo, j = mid, k = lo; k < hi; k++) {
      if (i >= mid) st2[k] = st1[j++];
      else if (j >= hi) { st2[k] = st1[i++]; ret += j - mid; }
      else if (cmp(st1[j], st1[i])) { st2[k] = st1[j++]; ret += mid - i; }
      else { st2[k] = st1[i++]; ret += j - mid; }
    }
  }
  return ret;
}

// Counts inversions and sorts an array using merge sort with a comparator
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
// In practice, has a small constant, faster than the Fenwick Tree version
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://www.spoj.com/problems/INVCNT/
//   https://codeforces.com/problemsets/acmsguru/problem/99999/180
template <class It, class Cmp = less<typename iterator_traits<It>::value_type>>
long long countInversions(It st, It en, Cmp cmp = Cmp()) {
  vector<typename iterator_traits<It>::value_type> tmp(st, en);
  int N = en - st; auto aux = tmp.begin(); long long ret = 0; bool flag = true;
  for (int len = 1; len < N; len *= 2, flag = !flag)
    ret += flag ? countInversions(aux, st, N, cmp, len)
                : countInversions(st, aux, N, cmp, len);
  if (flag) copy_n(aux, N, st);
  return ret / 2;
}
