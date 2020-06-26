#pragma once
#include <bits/stdc++.h>
using namespace std;

// Combine struct used for maximum non empty contiguous subarray
//   for Segment Trees, Implict Treaps, Link Cut Trees, etc
// Tested:
//   https://www.spoj.com/problems/GSS1/
//   https://dmoj.ca/problem/dmpg17g2
//   https://mcpt.ca/problem/seq3
//   https://dmoj.ca/problem/acc1p1
//   https://dmoj.ca/problem/noi05p2
template <class T> struct MaxContiguousSubarraySumCombine {
  struct Data { T pre, suf, sum, maxSum; };
  using Lazy = pair<T, T>;
  static Data makeData(const T &v) {
    Data ret; ret.pre = ret.suf = ret.sum = ret.maxSum = v; return ret;
  }
  static Lazy makeLazy(const T &v) { return Lazy(v, v); }
  const Data qdef = [&] () {
    Data ret = makeData(numeric_limits<T>::lowest()); ret.sum = T(); return ret;
  }();
  const Lazy ldef = makeLazy(numeric_limits<T>::lowest());
  Data merge(const Data &l, const Data &r) const {
    if (l.maxSum == numeric_limits<T>::lowest()) return r;
    if (r.maxSum == numeric_limits<T>::lowest()) return l;
    Data ret; ret.pre = max(l.pre, l.sum + r.pre);
    ret.suf = max(l.suf + r.sum, r.suf); ret.sum = l.sum + r.sum;
    ret.maxSum = max(max(l.maxSum, r.maxSum), l.suf + r.pre); return ret;
  }
  Data applyLazy(const Data &, const Lazy &r) const {
    Data ret; ret.pre = ret.suf = ret.maxSum = max(r.first, r.second);
    ret.sum = r.first; return ret;
  }
  template <class IndexType>
  Lazy getSegmentVal(const Lazy &v, IndexType k) const {
    return Lazy(v.first * k, v.second);
  }
  Lazy mergeLazy(const Lazy &, const Lazy &r) const { return r; }
  void revData(Data &v) const { swap(v.pre, v.suf); }
  template <class IndexType> Data getSegmentVdef(IndexType) const {
    return makeData(IndexType());
  }
};
