#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum spanning tree using Prims's algorithm.
// Time Complexity: O(E log E) or O(E log V) if an indexed priority queue is used
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct PrimMST {
    unit INF; PrimMST(unit INF) : INF(INF) {}
    struct Edge { int v, w; unit weight; };
    pair<int, unit> to[MAXV]; unit weight, cost[MAXV]; vector<Edge> mst; bool vis[MAXV]; vector<pair<int, unit>> adj[MAXV];
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); adj[w].emplace_back(v, weight); }
    unit run(int V) {
        weight = 0; fill(vis, vis + V, false); fill(cost, cost + V, INF); fill(to, to + V, make_pair(-1, 0));
        std::priority_queue<pair<unit, int>, vector<pair<unit, int>>, greater<pair<unit, int>>> PQ;
        for (int s = 0; s < V; s++) if (!vis[s]) {
            PQ.emplace(cost[s] = 0, s);
            while (!PQ.empty()) {
                int v = PQ.top().second; PQ.pop(); vis[v] = true;
                for (auto &&e : adj[v]) if (!vis[e.first] && e.second < cost[e.first]) {
                    to[e.first] = {v, e.second}; PQ.emplace(cost[e.first] = e.second, e.first);
                }
            }
        }
        for (int v = 0; v < V; v++) if (to[v].first != -1) {
            mst.push_back({v, to[v].first, to[v].second}); weight += to[v].second;
        }
        return weight;
    }
    void clear() { mst.clear(); for (int i = 0; i < MAXV; i++) adj[i].clear(); }
};
