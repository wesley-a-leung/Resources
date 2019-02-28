#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array, allowing for multiple insertions at the same time (including negative)
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   rebuild: O(R * (N ^ (1 / R)))
//   count(): O(1)
//   floor, ceiling, above, below, contains, count(val), count(lo, hi): O(R * (N ^ (1 / R)) + log(N)) amortized
//   valuesAndCount: O(N)
// Memory Complexity: O(N)
template <const int R, class Value, class CountType, class Comparator = less<Value>> struct RootOrderMaintenance {
    Comparator cmp; CountType tot; int n; double SCALE_FACTOR; vector<pair<Value, CountType>> A[R];
    function<bool(const pair<Value, CountType>&, const pair<Value, CountType>&)> pairCmp =
        [&] (const pair<Value, CountType> &a, const pair<Value, CountType> &b) { return cmp(a.first, b.first); };
    RootOrderMaintenance(const double SCALE_FACTOR = 1) : tot(0), n(0), SCALE_FACTOR(SCALE_FACTOR) {}
    template <class PairIt> RootOrderMaintenance(const PairIt st, const PairIt en, const double SCALE_FACTOR = 1) :
            SCALE_FACTOR(SCALE_FACTOR) {
        assert(is_sorted(st, en, pairCmp)); A[R - 1] = vector<pair<Value, CountType>>(st, en); resizeUnique(A[R - 1]);
        n = int(A[R - 1].size()); tot = 0; for (auto &&p : A[R - 1]) tot += p.second;
    }
    void resizeUnique(vector<pair<Value, CountType>> &v) {
        if (!v.empty()) {
            int j = 0;
            for (int i = 1; i < int(v.size()); i++) {
                if (cmp(v[i].first, v[j].first) || cmp(v[j].first, v[i].first)) {
                    v[++j] = v[i]; v[j].second += v[j - 1].second;
                } else v[j].second += v[i].second;
            }
            v.resize(j + 1);
        }
    }
    void rebuild() {
        double b = pow(n, 1.0 / R), c = b;
        for (int i = 0; i < R - 1; i++, c *= b) {
            if (int(A[i].size()) > SCALE_FACTOR * c) {
                int nxtSz = int(A[i + 1].size());
                if (i == 0) sort(A[i].begin(), A[i].end(), pairCmp);
                else for (int j = int(A[i].size()) - 1; j >= 1; j--) A[i][j].second -= A[i][j - 1].second;
                for (int j = nxtSz - 1; j >= 1; j--) A[i + 1][j].second -= A[i + 1][j - 1].second;
                for (auto &&p : A[i]) A[i + 1].push_back(p);
                A[i].clear(); inplace_merge(A[i + 1].begin(), A[i + 1].begin() + nxtSz, A[i + 1].end(), pairCmp); resizeUnique(A[i + 1]);
            }
        }
    }
    void insert(const pair<Value, CountType> &p) { A[0].push_back(p); tot += p.second; n++; }
    void emplace(const Value &v, const CountType &c) { A[0].emplace_back(v, c); tot += c; n++; }
    CountType aboveInd(const Value &val) {
        rebuild(); CountType ret = 0;
        for (int i = R - 1; i >= 1; i--) {
            int ind = upper_bound(A[i].begin(), A[i].end(), make_pair(val, CountType(0)), pairCmp) - A[i].begin();
            ret += ind == 0 ? 0 : A[i][ind - 1].second;
        }
        for (auto &&p : A[0]) if (!cmp(val, p.first)) ret += p.second;
        return ret;
    }
    CountType ceilingInd(const Value &val) {
        rebuild(); CountType ret = 0;
        for (int i = R - 1; i >= 1; i--) {
            int ind = lower_bound(A[i].begin(), A[i].end(), make_pair(val, CountType(0)), pairCmp) - A[i].begin();
            ret += ind == 0 ? 0 : A[i][ind - 1].second;
        }
        for (auto &&p : A[0]) if (cmp(p.first, val)) ret += p.second;
        return ret;
    }
    CountType floorInd(const Value &val) { return aboveInd(val) - 1; }
    CountType belowInd(const Value &val) { return ceilingInd(val) - 1; }
    bool contains(const Value &val) {
        for (int i = R - 1; i >= 1; i--) if (binary_search(A[i].begin(), A[i].end(), make_pair(val, CountType(0)), pairCmp)) return true;
        rebuild();
        for (int i = R - 1; i >= 1; i--) if (binary_search(A[i].begin(), A[i].end(), make_pair(val, CountType(0)), pairCmp)) return true;
        for (auto &&p : A[0]) if (!cmp(val, p.first) && !cmp(p.first, val)) return true;
        return false;
    }
    CountType count(const Value &val) { return aboveInd(val) - ceilingInd(val); }
    // number of values in the range [lo, hi]
    CountType count(const Value &lo, const Value &hi) { return aboveInd(hi) - ceilingInd(lo); }
    CountType count() const { return tot; } 
    void clear() { tot = 0; for (int i = 0; i < R; i++) A[i].clear(); }
    vector<pair<Value, CountType>> valuesAndCount() const { // sorted
        vector<pair<Value, CountType>> ret;
        for (int i = 0; i < R; i++) {
            int mid = int(ret.size());
            for (auto &&p : A[i]) ret.push_back(p);
            if (i > 0) inplace_merge(ret.begin(), ret.begin() + mid, ret.end(), pairCmp);
        }
        resizeUnique(ret); return ret;
    }
};
