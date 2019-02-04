#pragma once
#include <bits/stdc++.h>
using namespace std;

// Breadth First Traversal of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct BFS {
    int dist[MAXV], to[MAXV], q[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, const vector<int> &srcs) {
        fill(dist, dist + V, INT_MAX); fill(to, to + V, -1); int front = 0, back = 0;
        for (int s : srcs) { dist[s] = 0; q[back++] = s; }
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v]) if (dist[w] == INT_MAX) { dist[w] = dist[v] + 1; to[w] = v; q[back++] = w; }
        }
    }
    void run(int V, int s) { run(V, vector<int>(1, s)); }
};
