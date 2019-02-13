#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   empty, size: O(1)
//   floor, ceiling, above, below, contains: O(log(N) + sqrt(N)) amortized
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct SqrtOrderMaintenance {
    Comparator cmp; double SCALE_FACTOR; vector<Value> small, large;
    SqrtOrderMaintenance(const double SCALE_FACTOR = 1) : SCALE_FACTOR(SCALE_FACTOR) {}
    template <class It> SqrtOrderMaintenance(const It st, const It en, const double SCALE_FACTOR = 1) : large(st, en), SCALE_FACTOR(SCALE_FACTOR) {}
    void resize() {
        if (int(small.size()) > SCALE_FACTOR * sqrt(small.size() + large.size())) {
            int largeSz = int(large.size()); sort(small.begin(), small.end(), cmp);
            for (auto &&x : small) large.push_back(x);
            small.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), cmp);
        }
    }
    void insert(const Value &val) { small.push_back(val); }
    int aboveInd(const Value &val) {
        resize(); int ret = upper_bound(large.begin(), large.end(), val, cmp) - large.begin();
        for (auto &&x : small) ret += !cmp(val, x);
        return ret;
    }
    int ceilingInd(const Value &val) {
        resize(); int ret = lower_bound(large.begin(), large.end(), val, cmp) - large.begin();
        for (auto &&x : small) ret += cmp(x, val);
        return ret;
    }
    int floorInd(const Value &val) { return aboveInd(val) - 1; }
    int belowInd(const Value &val) { return ceilingInd(val) - 1; }
    bool contains(const Value &val) {
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        resize();
        if (binary_search(large.begin(), large.end(), val, cmp)) return true;
        for (auto &&x : small) if (!cmp(val, x) && !cmp(x, val)) return true;
        return false;
    }
    bool empty() const { return small.empty() && large.empty(); } 
    int size() const { return int(small.size() + large.size()); } 
    void clear() const { small.clear(); large.clear(); }
};
