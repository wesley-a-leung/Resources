#pragma once
#include <bits/stdc++.h>
using namespace std;

// Determines the strong components in a directed graph using Kosaraju and Sharir's algorithm
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct KosarajuSharirSCC {
    int id[MAXV]; bool vis[MAXV]; vector<int> adj[MAXV], rev[MAXV]; vector<vector<int>> components; stack<int> revPost;
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void postOrder(int v) {
        vis[v] = true;
        for (int w : rev[v]) if (!vis[v]) postOrder(w);
        revPost.push(v);
    }
    void dfs(int v) {
        vis[v] = true; id[v] = components.size() - 1; components.back().push_back(v);
        for (int w : adj[v]) if (!vis[v]) dfs(w);
    }
    void clear() { components.clear(); for (int i = 0; i < MAXV; i++) { adj[i].clear(); rev[i].clear(); } }
    void run(int V) {
        fill(vis, vis + V, false);
        for (int v = 0; v < V; v++) for (int w : adj[v]) rev[w].push_back(v);
        for (int v = 0; v < V; v++) if (!vis[v]) postOrder(v);
        fill(vis, vis + V, false);
        while (!revPost.empty()) {
            int v = revPost.top(); revPost.pop();
            if (!vis[v]) { components.emplace_back(); dfs(v); }
        }
    }
};
