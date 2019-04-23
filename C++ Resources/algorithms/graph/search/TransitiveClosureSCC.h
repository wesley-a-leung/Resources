#pragma once
#include <bits/stdc++.h>
#include "../components/TarjanSCC.h"
using namespace std;

// Computes the Transitive Closure in a directed graph
// Time Complexity: O(V + E + VE / 64)
// Memory Complexity: O(V^2 / 64)
template <const int MAXV> struct TransitiveClosureSCC {
    TarjanSCC<MAXV> SCC; bool vis[MAXV]; bitset<MAXV> dp[MAXV];
    void addEdge(int v, int w) { SCC.addEdge(v, w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear() { SCC.clear(); }
    bitset<MAXV> dfs(int v) {
        if (vis[v]) return dp[v];
        vis[v] = true;
        dp[v][v] = 1;
        for (int w : SCC.DAG[v]) dp[v] |= dfs(w);
        return dp[v];
    }
    void run(int V) {
        SCC.run(V);
        for (int i = 0; i < int(SCC.components.size()); i++) { dp[i].reset(); vis[i] = false; }
        SCC.genDAG(V);
        for (int i = 0; i < int(SCC.components.size()); i++) if (!vis[i]) dfs(i);
    }
    // is w reachable from v
    bool reachable(int v, int w) { return dp[SCC.id[v]][SCC.id[w]]; }
};
