#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a tree by reduing the problem
// to a range minimum query
// Time Complexity:
//    run: O(V log V)
//    lca: O(1)
template <const int MAXV, const int MAXLGV> struct LCA_RMQ {
    int LG[MAXV * 2], ind, head[MAXV], dep[MAXV], rmq[MAXLGV][MAXV * 2]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v, int prev, int d) {
        dep[v] = d; rmq[0][head[v] = ind++] = v;
        for (int w : adj[v]) if (w != prev) { dfs(w, v, d + 1); rmq[0][ind++] = v; }
    }
    int minDep(int v, int w) { return dep[v] < dep[w] ? v : w; }
    int RMQ(int l, int r) { int i = LG[r - l + 1]; return minDep(rmq[i][l], rmq[i][r - (1 << i) + 1]); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) {
        LG[0] = LG[1] = 0;
        for (int i = 2; i <= V * 2 - 1; i++) LG[i] = LG[i / 2] + 1;
        ind = 0; dfs(root, -1, 0); int lg = LG[V * 2 - 1] + 1; assert(lg < MAXLGV);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j < ind; j++) rmq[i + 1][j] = minDep(rmq[i][j], rmq[i][min(j + (1 << i), ind - 1)]);
    }
    int lca(int v, int w) {
        if (head[v] > head[w]) swap(v, w);
        return RMQ(head[v], head[w]);
    }
    int dist(int v, int w) { return dep[v] + dep[w] - 2 * dep[lca(v, w)]; }
};
