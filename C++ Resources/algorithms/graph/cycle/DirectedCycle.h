#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a cycle in a directed graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct DirectedCycle {
    bool vis[MAXV], onStack[MAXV]; int to[MAXV]; vector<int> adj[MAXV], cycle;
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void dfs(int v) {
        vis[v] = onStack[v] = true;
        for (int w : adj[v]) {
            if (!cycle.empty()) return;
            else if (!vis[w]) { to[w] = v; dfs(w); }
            else if (onStack[w]) {
                cycle.clear();
                for (int x = v; x != w; x = to[x]) cycle.push_back(x);
                cycle.push_back(w); cycle.push_back(v);
            }
        }
        onStack[v] = false;
    }
    void clear(int V = MAXV) { cycle.clear(); for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V) {
        fill(vis, vis + MAXV, false);
        for (int v = 0; v < V; v++) if (!vis[v] && cycle.empty()) dfs(v);
    }
};
