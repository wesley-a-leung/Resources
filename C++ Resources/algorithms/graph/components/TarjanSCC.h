#pragma once
#include <bits/stdc++.h>
using namespace std;

// Determines the strong components in a directed graph using Tarjan's algorithm
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct TarjanSCC {
    int id[MAXV], low[MAXV], pre; bool vis[MAXV]; vector<int> adj[MAXV], DAG[MAXV]; vector<vector<int>> components; stack<int> s;
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void dfs(int v) {
        vis[v] = true; int mn = low[v] = pre++; s.push(v);
        for (int w : adj[v]) {
            if (!vis[w]) dfs(w);
            if (low[w] < mn) mn = low[w];
        }
        if (mn < low[v]) { low[v] = mn; return; }
        int w; components.emplace_back();
        do {
            w = s.top(); s.pop();
            id[w] = components.size() - 1; components.back().push_back(w); low[w] = INT_MAX;
        } while (w != v);
    }
    void clear(int V = MAXV) { components.clear(); for (int i = 0; i < V; i++) { adj[i].clear(); DAG[i].clear(); } }
    void run(int V) {
        fill(vis, vis + V, false); pre = 0;
        for (int v = 0; v < V; v++) if (!vis[v]) dfs(v);
    }
    void genDAG(int V) {
        for (int v = 0; v < V; v++) for (int w : adj[v]) if (id[v] != id[w]) DAG[id[v]].push_back(id[w]);
        for (int i = 0; i < int(components.size()); i++) {
            sort(DAG[i].begin(), DAG[i].end());
            DAG[i].erase(unique(DAG[i].begin(), DAG[i].end()), DAG[i].end());
        }
    }
};
