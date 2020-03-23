#pragma once
#include <bits/stdc++.h>
using namespace std;

// Determines the strong components in a directed graph using Kosaraju and Sharir's algorithm
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct KosarajuSharirSCC {
    int id[MAXV]; bool vis[MAXV]; vector<int> adj[MAXV], rev[MAXV], DAG[MAXV], revPost;
    vector<vector<int>> components;
    void addEdge(int v, int w) { adj[v].push_back(w); rev[w].push_back(v); }
    void postOrder(int v) {
        vis[v] = true;
        for (int w : rev[v]) if (!vis[v]) postOrder(w);
        revPost.push_back(v);
    }
    void dfs(int v) {
        vis[v] = true; id[v] = components.size() - 1; components.back().push_back(v);
        for (int w : adj[v]) if (!vis[v]) dfs(w);
    }
    void clear(int V = MAXV) { components.clear(); for (int i = 0; i < V; i++) { adj[i].clear(); rev[i].clear(); } }
    void run(int V) {
        fill(vis, vis + V, false);
        for (int v = 0; v < V; v++) if (!vis[v]) postOrder(v);
        fill(vis, vis + V, false); reverse(revPost.begin(), revPost.end());
        for (int v : revPost) if (!vis[v]) { components.emplace_back(); dfs(v); }
    }
    void genDAG(int V) {
        for (int v = 0; v < V; v++) for (int w : adj[v]) if (id[v] != id[w]) DAG[id[v]].push_back(id[w]);
        for (int i = 0; i < int(components.size()); i++) {
            sort(DAG[i].begin(), DAG[i].end()); DAG[i].erase(unique(DAG[i].begin(), DAG[i].end()), DAG[i].end());
        }
    }
};
