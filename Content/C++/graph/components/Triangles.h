#pragma once
#include <bits/stdc++.h>
using namespace std;

// Counts the number of triangles in a graph, ignoring duplicate edges
// Time Complexity: O(E sqrt E)
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE> struct Triangles {
    int E, deg[MAXV], ind[MAXV]; pair<int, int> edges[MAXE]; vector<pair<int, int>> adj[MAXV];
    void addEdge(int v, int w) { edges[E++] = make_pair(min(v, w), max(v, w)); }
    void init() { E = 0; }
    int run(int V) {
        int cnt = 0; sort(edges, edges + E); E = unique(edges, edges + E) - edges;
        for (int v = 0; v < V; v++) { deg[v] = 0; ind[v] = -1; adj[v].clear(); }
        for (int i = 0; i < E; i++) { deg[edges[i].first]++; deg[edges[i].second]++; }
        for (int i = 0, v, w; i < E; i++) {
            tie(v, w) = edges[i];
            if (deg[v] > deg[w]) swap(v, w);
            adj[v].emplace_back(w, i);
        }
        for (int v = 0; v < V; v++) {
            for (auto &&e1 : adj[v]) ind[e1.first] = e1.second;
            for (auto &&e1 : adj[v]) for (auto &&e2 : adj[e1.first]) if (ind[e2.first] != -1) cnt++;
            for (auto &&e1 : adj[v]) ind[e1.first] = -1;
        }
        return cnt;
    }
};
