#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports updates and queries on a subtree
// Time Complexity:
//   run: O(V)
//   updateSubtree, querySubtree, updateVertex: O(1) * (complexity of update/query)
// Memory Complexity: O(V)
template <const int MAXV, bool ONE_INDEXED> struct SubtreeQueries {
    using Data = int; int st[MAXV], en[MAXV], vert[MAXV], curInd; vector<int> adj[MAXV];
    void update(int i, Data val); // to be implemented
    void update(int l, int r, Data val); // to be implemented
    Data query(int l, int r); // to be implemented
    void dfs(int v, int prev) {
        vert[st[v] = ++curInd] = v;
        for (int w : adj[v]) if (w != prev) dfs(w, v);
        en[v] = curInd;
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) {
        curInd = int(ONE_INDEXED) - 1; fill(st, st + V, -1);
        for (int v = 0; v < V; v++) if (st[v] == -1) dfs(v, -1);
    }
    void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
    void updateVertex(int v, Data val) { update(st[v], val); }
    void updateSubtree(int v, Data val) { update(st[v], en[v], val); }
    Data querySubtree(int v) { return query(st[v], en[v]); }
};
