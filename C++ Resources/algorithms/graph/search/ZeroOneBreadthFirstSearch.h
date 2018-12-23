#pragma once
#include <bits/stdc++.h>
using namespace std;

// Breadth First Traversal of a graph where all edge weights are 0 or 1
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct BFS {
    int dist[MAXV], to[MAXV]; vector<pair<int, int>> adj[MAXV];
    void addEdge(int v, int w, int weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, int weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, const vector<int> &srcs) {
        fill(dist, dist + V, INT_MAX); fill(to, to + V, -1); deque<int> dq;
        for (int s : srcs) { dist[s] = 0; dq.push_front(s); }
        while (!dq.empty()) {
            int v = dq.front(); dq.pop_front();
            for (auto &&e : adj[v]) if (dist[e.first] == INT_MAX) {
                dist[e.first] = dist[v] + e.second; to[e.first] = v;
                if (e.second == 0) dq.push_front(e.first);
                else dq.push_back(e.first);
            }
        }
    }
    void run(int V, int s) { run(V, vector<int>(1, s)); }
};
