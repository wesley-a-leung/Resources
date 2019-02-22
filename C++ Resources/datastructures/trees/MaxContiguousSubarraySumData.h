#pragma once
#include <bits/stdc++.h>
using namespace std;

// Data struct used for maximum non empty contiguous subarray for Segment Trees, Link Cut Trees, etc
template <class T> struct MaxContiguousSubarrayData { T pre, suf, sum, maxSum; bool isNull; };
template <class T> MaxContiguousSubarrayData<T> makeData(bool isNull = true, const T &v = 0) {
    MaxContiguousSubarrayData<T> ret; ret.pre = ret.suf = ret.sum = ret.maxSum = v; ret.isNull = isNull; return ret;
}
template <class T> MaxContiguousSubarrayData<T> merge(const MaxContiguousSubarrayData<T> &l, const MaxContiguousSubarrayData<T> &r) {
    if (l.isNull) return r;
    if (r.isNull) return l;
    MaxContiguousSubarrayData<T> ret; ret.isNull = false; ret.pre = max(l.pre, l.sum + r.pre); ret.suf = max(l.suf + r.sum, r.suf);
    ret.sum = l.sum + r.sum; ret.maxSum = max(max(l.maxSum, r.maxSum), l.suf + r.pre); return ret;
}
// range assignments
template <class T> MaxContiguousSubarrayData<T> applyLazy(const MaxContiguousSubarrayData<T> &l, const pair<T, T> &r) {
    MaxContiguousSubarrayData<T> ret; ret.pre = ret.suf = ret.maxSum = max(r.first, r.second); ret.sum = r.first; ret.isNull = false; return ret;
}
// getSegmentVal returns a pair containing the range value, and the actual value
template <class T> pair<T, T> getSegmentVal(const pair<T, T> &l, int len) { return make_pair(l.first * len, l.second); }
template <class T> pair<T, T> mergeLazy(const pair<T, T> &l, const pair<T, T> &r) { return r; }
