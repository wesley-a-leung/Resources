#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports persistent queries for the earliest time two vertices are in the same 2-edge connected component
// after edges have been added
// Time Complexity:
//   solve: O((V + Q) log V)
//   query: O(log V log Q)
// Memory Complexity O(V + Q)
template <const int MAXV, const int MAXQ> struct SemiDynamicBridgesPersistent {
    int Q = 0, UF[MAXV], par[MAXV], dep[MAXV]; vector<int> adj[MAXV], ans; vector<pair<int, int>> UF2[MAXV];
    struct Query { int type, v, w; } q[MAXQ];
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    int find2(int t, int v) { return UF2[v].back().second < 0 || UF2[v].back().first > t ? v : find2(t, UF2[v].back().second); }
    bool join(int v, int w) {
        if ((v = find(v)) == (w = find(w))) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v; return true;
    }
    void dfs(int v, int prev, int d) {
        dep[v] = d; par[v] = prev;
        for (int w : adj[v]) if (w != prev) dfs(w, v, d + 1);
    }
    void init(int V = MAXV) {
        fill(UF, UF + V, -1); Q = 0; ans.clear();
        for (int i = 0; i < V; i++) { adj[i].clear(); UF2[i].clear(); }
    }
    void addEdge(int v, int w) {
        if (join(v, w)) { adj[v].push_back(w); adj[w].push_back(v); q[Q++] = {1, v, w}; }
        else q[Q++] = {2, v, w};
    }
    void solve(int V) {
        fill(par, par + MAXV, -1); fill(UF, UF + MAXV, -1);
        for (int v = 0; v < V; v++) if (par[v] == -1) dfs(v, -1, 0);
        for (int i = 0; i < V; i++) UF2[i].emplace_back(-1, -1);
        for (int i = 0; i < Q; i++) if (q[i].type == 2) {
            int a = q[i].v, b = q[i].w;
            while ((a = find2(i, a)) != (b = find2(i, b))) {
                if (dep[a] < dep[b]) swap(a, b);
                UF2[a].emplace_back(i, find2(i, par[a])); a = par[a];
            }
        }
    }
    // returns the first time i (0-indexed) such that v and w are in the same 2-edge connected component
    // after the ith edge is added
    // returns -1 if v = w and Q if they are never in the same 2-edge connected component
    int query(int v, int w) {
        int lo = -1, hi = Q;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (find2(mid, v) == find2(mid, w)) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
};
