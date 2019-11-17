#pragma once
#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm, supporting point updates, used to count the number of distinct integers in a subarray
// Time Complexity: O(N + Q * ((N/B)^2 + B * (update complexity)))
// Memory Complexity: O(N + Q)
template <const int MAXN, const int MAXQ, const int BLOCKSZ> struct MoUpdates {
    int Q = 0, cnt[MAXN + MAXQ], ans[MAXQ], val[MAXN], temp[MAXN + MAXQ], curAns;
    pair<int, int> q[MAXQ]; bool isQuery[MAXQ]; vector<int> qs[MAXN / BLOCKSZ + 5][MAXN / BLOCKSZ + 5];
    void add(int i) { if (cnt[val[i]]++ == 0) curAns++; }
    void rem(int i) { if (--cnt[val[i]] == 0) curAns--; }
    void upd(int i, int x) { rem(i); val[i] = x; add(i); }
    void query(int l, int r) { q[Q] = {l, r}; isQuery[Q++] = true; }
    void update(int i, int x) { q[Q] = {i, x}; isQuery[Q++] = false; }
    void run(int N) {
        int cur = N, blocks = (N - 1) / BLOCKSZ + 1; fill(cnt, cnt + N + Q, 0);
        for (int i = 0; i < Q; i++) if (!isQuery[i]) temp[cur++] = q[i].second;
        copy(val, val + N, temp); sort(temp, temp + cur); int k = unique(temp, temp + cur) - temp;
        for (int i = 0; i < N; i++) val[i] = lower_bound(temp, temp + k, val[i]) - temp;
        for (int i = 0; i < Q; i++) {
            if (isQuery[i]) qs[q[i].first / BLOCKSZ][q[i].second / BLOCKSZ].push_back(i);
            else {
                q[i].second = lower_bound(temp, temp + k, q[i].second) - temp;
                for (int bl = 0; bl <= q[i].first / BLOCKSZ; bl++) for (int br = q[i].first / BLOCKSZ; br < blocks; br++)
                    qs[bl][br].push_back(i);
            }
        }
        int l = 0, r = l - 1; curAns = 0; stack<pair<int, int>> revert;
        for (int bl = 0; bl < blocks; bl++) for (int br = bl; br < blocks; br++) {
            for (int i : qs[bl][br]) {
                if (isQuery[i]) {
                    while (l < q[i].first) rem(l++);
                    while (l > q[i].first) add(--l);
                    while (r < q[i].second) add(++r);
                    while (r > q[i].second) rem(r--);
                    ans[i] = curAns;
                } else {
                    revert.emplace(q[i].first, val[q[i].first]);
                    if (l <= q[i].first && q[i].first <= r) upd(q[i].first, q[i].second);
                    else val[q[i].first] = q[i].second;
                }
            }
            while (!revert.empty()) {
                pair<int, int> x = revert.top(); revert.pop();
                if (l <= x.first && x.first <= r) upd(x.first, x.second);
                else val[x.first] = x.second;
            }
        }
    }
    void clear(int N = MAXN) {
        Q = 0; int bnd = min((N - 1) / BLOCKSZ + 1, MAXN / BLOCKSZ + 5);
        for (int i = 0; i < bnd; i++) for (int j = 0; j < bnd; j++) qs.clear();
    }
};
