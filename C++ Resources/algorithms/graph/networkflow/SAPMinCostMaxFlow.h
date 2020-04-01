#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the maximum flow using a path with the minimum cost by finding Shortest Augmenting Paths
// Time Complexity: O(E^2 V log V), much faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class flowUnit, class costUnit> struct SAPMinCostMaxFlow {
    flowUnit FLOW_INF, FLOW_EPS; costUnit COST_INF;
    using heap = __gnu_pbds::priority_queue<pair<costUnit, int>, greater<pair<costUnit, int>>, pairing_heap_tag>;
    SAPMinCostMaxFlow(flowUnit FLOW_INF, flowUnit FLOW_EPS, costUnit COST_INF) : FLOW_INF(FLOW_INF), FLOW_EPS(FLOW_EPS), COST_INF(COST_INF) {}
    struct Edge {
        int to; flowUnit cap, resCap; costUnit cost; int rev;
        Edge(int to, flowUnit cap, costUnit cost, int rev) : to(to), cap(cap), resCap(cap), cost(cost), rev(rev) {}
    };
    int prev[MAXV]; Edge *to[MAXV]; vector<Edge> adj[MAXV]; typename heap::point_iterator ptr[MAXV];
    flowUnit maxFlow; costUnit phi[MAXV], dist[MAXV], minCost; bool hasNegativeEdgeCost;
    void addEdge(int v, int w, flowUnit flow, costUnit cost) {
        if (cost < 0) hasNegativeEdgeCost = true;
        adj[v].emplace_back(w, flow, cost, int(adj[w].size())); adj[w].emplace_back(v, 0, -cost, int(adj[v].size()) - 1);
    }
    void bellmanFord(int V, int s, int t) {
        fill(phi, phi + V, COST_INF); phi[s] = 0;
        for (int j = 0; j < V - 1; j++) for (int v = 0; v < V; v++) for (auto &&e : adj[v])
            if (e.resCap > FLOW_EPS) phi[e.to] = min(phi[e.to], phi[v] + e.cost);
    }
    bool dijkstra(int V, int s, int t) {
        fill(dist, dist + V, COST_INF); fill(prev, prev + V, -1);
        heap PQ; fill(ptr, ptr + V, PQ.end()); ptr[s] = PQ.push({dist[s] = 0, s});
        while (!PQ.empty()) {
            int v = PQ.top().second; PQ.pop();
            for (auto &&e : adj[v]) {
                if (e.resCap <= FLOW_EPS) continue;
                int w = e.to; costUnit d = dist[v] + e.cost + phi[v] - phi[w];
                if (dist[w] <= d) continue;
                prev[w] = v; to[w] = &e;
                if (ptr[w] == PQ.end()) ptr[w] = PQ.push({dist[w] = d, w});
                else PQ.modify(ptr[w], {dist[w] = d, w});
            }
        }
        return dist[t] != COST_INF;
    }
    void init(int V) { hasNegativeEdgeCost = false; for (int i = 0; i < V; i++) adj[i].clear(); }
    pair<flowUnit, costUnit> getMaxFlowMinCost(int V, int s, int t) {
        maxFlow = 0; minCost = 0; fill(phi, phi + V, 0);
        if (hasNegativeEdgeCost) bellmanFord(V, s, t);
        while (dijkstra(V, s, t)) {
            flowUnit aug = FLOW_INF;
            for (int cur = t; prev[cur] != -1; cur = prev[cur]) aug = min(aug, to[cur]->resCap);
            maxFlow += aug;
            for (int cur = t; prev[cur] != -1; cur = prev[cur]) {
                to[cur]->resCap -= aug; adj[to[cur]->to][to[cur]->rev].resCap += aug; minCost += aug * to[cur]->cost;
            }
            for (int v = 0; v < V; v++) if (dist[v] != COST_INF) phi[v] += dist[v];
        }
        return make_pair(maxFlow, minCost);
    }
};
