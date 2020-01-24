#pragma once
#include <bits/stdc++.h>
using namespace std;

// Classical Dijkstra's single source shortest path algorithm for weighted graphs without negative weights
// Time Complexity: O(V^2 + E)
//   Always guaranteed to terminate, but if there are negative weights, the distance array may be incorrect
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct ClassicalDijkstraSSSP {
    unit INF, dist[MAXV]; pair<int, unit> to[MAXV]; bool done[MAXV]; vector<pair<int, unit>> adj[MAXV];
    ClassicalDijkstraSSSP(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, const vector<int> &src) {
        fill(dist, dist + V, INF); fill(to, to + V, make_pair(-1, 0)); fill(done, done + V, false);
        for (int s : src) dist[s] = 0;
        for (int i = 0; i < V - 1; i++) {
            int v = -1;
            for (int w = 0; w < V; w++) if (!done[w] && (v == -1 || dist[v] > dist[w])) v = w;
            done[v] = true;
            for (auto &&e : adj[v]) if (dist[e.first] > dist[v] + e.second) {
                dist[e.first] = dist[v] + e.second; to[e.first] = make_pair(v, e.second);
            }
        }
    }
    void run(int V, int s) { run(V, vector<int>(1, s)); }
};
