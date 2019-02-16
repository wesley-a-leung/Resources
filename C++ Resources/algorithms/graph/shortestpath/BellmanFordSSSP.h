#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bellman Ford's single source shortest path algorithm for weighted graphs with negative weights
// Can be used to detect negative cycles
// Time Compleity: O(VE)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct BellmanFordSSSP {
    struct Edge { int v, w; unit weight; };
    unit INF, dist[MAXV]; pair<int, unit> to[MAXV]; vector<Edge> edges; bool hasNegativeCycle; BellmanFordSSSP(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { edges.push_back({v, w, weight}); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear() { edges.clear(); }
    void run(int V, int s) {
        fill(dist, dist + V, INF); fill(to, to + V, make_pair(-1, 0)); hasNegativeCycle = false; dist[s] = 0; bool inCycle = true;
        for (int i = 0; i < V - 1; i++) for (auto &&e : edges) if (dist[e.v] < INF && dist[e.w] > dist[e.v] + e.weight) {
            dist[e.w] = dist[e.v] + e.weight; to[e.w] = make_pair(e.v, e.weight);
        }
        while (inCycle) {
            inCycle = false;
            for (auto &&e : edges) if (dist[e.v] < INF && dist[e.w] > -INF && dist[e.w] > dist[e.v] + e.weight) {
                dist[e.w] = -INF; inCycle = true; hasNegativeCycle = true;
            }
        }
    }
};
