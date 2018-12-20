#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the Transitive Closure in a directed graph
// Time Complexity: O(V^3 / 64)
// Memory Complexity: O(V^2)
template <const int MAXV> struct TransistiveClosureFloydWarshall {
    bitset<MAXV> vis[MAXV];
    void addEdge(int v, int w) { vis[v][w] = 1; }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear() { for (int i = 0; i < MAXV; i++) vis[i].reset(); }
    void run(int V) {
        for (int v = 0; v < V; v++) vis[v][v] = 1;
        for (int u = 0; u < V; u++) for (int v = 0; v < V; v++) if (vis[v][u]) vis[v] |= vis[u];
    }
};
