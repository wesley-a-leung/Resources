#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a candidate for the majority element in a range [st, en)
//   It: the type of the iterator point to each element in the array
// The candidate is the majority element if and only if
//   std::count(st, en, boyerMooreMajority(st, en)) > (en - st) / 2
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Return Value: returns an iterator to an elmenet that is a candidate for the
//   majority element in the range [st, en)
// In practice, has a very small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/farmingmars
template <class It> It boyerMooreMajority(It st, It en) {
  It ret = st; int cnt = 0; for (It cur = st; cur != en; cur++) {
    if (cnt == 0) { ret = cur; cnt++; }
    else if (*cur == *ret) cnt++;
    else cnt--;
  }
  return ret;
}
