#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries on graphs that involve finding the number of nodes in a subtree
// with a certain property
// Time Complexity: O(V log V + Q)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ> struct DSUTree {
    int Q = 0, ans[MAXQ], color[MAXV], temp[MAXV], size[MAXV], cnt[MAXV]; bool isHeavy[MAXV];
    vector<int> adj[MAXV]; vector<pair<int, int>> q[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void query(int v, int val) { q[v].emplace_back(val, Q++); }
    void getSize(int v, int prev) {
        size[v] = 1;
        for (int w : adj[v]) if (w != prev) { getSize(w, v); size[v] += size[w]; }
    }
    void add(int v, int prev, int delta) {
        cnt[color[v]] += delta;
        for (int w : adj[v]) if (w != prev && !isHeavy[w]) add(w, v, delta);
    }
    void dfs(int v, int prev, bool keep) {
        int maxSize = -1, heavyInd = -1;
        for (int w : adj[v]) if (w != prev && size[w] > maxSize) { maxSize = size[w]; heavyInd = w; }
        for (int w : adj[v]) if (w != prev && w != heavyInd) dfs(w, v, 0);
        if (heavyInd != -1) { dfs(heavyInd, v, 1); isHeavy[heavyInd] = 1; }
        add(v, prev, 1);
        for (auto &&qq : q[v]) ans[qq.second] = cnt[qq.first];
        if (heavyInd != -1) isHeavy[heavyInd] = 0;
        if (!keep) add(v, prev, -1);
    }
    void run(int V) {
        copy(color, color + V, temp); sort(temp, temp + V); int k = unique(temp, temp + V) - temp;
        for (int v = 0; v < V; v++) color[v] = lower_bound(temp, temp + k, color[v]) - temp;
        fill(isHeavy, isHeavy + V, false);
        getSize(0, -1); dfs(0, -1, 0);
    }
    void clear(int V = MAXV) { Q = 0; for (int i = 0; i < V; i++) { adj[i].clear(); q[i].clear(); } }
};
