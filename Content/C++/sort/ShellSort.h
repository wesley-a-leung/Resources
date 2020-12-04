#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using shell sort with a comparator
// Template Arguments:
//   It: the type of the iterator
//   Cmp: the comparator to compare two values
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   cmp: an instance of the Cmp struct
// In practice, has a very small constant
// Time Complexity: O(N^(4/3))
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/bf1hard
template <class It, class Cmp> void shellSort(It st, It en, Cmp cmp) {
  int n = en - st, h = 1; while (h < n * 4 / 9) h = h * 9 / 4 + 1;
  for (; h >= 1; h = h * 4 / 9) for (int i = h; i < n; i++)
    for (int j = i; j >= h && cmp(st[j], st[j - h]); j -= h)
      swap(st[j], st[j - h]);
}

// Sorts an array using shell sort with the default < operator
// Template Arguments:
//   It: the type of the iterator
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// In practice, has a very small constant
// Time Complexity: O(N^(4/3))
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/bf1hard
template <class It> void shellSort(It st, It en) {
  shellSort(st, en, less<typename decay<decltype(*st)>::type>());
}
