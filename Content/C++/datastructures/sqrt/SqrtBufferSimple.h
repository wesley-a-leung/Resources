#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in a multiset
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the values being stored
//   Cmp: the comparator to compare two f(x) values,
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   st: an iterator pointing to the first element in the sorted array of
//     initial values
//   en: an iterator pointing to after the last element in the sorted array of
//     initial values
//   SCALE: the value to scale sqrt by
// Functions:
//   rebuild(): rebuilds the multiset by moving all elements in the small
//     container to the large container
//   insert(val): inserts val into the multiset
//   aboveInd(val): returns the index of the smallest element where
//     val compares less than that element
//   ceilingInd(val): returns the index of the smallest element that
//     does not compare less than val
//   floorInd(val): returns the index of the largest element where
//     val does not compare less than that element
//   belowInd(val): returns the index of the largest element that
//     compares less than val
//   contains(val): returns whether val is in the multiset or not
//   count(lo, hi): returns the number of values in the range [lo, hi]
//   empty(): returns whether the multiset is empty or not
//   size(): returns the number of elements the multiset
//   clear(): clears the multiset
//   values(): returns a vector of the sorted values in the multiset
// In practice, has a very small constant, performs similarly to RootArray,
//   and is faster than balanced binary search trees
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   rebuild: O(sqrt N)
//   empty, size, clear: O(1)
//   floorInd, ceilingInd, aboveInd, belowInd, contains, count:
//     O(sqrt N) amortized
//   values: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/dmopc19c3p3
//   https://dmoj.ca/problem/ccc05s5
//   https://mcpt.ca/problem/lcc18c5s3
//   https://codeforces.com/contest/1093/problem/E
template <class T, class Cmp = less<T>> struct SqrtBufferSimple {
  double SCALE; vector<T> small, large;
  SqrtBufferSimple(double SCALE = 1) : SCALE(SCALE) {}
  template <class It>
  SqrtBufferSimple(const It st, const It en, double SCALE = 1)
      : SCALE(SCALE), large(st, en) {
    assert(is_sorted(st, en, Cmp()));
  }
  bool rebuild() {
    if (int(small.size()) > SCALE * sqrt(small.size() + large.size())) {
      int lSz = large.size(); sort(small.begin(), small.end(), Cmp());
      for (auto &&x : small) large.push_back(x);
      inplace_merge(large.begin(), large.begin() + lSz, large.end(), Cmp());
      small.clear(); return true;
    }
    return false;
  }
  void insert(const T &val) { small.push_back(val); }
  int aboveInd(const T &val) {
    rebuild();
    int ret = upper_bound(large.begin(), large.end(), val, Cmp())
        - large.begin();
    for (auto &&x : small) ret += !Cmp()(val, x);
    return ret;
  }
  int ceilingInd(const T &val) {
    rebuild();
    int ret = lower_bound(large.begin(), large.end(), val, Cmp())
        - large.begin();
    for (auto &&x : small) ret += Cmp()(x, val);
    return ret;
  }
  int floorInd(const T &val) { return aboveInd(val) - 1; }
  int belowInd(const T &val) { return ceilingInd(val) - 1; }
  bool contains(const T &val) {
    if (binary_search(large.begin(), large.end(), val, Cmp())) return true;
    if (rebuild() && binary_search(large.begin(), large.end(), val, Cmp()))
      return true;
    for (auto &&x : small) if (!Cmp()(val, x) && !Cmp()(x, val)) return true;
    return false;
  }
  int count(const T &lo, const T &hi) {
    rebuild();
    int ret = upper_bound(large.begin(), large.end(), hi, Cmp())
        - lower_bound(large.begin(), large.end(), lo, Cmp());
    for (auto &&x : small) ret += !Cmp()(x, lo) && !Cmp()(hi, x);
    return ret;
  }
  bool empty() const { return small.empty() && large.empty(); } 
  int size() const { return int(small.size() + large.size()); } 
  void clear() { small.clear(); large.clear(); }
  vector<T> values() const {
    vector<T> ret; ret.reserve(size());
    for (auto &&x : small) ret.push_back(x);
    int mid = int(ret.size()); for (auto &&x : large) ret.push_back(x);
    inplace_merge(ret.begin(), ret.begin() + mid, ret.end(), Cmp());
    return ret;
  }
};
