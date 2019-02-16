#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the distance between 2 vertices in a tree
// Time Complexity:
//   run: O(V log V)
//   lca, getDist: O(1)
template <const int MAXV, const int MAXLGV, class unit> struct APSPTree {
    int ind, head[MAXV], dep[MAXV], root[MAXV], rmq[MAXLGV][2 * MAXV]; unit dist[MAXV]; vector<pair<int, unit>> adj[MAXV];
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); adj[w].emplace_back(v, weight); }
    void dfs(int v, int prev, int r, int d, unit dd) {
        dep[v] = d; dist[v] = dd; root[v] = r; rmq[0][head[v] = ind++] = v;
        for (auto &&e : adj[v]) if (e.first != prev) { dfs(e.first, v, r, d + 1, dd + e.second); rmq[0][ind++] = v; }
    }
    int minDep(int v, int w) { return dep[v] < dep[w] ? v : w; }
    int RMQ(int l, int r) { int i = 31 - __builtin_clz(r - l + 1); return minDep(rmq[i][l], rmq[i][r - (1 << i) + 1]); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V) {
        ind = 0; int lg = 32 - __builtin_clz(V * 2 - 1); assert(lg < MAXLGV); fill(root, root + V, -1);
        for (int i = 0; i < V; i++) if (root[i] == -1) dfs(i, -1, i, 0, 0);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j < ind; j++) rmq[i + 1][j] = minDep(rmq[i][j], rmq[i][min(j + (1 << i), ind - 1)]);
    }
    int lca(int v, int w) {
        if (head[v] > head[w]) swap(v, w);
        return RMQ(head[v], head[w]);
    }
    unit getDist(int v, int w) { return root[v] == root[w] ? dist[v] + dist[w] - 2 * dist[lca(v, w)] : -1; }
};
