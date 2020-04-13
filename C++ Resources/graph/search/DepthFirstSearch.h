#pragma once
#include <bits/stdc++.h>
using namespace std;

// Depth First Traversal of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct DFS {
    bool vis[MAXV]; int dep[MAXV], to[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void dfs(int v, int d) {
        vis[v] = true; dep[v] = d;
        for (int w : adj[v]) if (!vis[w]) { to[w] = v; dfs(w, d + 1); }
    }
    void run(int V, int s) { fill(vis, vis + V, false); fill(to, to + V, -1); dfs(s, 0); }
    vector<int> getPath(int v) {
        vector<int> path;
        for (; v != -1; v = to[v]) path.push_back(v);
        reverse(path.begin(), path.end()); return path;
    }
};
