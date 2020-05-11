#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array
// 0-indexed ranks
// In practice, performs very fast, and is faster than RootArray and balanced binary search trees
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   rebuild: O(sqrt(N))
//   empty, size, clear: O(1)
//   floorInd, ceilingInd, aboveInd, belowInd, contains, count: O(sqrt(N)) amortized
//   values: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/dmopc19c3p3
//   https://dmoj.ca/problem/ccc05s5
//   https://mcpt.ca/problem/lcc18c5s3
//   https://codeforces.com/contest/1093/problem/E
template <class T, class Comparator = less<T>> struct SqrtBufferSimple {
    Comparator cmp; double SCALE; vector<T> small, large; SqrtBufferSimple(double SCALE = 1) : SCALE(SCALE) {}
    template <class It> SqrtBufferSimple(const It st, const It en, double SCALE = 1) : SCALE(SCALE), large(st, en) {
        assert(is_sorted(st, en, cmp));
    }
    bool rebuild() {
        if (int(small.size()) > SCALE * sqrt(small.size() + large.size())) {
            int largeSz = int(large.size()); sort(small.begin(), small.end(), cmp);
            for (auto &&x : small) large.push_back(x);
            small.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), cmp); return true;
        }
        return false;
    }
    void insert(const T &val) { small.push_back(val); }
    int aboveInd(const T &val) {
        rebuild(); int ret = upper_bound(large.begin(), large.end(), val, cmp) - large.begin();
        for (auto &&x : small) ret += !cmp(val, x);
        return ret;
    }
    int ceilingInd(const T &val) {
        rebuild(); int ret = lower_bound(large.begin(), large.end(), val, cmp) - large.begin();
        for (auto &&x : small) ret += cmp(x, val);
        return ret;
    }
    int floorInd(const T &val) { return aboveInd(val) - 1; }
    int belowInd(const T &val) { return ceilingInd(val) - 1; }
    bool contains(const T &val) {
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        if (rebuild() && binary_search(large.begin(), large.end(), val, cmp)) return true;
        for (auto &&x : small) if (!cmp(val, x) && !cmp(x, val)) return true;
        return false;
    }
    int count(const T &lo, const T &hi) { // number of values in the range [lo, hi]
        rebuild(); int ret = upper_bound(large.begin(), large.end(), hi, cmp) - lower_bound(large.begin(), large.end(), lo, cmp);
        for (auto &&x : small) ret += !cmp(x, lo) && !cmp(hi, x);
        return ret;
    }
    bool empty() const { return small.empty() && large.empty(); } 
    int size() const { return int(small.size() + large.size()); } 
    void clear() { small.clear(); large.clear(); }
    vector<T> values() const { // sorted
        vector<T> ret; ret.reserve(size());
        for (auto &&x : small) ret.push_back(x);
        int mid = int(ret.size());
        for (auto &&x : large) ret.push_back(x);
        inplace_merge(ret.begin(), ret.begin() + mid, ret.end(), cmp); return ret;
    }
};
