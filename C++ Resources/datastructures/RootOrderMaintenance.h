#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array that supports only insertions
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   empty, size: O(1)
//   floor, ceiling, above, below, contains: O(R * (N ^ (1 / R)) + log(N)) amortized
//   values: O(N)
// Memory Complexity: O(N)
template <const int R, class Value, class Comparator = less<Value>> struct RootOrderMaintenance {
    Comparator cmp; int n; double SCALE_FACTOR; vector<Value> A[R];
    RootOrderMaintenance(const double SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}
    template <class It> RootOrderMaintenance(const It st, const It en, const double SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(is_sorted(st, en)); A[R - 1] = vector<Value>(st, en);
    }
    void resize() {
        double b = pow(n, 1.0 / R), c = b;
        for (int i = 0; i < R - 1; i++, c *= b) {
            if (int(A[i].size()) > SCALE_FACTOR * c) {
                int nxtSz = int(A[i + 1].size()); sort(A[i].begin(), A[i].end(), cmp);
                for (auto &&x : A[i]) A[i + 1].push_back(x);
                A[i].clear(); inplace_merge(A[i + 1].begin(), A[i + 1].begin() + nxtSz, A[i + 1].end(), cmp);   
            }
        }
    }
    void insert(const Value &val) { A[0].push_back(val); n++; }
    int aboveInd(const Value &val) {
        resize(); int ret = 0;
        for (int i = R - 1; i >= 1; i--) ret += upper_bound(A[i].begin(), A[i].end(), val, cmp) - A[i].begin();
        for (auto &&x : A[0]) ret += !cmp(val, x);
        return ret;
    }
    int ceilingInd(const Value &val) {
        resize(); int ret = 0;
        for (int i = R - 1; i >= 1; i--) ret += lower_bound(A[i].begin(), A[i].end(), val, cmp) - A[i].begin();
        for (auto &&x : A[0]) ret += cmp(x, val);
        return ret;
    }
    int floorInd(const Value &val) { return aboveInd(val) - 1; }
    int belowInd(const Value &val) { return ceilingInd(val) - 1; }
    bool contains(const Value &val) {
        for (int i = R - 1; i >= 1; i--) if (binary_search(A[i].begin(), A[i].end(), val, cmp)) return true;
        resize();
        for (int i = R - 1; i >= 1; i--) if (binary_search(A[i].begin(), A[i].end(), val, cmp)) return true;
        for (auto &&x : A[0]) if (!cmp(val, x) && !cmp(x, val)) return true;
        return false;
    }
    int count(const Value &val) { return aboveInd(val) - ceilingInd(val); }
    bool empty() const { return n == 0; } 
    int size() const { return n; } 
    void clear() const { for (int i = 0; i < R; i++) A[i].clear(); }
    vector<Value> values() const { // not sorted
        vector<Value> ret;
        for (int i = R - 1; i >= 0; i--) for (auto &&x : A[i]) ret.push_back(x);
        return ret;
    }
};
