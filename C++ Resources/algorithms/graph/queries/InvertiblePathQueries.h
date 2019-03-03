#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports updates and queries on a tree path from the root for an invertible operation
// Time Complexity:
//   run: O(V)
//   updateVertex, queryPath: O(1) * (complexity of update/query)
// Memory Complexity: O(V)
template <const int MAXV, const bool ONE_INDEXED> struct PathQueries {
    using Data = int; using Lazy = int; int st[MAXV], en[MAXV], vert[MAXV * 2], curInd;
    bool isPre[MAXV * 2]; Data A[MAXV]; vector<int> adj[MAXV];
    Lazy inverse(const Lazy &val); // to be implemented
    void update(int i, const Lazy &val); // to be implemented
    Data query(int l, int r); // to be implemented
    void dfs(int v, int prev) {
        vert[st[v] = ++curInd] = v; isPre[curInd] = true;
        for (int w : adj[v]) if (w != prev) dfs(w, v);
        vert[en[v] = ++curInd] = v; isPre[curInd] = false;
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) {
        curInd = int(ONE_INDEXED) - 1; fill(st, st + V, -1); dfs(root, -1);
        for (int v = 0; v < V; v++) if (st[v] == -1) dfs(v, -1);
    }
    void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
    void updateVertex(int v, const Lazy &val) { update(st[v], A[v] = val); update(en[v], inverse(val)); }
    Data queryPathFromRoot(int v) { return query(int(ONE_INDEXED), st[v]); }
};
