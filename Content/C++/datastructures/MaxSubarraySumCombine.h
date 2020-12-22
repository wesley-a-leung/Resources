#pragma once
#include <bits/stdc++.h>
using namespace std;

// Combine struct used for maximum non empty subarray
//   for Segment Trees, Implict Treaps, Link Cut Trees, etc
// Template Arguments:
//   T: the type of the element
// Functions:
//   makeData(v): returns a MaxSubarraySumCombine<T>::Data initialized with v
//   makeLazy(v): returns a MaxSubarraySumCombine<T>::Lazy initialized with v
// Time Complexity:
//   all functions: O(1)
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://www.spoj.com/problems/GSS1/
//   https://dmoj.ca/problem/dmpg17g2
//   https://mcpt.ca/problem/seq3
//   https://dmoj.ca/problem/acc1p1
//   https://dmoj.ca/problem/noi05p2
template <class T> struct MaxSubarraySumCombine {
  struct Data { T pre, suf, sum, maxSum; };
  using Lazy = pair<T, T>;
  static Data makeData(const T &v) {
    Data ret; ret.pre = ret.suf = ret.sum = ret.maxSum = v; return ret;
  }
  static Lazy makeLazy(const T &v) { return Lazy(v, v); }
  static Data qdef() {
    Data ret = makeData(numeric_limits<T>::lowest()); ret.sum = T();
    return ret;
  }
  static Lazy ldef() { return makeLazy(numeric_limits<T>::lowest()); }
  static Data merge(const Data &l, const Data &r) {
    if (l.maxSum == numeric_limits<T>::lowest()) return r;
    if (r.maxSum == numeric_limits<T>::lowest()) return l;
    Data ret; ret.pre = max(l.pre, l.sum + r.pre);
    ret.suf = max(l.suf + r.sum, r.suf); ret.sum = l.sum + r.sum;
    ret.maxSum = max(max(l.maxSum, r.maxSum), l.suf + r.pre); return ret;
  }
  static Data applyLazy(const Data &, const Lazy &r) {
    Data ret; ret.pre = ret.suf = ret.maxSum = max(r.first, r.second);
    ret.sum = r.first; return ret;
  }
  template <class IndexType>
  static Lazy getSegmentVal(const Lazy &v, IndexType k) {
    return Lazy(v.first * k, v.second);
  }
  static Lazy mergeLazy(const Lazy &, const Lazy &r) { return r; }
  static void revData(Data &v) { swap(v.pre, v.suf); }
  template <class IndexType> static Data getSegmentVdef(IndexType) {
    return makeData(IndexType());
  }
};
