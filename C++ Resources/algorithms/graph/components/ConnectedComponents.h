#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the connected components of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct ConnectedComponents {
    int id[MAXV]; vector<int> adj[MAXV]; vector<vector<int>> components;
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v) {
        id[v] = int(components.size()) - 1; components.back().push_back(v);
        for (int w : adj[v]) if (id[w] == -1) dfs(w);
    }
    void clear() { components.clear(); for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    void run(int V) {
        fill(id, id + V, -1);
        for (int v = 0; v < V; v++) if (id[v] == -1) { components.emplace_back(); dfs(v); }
    }
};
