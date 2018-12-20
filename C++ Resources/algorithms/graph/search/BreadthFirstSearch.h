#pragma once
#include <bits/stdc++.h>
using namespace std;

// Breadth First Traversal of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct BFS {
    int dist[MAXV], to[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear() { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    void run(const vector<int> &srcs) {
        fill(dist, dist + MAXV, INT_MAX); fill(to, to + MAXV, -1); queue<int> q;
        for (int s : srcs) { dist[s] = 0; q.push(s); }
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int w : adj[v]) if (dist[w] == INT_MAX) { dist[w] = dist[v] + 1; to[w] = v; q.push(w); }
        }
    }
    void run(int s) { run(vector<int>(1, s)); }
};
