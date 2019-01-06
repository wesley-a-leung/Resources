#pragma once
#include <bits/stdc++.h>
#include "../search/DepthFirstOrder.h"
using namespace std;

// Computes the longest path on a directed acyclic graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct DAGLongestPath {
    unit NEG_INF, dist[MAXV]; pair<int, unit> to[MAXV]; vector<pair<int, unit>> adj[MAXV]; DFSOrder<MAXV> ord;
    DAGLongestPath(unit NEG_INF) : NEG_INF(NEG_INF) {}
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); ord.addEdge(v, w); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { ord.clear(V); for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, const vector<int> &src = vector<int>()) { // if src is empty, then all vertices are sources
        fill(dist, dist + V, src.empty() ? 0 : NEG_INF); fill(to, to + V, make_pair(-1, 0)); ord.run(V);
        for (int s : src) dist[s] = 0;
        for (int i = 0; i < V; i++) {
            int v = ord.revPostOrd[i];
            for (auto &&e : adj[v]) if (dist[e.first] < dist[v] + e.second) {
                dist[e.first] = dist[v] + e.second; to[e.first] = make_pair(v, e.second);
            }
        }
    }
    void run(int V, int s) { run(V, vector<int>(1, s)); }
};
