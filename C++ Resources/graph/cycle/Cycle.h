#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a cycle in an undirected graph (including self loops and parallel edges)
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct Cycle {
    bool vis[MAXV]; int to[MAXV]; vector<int> adj[MAXV], cycle;
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    bool hasSelfLoop(int V) {
        for (int v = 0; v < V; v++) for (int w : adj[v]) {
            if (v != w) continue;
            cycle.clear(); cycle.push_back(v); cycle.push_back(v);
            return true;
        }
        return false;
    }
    bool hasParallelEdges(int V) {
        fill(vis, vis + V, false);
        for (int v = 0; v < V; v++) {
            for (int w : adj[v]) {
                if (vis[w]) {
                    cycle.clear(); cycle.push_back(v); cycle.push_back(w); cycle.push_back(v);
                    return true;
                }
                vis[w] = true;
            }
            for (int w : adj[v]) vis[w] = false;
        }
        return false;
    }
    void dfs(int v, int prev) {
        vis[v] = true;
        for (int w : adj[v]) {
            if (!cycle.empty()) return;
            if (!vis[w]) { to[w] = v; dfs(w, v); }
            else if (w != prev) {
                cycle.clear();
                for (int x = v; x != w; x = to[x]) cycle.push_back(x);
                cycle.push_back(w); cycle.push_back(v);
            }
        }
    }
    void clear(int V = MAXV) { cycle.clear(); for (int i = 0; i < V; i++) adj[i].clear(); }
    bool run(int V) { // returns true if there is a cycle
        if (hasSelfLoop(V) || hasParallelEdges(V)) return true;
        fill(vis, vis + V, false);
        for (int v = 0; v < V; v++) if (!vis[v]) dfs(v, -1);
        return !cycle.empty();
    }
};
