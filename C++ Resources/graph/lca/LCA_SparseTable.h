#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a tree using a sparse table
// Time Complexity:
//    run: O(V log V)
//    lca: O(log V)
template <const int MAXV, const int MAXLGV> struct LCA_SparseTable {
    int dep[MAXV], par[MAXLGV][MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v, int prev, int d) {
        dep[v] = d; par[0][v] = prev;
        for (int w : adj[v]) if (w != prev) dfs(w, v, d + 1);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) {
        dfs(root, -1, 0);
        for (int i = 0; i < MAXLGV - 1; i++) for (int j = 0; j < V; j++) par[i + 1][j] = par[i][j] == -1 ? -1 : par[i][par[i][j]];
    }
    int lca(int v, int w) {
        if (dep[v] < dep[w]) swap(v, w);
        for (int i = MAXLGV - 1; i >= 0; i--) if (par[i][v] != -1 && dep[par[i][v]] >= dep[w]) v = par[i][v];
        if (v == w) return v;
        for (int i = MAXLGV - 1; i >= 0; i--) if (par[i][v] != par[i][w]) { v = par[i][v]; w = par[i][w]; }
        return par[0][v];
    }
};
