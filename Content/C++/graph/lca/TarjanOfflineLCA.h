#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a tree using Tarjan's Offline algorithm
// Time Complexity: O(V + Q)
template <const int MAXV, const int MAXQ> struct TarjanOfflineLCA {
    bool vis[MAXV]; int par[MAXV], ans[MAXQ], Q = 0; vector<int> adj[MAXV]; vector<pair<int, int>> q[MAXV]; UnionFind<MAXV> uf;
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v, int prev) {
        par[v] = v;
        for (int w : adj[v]) if (w != prev) { dfs(w, v); uf.join(v, w); par[uf.find(v)] = v; }
        vis[v] = true;
        for (auto &&q : q[v]) if (vis[q.first]) ans[q.second] = par[uf.find(q.first)];
    }
    void clear(int V = MAXV) { Q = 0; for (int i = 0; i < V; i++) { adj[i].clear(); q[i].clear(); } }
    void run(int V, int root = 0) { uf.init(V); fill(vis, vis + V, false); dfs(root, -1); }
    void query(int v, int w) { q[v].emplace_back(w, Q); q[w].emplace_back(v, Q++); }
};
