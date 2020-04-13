#pragma once
#include <bits/stdc++.h>
using namespace std;

// Dijstra's single source shortest path algorithm for weighted graphs without negative weights
// Time Complexity:
//   O(E log E) if a regular priority queue is used
//   O(E log V) if an indexed priority queue or pairing heap is used
//   O(E log (sum of weights)) if all weights are integers and a radix heap is used
//   If there is a negative cycle, then it may not terminate
//   If there are negative weights, but no negative cycles, the time complexity can become exponential
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct DijkstraSSSP {
    unit INF, dist[MAXV]; pair<int, unit> to[MAXV]; vector<pair<int, unit>> adj[MAXV]; DijkstraSSSP(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, const vector<int> &src) {
        std::priority_queue<pair<unit, int>, vector<pair<unit, int>>, greater<pair<unit, int>>> PQ;
        fill(dist, dist + V, INF); fill(to, to + V, make_pair(-1, 0));
        for (int s : src) PQ.emplace(dist[s] = 0, s);
        while (!PQ.empty()) {
            unit d = PQ.top().first; int v = PQ.top().second; PQ.pop();
            if (d > dist[v]) continue;
            for (auto &&e : adj[v]) if (dist[e.first] > dist[v] + e.second) {
                to[e.first] = make_pair(v, e.second); PQ.emplace(dist[e.first] = dist[v] + e.second, e.first);
            }
        }
    }
    void run(int V, int s) { run(V, vector<int>(1, s)); }
};
