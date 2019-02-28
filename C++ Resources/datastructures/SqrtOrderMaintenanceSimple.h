#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   rebuild: O(sqrt(N))
//   empty, size: O(1)
//   floor, ceiling, above, below, contains, count: O(sqrt(N) + log(N)) amortized
//   values: O(N)
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct SqrtOrderMaintenanceSimple {
    Comparator cmp; double SCALE_FACTOR; vector<Value> small, large;
    SqrtOrderMaintenanceSimple(const double SCALE_FACTOR = 1) : SCALE_FACTOR(SCALE_FACTOR) {}
    template <class It> SqrtOrderMaintenanceSimple(const It st, const It en, const double SCALE_FACTOR = 1) :
            SCALE_FACTOR(SCALE_FACTOR), large(st, en) {
        assert(is_sorted(st, en, cmp));
    }
    void rebuild() {
        if (int(small.size()) > SCALE_FACTOR * sqrt(small.size() + large.size())) {
            int largeSz = int(large.size()); sort(small.begin(), small.end(), cmp);
            for (auto &&x : small) large.push_back(x);
            small.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), cmp);
        }
    }
    void insert(const Value &val) { small.push_back(val); }
    int aboveInd(const Value &val) {
        rebuild(); int ret = upper_bound(large.begin(), large.end(), val, cmp) - large.begin();
        for (auto &&x : small) ret += !cmp(val, x);
        return ret;
    }
    int ceilingInd(const Value &val) {
        rebuild(); int ret = lower_bound(large.begin(), large.end(), val, cmp) - large.begin();
        for (auto &&x : small) ret += cmp(x, val);
        return ret;
    }
    int floorInd(const Value &val) { return aboveInd(val) - 1; }
    int belowInd(const Value &val) { return ceilingInd(val) - 1; }
    bool contains(const Value &val) {
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        rebuild();
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        for (auto &&x : small) if (!cmp(val, x) && !cmp(x, val)) return true;
        return false;
    }
    int count(const Value &val) { return aboveInd(val) - ceilingInd(val); }
    // number of values in the range [lo, hi]
    int count(const Value &lo, const Value &hi) { return aboveInd(hi) - ceilingInd(lo); }
    bool empty() const { return small.empty() && large.empty(); } 
    int size() const { return int(small.size() + large.size()); } 
    void clear() { small.clear(); large.clear(); }
    vector<Value> values() const { // sorted
        vector<Value> ret;
        for (auto &&x : small) ret.push_back(x);
        int mid = int(ret.size());
        for (auto &&x : large) ret.push_back(x);
        inplace_merge(ret.begin(), ret.begin() + mid, ret.end(), cmp); return ret;
    }
};
