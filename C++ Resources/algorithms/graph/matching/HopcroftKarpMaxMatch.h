#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching (ans minimum vertex cover) on an unweighted bipartite graph
// Time Complexity: O((V + E) sqrt V)
// Memory Complexity: O(V + E)
template <const int MAXV> struct HopcroftKarpMaxMatch {
    int cardinality, mate[MAXV], dist[MAXV], pathDist; vector<int> adj[MAXV], typeA; bool color[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    bool hasPath() {
        queue<int> q; pathDist = INT_MAX;
        for (int v : typeA) {
            if (mate[v] == -1) { dist[v] = 0; q.push(v); }
            else dist[v] = INT_MAX;
        }
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int w : adj[v]) {
                if (mate[w] == -1) {
                    if (pathDist == INT_MAX) pathDist = dist[v] + 1;
                } else if (dist[mate[w]] == INT_MAX) {
                    dist[mate[w]] = dist[v] + 1;
                    if (pathDist == INT_MAX) q.push(mate[w]);
                }
            }
        }
        return pathDist != INT_MAX;
    }
    bool dfs(int v) {
        for (int w : adj[v]) {
            if (mate[w] == -1) {
                if (pathDist == dist[v] + 1) { mate[w] = v; mate[v] = w; return true; }
            } else if (dist[mate[w]] == dist[v] + 1) {
                if (dfs(mate[w])) { mate[w] = v; mate[v] = w; return true; }
            }
        }
        dist[v] = INT_MAX; return false;
    }
    void init(int V = MAXV) { fill(mate, mate + V, -1); fill(color, color + V, false); }
    void clear(int V = MAXV) { typeA.clear(); for (int i = 0; i < V; i++) adj[i].clear(); }
    int getMaxMatch() {
        cardinality = 0;
        for (int v = 0; v < MAXV; v++) if (color[v]) typeA.push_back(v);
        while (hasPath()) for (int v : typeA) if (mate[v] == -1 && dfs(v)) cardinality++;
        return cardinality;
    }
};
