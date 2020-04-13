#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a tree using Tarjan's Offline algorithm
// Time Complexity: O(V + Q)
template <const int MAXV, const int MAXQ> struct TarjanOfflineLCA {
    const bool WHITE = false, BLACK = true; bool color[MAXV]; int par[MAXV], ans[MAXQ], UF[MAXV], Q = 0;
    vector<int> adj[MAXV]; vector<pair<int, int>> qAdj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    void join(int v, int w) {
        if ((v = find(v)) == (w = find(w))) return;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v;
    }
    void dfs(int v, int prev) {
        par[v] = v;
        for (int w : adj[v]) if (w != prev) { dfs(w, v); join(v, w); par[find(v)] = v; }
        color[v] = BLACK;
        for (auto &&q : qAdj[v]) if (color[q.first] == BLACK) ans[q.second] = par[find(q.first)];
    }
    void clear(int V = MAXV) { Q = 0; for (int i = 0; i < V; i++) { adj[i].clear(); qAdj[i].clear(); } }
    void run(int V, int root = 0) { fill(UF, UF + V, -1); fill(color, color + V, WHITE); dfs(root, -1); }
    void query(int v, int w) { qAdj[v].emplace_back(w, Q); qAdj[w].emplace_back(v, Q); Q++; }
};
