#pragma once
#include <bits/stdc++.h>
using namespace std;

// Johnson's all pairs shortest path algorithm for weighted graphs with negative weights
// Can also detect negative cycles
// Time Complexity:
//   O(VE log E) if a regular priority queue is used
//   O(VE log V) if an indexed priority queue or pairing heap is used
//   O(VE log (sum of weights)) if all weights are integers and a radix heap is used
// Memory Complexity: O(V^2 + E)
template <const int MAXV, class unit> struct JohnsonAPSP {
    unit INF, dist[MAXV][MAXV], h[MAXV]; int q[MAXV], vis[MAXV]; JohnsonAPSP(unit INF) : INF(INF) {}
    pair<int, unit> to[MAXV][MAXV]; bool hasNegativeCycle = false, hasNegativeWeight = false; vector<pair<int, unit>> adj[MAXV];
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); hasNegativeWeight |= weight < 0; }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { hasNegativeCycle = hasNegativeWeight = false; for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V) {
        for (int v = 0; v < V; v++) { fill(dist[v], dist[v] + V, INF); fill(to[v], to[v] + V, make_pair(-1, 0)); }
        if (hasNegativeWeight) {
            fill(h, h + V, INF); h[V] = 0;
            for (int v = 0; v < V; v++) adj[V].emplace_back(v, 0);
            for (int i = 0; i < V; i++) for (int v = 0; v <= V; v++) for (auto &&e : adj[v]) 
                if (h[v] < INF && h[e.first] > h[v] + e.second) h[e.first] = h[v] + e.second;
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
        } else fill(h, h + V, 0);
        fill(vis, vis + V, -1);
        for (int s = 0; s < V; s++) {
            if (h[s] > -INF) {
                std::priority_queue<pair<unit, int>, vector<pair<unit, int>> , greater<pair<unit, int>>> PQ; PQ.emplace(dist[s][s] = 0, s);
                while (!PQ.empty()) {
                    unit d = PQ.top().first; int v = PQ.top().second; PQ.pop();
                    if (d > dist[s][v]) continue;
                    for (auto &&e : adj[v]) if (h[e.first] > -INF && dist[s][e.first] > dist[s][v] + e.second + h[v] - h[e.first]) {
                        to[s][e.first] = make_pair(v, e.second); PQ.emplace(dist[s][e.first] = dist[s][v] + e.second + h[v] - h[e.first], e.first);
                    }
                }
            }
            if (hasNegativeWeight) {
                int front = 0, back = 0; vis[s] = s; q[back++] = s; dist[s][s] = dist[s][s] == INF ? -INF : dist[s][s];
                while (front < back) for (auto &&e : adj[q[front++]]) if (vis[e.first] != s) {
                    vis[e.first] = s; q[back++] = e.first; dist[s][e.first] = dist[s][e.first] == INF ? -INF : dist[s][e.first];
                }
            }
        }
        if (hasNegativeWeight) for (int v = 0; v < V; v++) for (int w = 0; w < V; w++)
            if (abs(dist[v][w]) < INF) dist[v][w] = dist[v][w] - h[v] + h[w];
    }
};
