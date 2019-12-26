#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintans the difference between adjacent elements in a sorted set
// Time Complexity:
//   insert, erase: O(log N)
// Memory Complexity: O(N)
template <class T> struct SetDifferenceMaintenance {
    map<T, int> vals, diffs;
    void insert(T v) {
        auto cur = vals.find(v);
        if (cur != vals.end()) { ++cur->second; ++diffs[0]; return; }
        auto nxt = vals.upper_bound(v);
        if (nxt != vals.begin() && nxt != vals.end()) {
            auto it = diffs.find(nxt->first - prev(nxt)->first);
            if (--it->second == 0) diffs.erase(it);
        }
        cur = vals.emplace_hint(nxt, v, 1);
        if (nxt != vals.end()) ++diffs[nxt->first - v];
        if (cur != vals.begin()) ++diffs[v - prev(cur)->first];
    }
    void erase(T v) {
        auto cur = vals.find(v);
        if (cur->second >= 2) {
            --cur->second; auto it = diffs.find(0);
            if (--it->second == 0) diffs.erase(it);
            return;
        }
        auto nxt = next(cur);
        if (nxt != vals.end()) {
            auto it = diffs.find(nxt->first - v);
            if (--it->second == 0) diffs.erase(it);
        }
        if (cur != vals.begin()) {
            auto it = diffs.find(v - prev(cur)->first);
            if (--it->second == 0) diffs.erase(it);
        }
        vals.erase(cur);
        if (nxt != vals.begin()) if (nxt != vals.end()) ++diffs[nxt->first - prev(nxt)->first];
    }
};
