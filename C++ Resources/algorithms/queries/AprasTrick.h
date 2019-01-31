#pragma once
#include <bits/stdc++.h>
using namespace std;

// Apra's Trick to solve Range Minimum Queries offline
// Time Complexity: O((N + Q) * (Inverse Ackerman))
// Memory Complexity: O(N + Q)
template <const int MAXN, const int MAXQ, class T, const bool ONE_INDEXED> struct ArpasTrick {
    T A[MAXN], ans[MAXQ]; int Q = 0, UF[MAXN]; vector<pair<int, int>> L[MAXN];
    int find(int i) { while (i != UF[i]) i = UF[i] = UF[UF[i]]; return i; }
    // 0-indexed, inclusive
    void query(int l, int r) { L[r].emplace_back(l, Q++); }
    void solve(int N) {
        stack<int> stk;
        for (int r = ONE_INDEXED; r < N + ONE_INDEXED; r++) {
            for (UF[r] = r; !stk.empty() && A[stk.top()] > A[r]; stk.pop()) UF[stk.top()] = r;
            stk.push(r);
            for (auto &&q : L[r]) ans[q.second] = A[find(q.first)];
        }
    }
    void clear(int N = MAXN) { Q = 0; for (int i = 0; i < N; i++) L[i].clear(); }
};
