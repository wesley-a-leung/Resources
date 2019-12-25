#pragma once
#include <bits/stdc++.h>
using namespace std;

// Arpa's Trick to solve Range Minimum Queries offline
// Time Complexity: O((N + Q) * alpha(N))
// Memory Complexity: O(N + Q)
template <const int MAXN, const int MAXQ, class T, const bool ONE_INDEXED> struct ArpasTrick {
    T A[MAXN], ans[MAXQ]; int Q = 0, UF[MAXN], stk[MAXN]; vector<pair<int, int>> L[MAXN];
    int find(int i) { return UF[i] == i ? i : UF[i] = find(UF[i]); }
    // 0-indexed, inclusive
    void query(int l, int r) { L[r].emplace_back(l, Q++); }
    void solve(int N) {
        for (int r = ONE_INDEXED, top = 0; r < N + ONE_INDEXED; r++) {
            for (UF[r] = r; top > 0 && A[stk[top - 1]] > A[r];) UF[stk[--top]] = r;
            for (auto &&q : L[stk[top++] = r]) ans[q.second] = A[find(q.first)];
        }
    }
    void clear(int N = MAXN) { Q = 0; for (int i = 0; i < N; i++) L[i].clear(); }
};
