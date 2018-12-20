#pragma once
#include <bits/stdc++.h>
using namespace std;

// Classical Dijkstra's single source shortest path algorithm for weighted graphs with non negative weights
// Time Complexity: O(V^2)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct ClassicalDijkstraSSSP {
    unit INF, dist[MAXV]; pair<int, unit> to[MAXV]; bool done[MAXV]; vector<pair<int, unit>> adj[MAXV];
    ClassicalDijkstraSSSP(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear() { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    void run(int V, int s) {
        fill(dist, dist + V, INF); fill(to, to + V, make_pair(-1, 0)); fill(done, done + V, false); dist[s] = 0;
        for (int v = 0; v < V - 1; v++) {
            int minV = -1;
            for (int w = 0; w < V; v++) if (!done[w] && (minV == -1 || dist[minV] > dist[w])) minV = w;
            done[minV] = true;
            for (auto &&e : adj[minV]) if (dist[e.first] > dist[minV] + e.second) {
                dist[e.first] = dist[minV] + e.second; to[e.first] = {v, e.second};
            }
        }
    }
};
