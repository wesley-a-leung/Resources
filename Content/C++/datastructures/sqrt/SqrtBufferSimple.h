#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in a multiset
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the values being stored
//   Cmp: the comparator to compare two f(x) values,
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   v: a vector of type T to initialize the structure
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
//   aboveVal(val): returns a pointer to the smallest element where
//     val compares less than that element, nullptr if it does not exist
//   ceilingVal(val): returns a pointer to the smallest element that
//     does not compare less than val, nullptr if it does not exist
//   floorVal(val): returns a pointer to the largest element where val does not
//     compare less than that element, nullptr if it does not exist
//   belowVal(val): returns a pointer to the largest element that
//     compares less than val, nullptr if it does not exist
//   above(val): returns a pair containing the index and a pointer to the
//     smallest element where val compares less than that element,
//     pointer is nullptr if it does not exist
//   ceiling(val): returns a pair containing the index and a pointer to the
//     smallest element that does not compare less than val,
//     pointer is nullptr if it does not exist
//   floor(val): returns a pair containing the index and a pointer to the
//     largest element where val does not compare less than that element,
//     pointer is nullptr if it does not exist
//   below(val): returns a pair containing the index and a pointer to the
//     largest element that compares less than val,
//     pointer is nullptr if it does not exist
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
//   floorInd, ceilingInd, aboveInd, belowInd: O(sqrt N) amortized
//   floorVal, ceilingVal, aboveVal, belowVal: O(sqrt N) amortized
//   floor, ceiling, above, below, contains, count: O(sqrt N) amortized
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
  SqrtBufferSimple(const vector<T> &v, double SCALE = 1)
      : SCALE(SCALE), large(v) {
    assert(is_sorted(v.begin(), v.end(), Cmp()));
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
  T *aboveVal(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = upper_bound(large.begin(), large.end(), val, Cmp());
    if (it != large.end()) y = &*it;
    for (auto &&x : small) if (Cmp()(val, x) && (!y || Cmp()(x, *y))) y = &x;
    return y;
  }
  T *ceilingVal(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = lower_bound(large.begin(), large.end(), val, Cmp());
    if (it != large.end()) y = &*it;
    for (auto &&x : small) if (!Cmp()(x, val) && (!y || Cmp()(x, *y))) y = &x;
    return y;
  }
  T *floorVal(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = upper_bound(large.begin(), large.end(), val, Cmp());
    if (it != large.begin()) y = &*--it;
    for (auto &&x : small) if (!Cmp()(val, x) && (!y || Cmp()(*y, x))) y = &x;
    return y;
  }
  T *belowVal(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = lower_bound(large.begin(), large.end(), val, Cmp());
    if (it != large.begin()) y = &*--it;
    for (auto &&x : small) if (Cmp()(x, val) && (!y || Cmp()(*y, x))) y = &x;
    return y;
  }
  pair<int, T *> above(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = upper_bound(large.begin(), large.end(), val, Cmp());
    int cnt = it - large.begin(); if (it != large.end()) y = &*it;
    for (auto &&x : small) {
      if (!Cmp()(val, x)) cnt++;
      else if (!y || Cmp()(x, *y)) y = &x;
    }
    return make_pair(cnt, y);
  }
  pair<int, T *> ceiling(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = lower_bound(large.begin(), large.end(), val, Cmp());
    int cnt = it - large.begin(); if (it != large.end()) y = &*it;
    for (auto &&x : small) {
      if (Cmp()(x, val)) cnt++;
      else if (!y || Cmp()(x, *y)) y = &x;
    }
    return make_pair(cnt, y);
  }
  pair<int, T *> floor(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = upper_bound(large.begin(), large.end(), val, Cmp());
    int cnt = it - large.begin(); if (it != large.begin()) y = &*--it;
    for (auto &&x : small) if (!Cmp()(val, x)) {
      cnt++; if (!y || Cmp()(*y, x)) y = &x;
    }
    return make_pair(--cnt, y);
  }
  pair<int, T *> below(const T &val) {
    rebuild(); T *y = nullptr;
    auto it = lower_bound(large.begin(), large.end(), val, Cmp());
    int cnt = it - large.begin(); if (it != large.begin()) y = &*--it;
    for (auto &&x : small) if (Cmp()(x, val)) {
      cnt++; if (!y || Cmp()(*y, x)) y = &x;
    }
    return make_pair(--cnt, y);
  }
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
