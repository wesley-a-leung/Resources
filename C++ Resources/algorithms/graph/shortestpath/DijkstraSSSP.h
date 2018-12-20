#pragma once
#include <bits/stdc++.h>
using namespace std;

// Dijstra's single source shortest path algorithm for weighted graphs without negative cycles
// Time Complexity: O(E log E) or O(E log V) if an indexed priority queue is used
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct DijkstraSSSP {
    unit INF, dist[MAXV]; pair<int, unit> to[MAXV]; vector<pair<int, unit>> adj[MAXV]; DijkstraSSSP(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear() { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    void run(int V, int s) {
        priority_queue<pair<unit, int>, vector<pair<unit, int>>, greater<pair<unit, int>>> PQ;
        fill(dist, dist + V, INF); fill(to, to + V, make_pair(-1, 0)); PQ.emplace(dist[s] = 0, s);
        while (!PQ.empty()) {
            unit d = PQ.top().first; int v = PQ.top().second; PQ.pop();
            if (d > dist[v]) continue;
            for (auto &&e : adj[v]) if (dist[e.first] > dist[v] + e.second) {
                to[e.first] = {v, e.second}; PQ.emplace(dist[e.first] = dist[v] + e.second, e.first);
            }
        }
    }
};
