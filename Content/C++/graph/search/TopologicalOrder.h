#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the Topological of a directed graph iteratively
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct TopologicalOrder {
    int ind[MAXV], ord[MAXV], inDeg[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); inDeg[w]++; }
    void init(int V = MAXV) { for (int i = 0; i < V; i++) { adj[i].clear(); inDeg[i] = 0; } }
    void run(int V) {
        int front = 0, back = 0; fill(ind, ind + V, -1);
        for (int v = 0; v < V; v++) if (inDeg[v] == 0) ord[back++] = v;
        while (front < back) {
            int v = ord[front]; ind[v] = front++;
            for (int w : adj[v]) if (--inDeg[w] == 0) ord[back++] = w;
        }
    }
};
