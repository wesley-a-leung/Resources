#pragma once
#include <bits/stdc++.h>
using namespace std;

// Union Find / Disjoint Sets by size supporting queries for a past version of the data structure
// Time Complexity:
//   init: O(N)
//   find, join, connected, getSize, getCnt: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, const bool ONE_INDEXED> struct UnionFindSemiPersistent {
    vector<pair<int, int>> UF[MAXN], cnt; int curTime;
    void init(int N) {
        curTime = 0; cnt.emplace_back(curTime, N);
        for (int i = ONE_INDEXED; i < N + ONE_INDEXED; i++) UF[i].emplace_back(curTime, -1);
    }
    int find(int t, int v) { return UF[v].back().second >= 0 && UF[v].back().first <= t ? find(t, UF[v].back().second) : v; }
    bool join(int v, int w) {
        ++curTime;
        if ((v = find(curTime, v)) == (w = find(curTime, w))) return false;
        int ufv = UF[v].back().second, ufw = UF[w].back().second;
        if (ufv > ufw) { swap(v, w); swap(ufv, ufw); }
        UF[v].emplace_back(curTime, ufv + ufw); UF[w].emplace_back(curTime, v);
        cnt.emplace_back(curTime, cnt.back().second - 1); return true;
    }
    bool connected(int t, int v, int w) { return find(t, v) == find(t, w); }
    int getSize(int t, int v) { v = find(t, v); return -prev(upper_bound(UF[v].begin(), UF[v].end(), make_pair(t, INT_MAX)))->second; }
    int getCnt(int t) { return prev(upper_bound(cnt.begin(), cnt.end(), make_pair(t, INT_MAX)))->second; }
};
