#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array that supports only insertions
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   empty, size: O(1)
//   rebuild: O(cbrt(N))
//   floor, ceiling, above, below, contains: O(cbrt(N) + log(N)) amortized
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct CbrtOrderMaintenance {
    Comparator cmp; double SCALE_FACTOR; vector<Value> small, medium, large;
    CbrtOrderMaintenance(const double SCALE_FACTOR = 1) : SCALE_FACTOR(SCALE_FACTOR) {}
    template <class It> CbrtOrderMaintenance(const It st, const It en, const double SCALE_FACTOR = 1) : SCALE_FACTOR(SCALE_FACTOR), large(st, en) {
        assert(is_sorted(st, en));
    }
    void rebuild() {
        double c = cbrt(small.size() + medium.size() + large.size());
        if (int(small.size()) > SCALE_FACTOR * c) {
            int mediumSz = int(medium.size()); sort(small.begin(), small.end(), cmp);
            for (auto &&x : small) medium.push_back(x);
            small.clear(); inplace_merge(medium.begin(), medium.begin() + mediumSz, medium.end(), cmp);
            if (int(medium.size()) > SCALE_FACTOR * c * c) {
                int largeSz = int(large.size()); sort(medium.begin(), medium.end(), cmp);
                for (auto &&x : medium) large.push_back(x);
                medium.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), cmp);
            }
        }
    }
    void insert(const Value &val) { small.push_back(val); }
    int aboveInd(const Value &val) {
        rebuild(); int ret = upper_bound(large.begin(), large.end(), val, cmp) - large.begin();
        ret += upper_bound(medium.begin(), medium.end(), val, cmp) - medium.begin();
        for (auto &&x : small) ret += !cmp(val, x);
        return ret;
    }
    int ceilingInd(const Value &val) {
        rebuild(); int ret = lower_bound(large.begin(), large.end(), val, cmp) - large.begin();
        ret += lower_bound(medium.begin(), medium.end(), val, cmp) - medium.begin();
        for (auto &&x : small) ret += cmp(x, val);
        return ret;
    }
    int floorInd(const Value &val) { return aboveInd(val) - 1; }
    int belowInd(const Value &val) { return ceilingInd(val) - 1; }
    bool contains(const Value &val) {
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        if (binary_search(medium.begin(), medium.end(), val, cmp)) return true;
        rebuild();
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        if (binary_search(medium.begin(), medium.end(), val, cmp)) return true;
        for (auto &&x : small) if (!cmp(val, x) && !cmp(x, val)) return true;
        return false;
    }
    int count(const Value &val) { return aboveInd(val) - ceilingInd(val); }
    bool empty() const { return small.empty() && medium.empty() && large.empty(); } 
    int size() const { return int(small.size() + medium.size() + large.size()); } 
    void clear() const { small.clear(); medium.clear(); large.clear(); }
    vector<Value> values() const { // not sorted
        vector<Value> ret;
        for (auto &&x : large) ret.push_back(x);
        for (auto &&x : medium) ret.push_back(x);
        for (auto &&x : small) ret.push_back(x);
        return ret;
    }
};
