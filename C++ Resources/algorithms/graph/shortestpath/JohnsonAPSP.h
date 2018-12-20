#pragma once
#include <bits/stdc++.h>
using namespace std;

// Johnson's all pairs shortest path algorithm for weighted graphs
// Can be used to detect negative cycles
// Time Complexity: O(VE log E) or O(VE log V) if an indexed priority queue is used
// Memory Complexity: O(V^2 + E)
template <const int MAXV, class unit> struct JohnsonAPSP {
    unit INF, dist[MAXV][MAXV], h[MAXV]; JohnsonAPSP(unit INF) : INF(INF) {}
    pair<int, unit> to[MAXV][MAXV]; bool hasNegativeCycle; vector<pair<int, unit>> adj[MAXV];
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear() { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    void run(int V) {
        for (int v = 0; v < V; v++) { fill(dist[v], dist[v] + V, INF); fill(to[v], to[v] + V, make_pair(-1, 0)); }
        fill(h, h + V, INF); h[V] = 0;
        for (int v = 0; v < V; v++) adj[V].emplace_back(v, 0);
        for (int i = 0; i < V; i++) for (int v = 0; v <= V; v++) for (auto &&e : adj[v])
            if (h[v] < INF && h[e.first] > h[v] + e.second) h[e.first] = h[v] + e.second;
        adj[V].clear();
        bool inCycle = true;
        while (inCycle) {
            inCycle = false;
            for (int v = 0; v <= V; v++) for (auto &&e : adj[v]) {
                if (h[v] < INF && h[e.first] > -INF && h[e.first] > h[v] + e.second) {
                    h[e.first] = -INF; inCycle = true; hasNegativeCycle = true;
                }
            }
        }
        adj[V].clear();
        if (hasNegativeCycle) return;
        for (int s = 0; s < V; s++) {
            priority_queue<pair<unit, int>, vector<pair<unit, int>> , greater<pair<unit, int>>> PQ;
            PQ.emplace(dist[s][s] = 0, s);
            while (!PQ.empty()) {
                unit d = PQ.top().first; int v = PQ.top().second; PQ.pop();
                if (d > dist[s][v]) continue;
                for (auto &&e : adj[v]) if (dist[s][e.first] > dist[s][v] + e.second + h[v] - h[e.first]) {
                    to[s][e.first] = {v, e.second}; PQ.emplace(dist[s][e.first] = dist[s][v] + e.second + h[v] - h[e.first], e.first);
                }
            }
        }
        for (int v = 0; v < V; v++) for (int w = 0; w < V; w++) dist[v][w] = dist[v][w] - h[v] + h[w];
    }
};
