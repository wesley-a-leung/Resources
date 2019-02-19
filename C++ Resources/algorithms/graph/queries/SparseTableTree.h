#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves queries on a tree using a sparse table / binary lifting
// Time Complexity:
//   run: O(V log V)
//   lca: O(log V)
//   queryPath: O(log V) * (complexity of merge)
// Memory Complexity: O(V log V)
template <const int MAXV, const int MAXLGV, const bool VALUES_ON_EDGES> struct SparseTableTree {
    using Data = int; const Data vdef = 0, qdef = 0; Data A[MAXV], ST[MAXLGV][MAXV]; int dep[MAXV], par[MAXLGV][MAXV]; vector<int> adj[MAXV];
    Data merge(const Data &l, const Data &r); // to be implemented
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v, int prev, int d) {
        dep[v] = d; par[0][v] = prev;
        for (int w : adj[v]) if (w != prev) dfs(w, v, d + 1);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) {
        copy(A, A + V, ST[0]); dfs(root, -1, 0);
        for (int i = 0; i < MAXLGV - 1; i++) for (int j = 0; j < V; j++) {
            if (par[i][j] == -1) { par[i + 1][j] = -1; ST[i + 1][j] = ST[i][j]; }
            else { par[i + 1][j] = par[i][par[i][j]]; ST[i + 1][j] = merge(ST[i][j], ST[i][par[i][j]]); }
        }
    }
    int lca(int v, int w) {
        if (dep[v] < dep[w]) swap(v, w);
        for (int i = MAXLGV - 1; i >= 0; i--) if (par[i][v] != -1 && dep[par[i][v]] >= dep[w]) v = par[i][v];
        if (v == w) return v;
        for (int i = MAXLGV - 1; i >= 0; i--) if (par[i][v] != par[i][w]) { v = par[i][v]; w = par[i][w]; }
        return par[0][v];
    }
    Data queryPath(int v, int w) {
        Data up = qdef, down = qdef; bool isUp = true;
        if (dep[v] < dep[w]) { swap(v, w); isUp = false; }
        for (int i = MAXLGV - 1; i >= 0; i--) if (par[i][v] != -1 && dep[par[i][v]] >= dep[w]) {
            if (isUp) up = merge(up, ST[i][v]);
            else down = merge(ST[i][v], down);
            v = par[i][v];
        }
        if (v != w) {
            if (!isUp) swap(v, w);
            for (int i = MAXLGV - 1; i >= 0; i--) if (par[i][v] != par[i][w]) {
                up = merge(up, ST[i][v]); down = merge(ST[i][w], down); v = par[i][v]; w = par[i][w];
            }
            up = merge(up, ST[0][v]); down = merge(ST[0][w], down);
            if (!VALUES_ON_EDGES) up = merge(up, ST[0][par[0][v]]);
        } else if (!VALUES_ON_EDGES) up = merge(up, ST[0][v]);
        return merge(up, down);
    }
};
