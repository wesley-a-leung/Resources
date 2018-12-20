#pragma once
#include <bits/stdc++.h>
#include "../components/TarjanSCC.h"
using namespace std;

// Computes the Transitive Closure in a directed graph
// Time Complexity: O(V + E + VE / 64)
// Memory Complexity: O(V^2)
template <const int MAXV, class unit> struct TransistiveClosureSCC {
    TarjanSCC<MAXV> SCC; bool G[MAXV][MAXV]; bitset<MAXV> vis[MAXV]; vector<int> DAG[MAXV];
    void addEdge(int v, int w) { DAG.addEdge(v, w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear() { SCC.clear(); }
    bitset<MAXV> dfs(int v) {
        if (vis[v].count()) return vis[v];
        for (int w : SCC.components[v]) vis[v][w] = 1;
        for (int w : DAG[v]) vis[v] |= dfs(w);
        return vis[v];
    }
    void run(int V) {
        SCC.run(V);
        for (int i = 0; i < int(SCC.components.size); i++) {
            DAG[i].clear(); vis[i].reset(); fill(G[i], G[i] + int(SCC.components.size), false);
        }
        for (int v = 0; v < V; v++) for (int w : SCC.adj[v]) if (SCC.id[v] != SCC.id[w]) G[SCC.id[v]][SCC.id[w]] = true;
        for (int i = 0; i < int(SCC.components.size); i++) dfs(i);
    }
    bool reachable(int v, int w) { // is w reachable from v
        return vis[SCC.id[v]][w];
    }
};
