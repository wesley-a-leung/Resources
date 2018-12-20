#pragma once
#include <bits/stdc++.h>
#include "../components/TarjanSCC.h"
using namespace std;

// Computes the Transitive Closure in a directed graph
// Time Complexity: O(V + E + VE / 64)
// Memory Complexity: O(V^2)
template <const int MAXV> struct TransistiveClosureSCC {
    TarjanSCC<MAXV> SCC; bool G[MAXV][MAXV], vis[MAXV]; bitset<MAXV> dp[MAXV]; vector<int> DAG[MAXV];
    void addEdge(int v, int w) { SCC.addEdge(v, w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear() { SCC.clear(); }
    bitset<MAXV> dfs(int v) {
        if (vis[v]) return dp[v];
        vis[v] = true;
        for (int w : SCC.components[v]) dp[v][w] = 1;
        for (int w : DAG[v]) dp[v] |= dfs(w);
        return dp[v];
    }
    void run(int V) {
        SCC.run(V);
        for (int i = 0; i < int(SCC.components.size()); i++) {
            DAG[i].clear(); dp[i].reset(); fill(G[i], G[i] + int(SCC.components.size()), false); vis[i] = false;
        }
        for (int v = 0; v < V; v++) for (int w : SCC.adj[v]) if (SCC.id[v] != SCC.id[w]) G[SCC.id[v]][SCC.id[w]] = true;
        for (int i = 0; i < int(SCC.components.size()); i++) for (int j = 0; j < int(SCC.components.size()); j++) if (G[i][j]) DAG[i].push_back(j);
        for (int i = 0; i < int(SCC.components.size()); i++) dfs(i);
    }
    // is w reachable from v
    bool reachable(int v, int w) { return dp[SCC.id[v]][w]; }
};
