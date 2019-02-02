#pragma once
#include <bits/stdc++.h>
using namespace std;

// Data struct used for maximum contiguous subarray for Segment Trees, Link Cut Trees, etc
template <class T> struct MaxContiguousSubarrayData { T pre, suf, sum, maxSum; bool isNull; };
template <class T> MaxContiguousSubarrayData<T> nullNode() {
    MaxContiguousSubarrayData<T> ret; ret.pre = ret.suf = ret.sum = ret.maxSum = 0; ret.isNull = true; return ret;
}
template <class T> MaxContiguousSubarrayData<T> merge(const MaxContiguousSubarrayData<T> &l, const MaxContiguousSubarrayData<T> &r) {
    if (l.isNull) return r;
    if (r.isNull) return l;
    MaxContiguousSubarrayData<T> ret; ret.isNull = false; ret.pre = max(l.pre, l.sum + r.pre); ret.suf = max(l.suf + r.sum, r.suf);
    ret.sum = l.sum + r.sum; ret.maxSum = max(max(l.maxSum, r.maxSum), l.suf + r.pre); return ret;
}
// range assignments
template <class T> MaxContiguousSubarrayData<T> applyLazy(const MaxContiguousSubarrayData<T> &x, const T &v) {
    MaxContiguousSubarrayData<T> ret; ret.pre = ret.suf = ret.sum = ret.maxSum = v; ret.isNull = false; return ret;
}
template <class T> T getSegmentVal(const T &l, int len) { return l * len; }
template <class T> T mergeLazy(const T &l, const T &r) { return r; }
