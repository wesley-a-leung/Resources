#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the maximum flow using a path with the minimum cost by finding Shortest Augmenting Paths
// Time Complexity: O(E^2 V log V), much faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class flowUnit, class costUnit> struct SAPMinCostMaxFlow {
    flowUnit FLOW_INF, FLOW_EPS; costUnit COST_INF;
    using heap = __gnu_pbds::priority_queue<pair<costUnit, int>, greater<pair<costUnit, int>>, pairing_heap_tag>;
    SAPMinCostMaxFlow(flowUnit FLOW_INF, flowUnit FLOW_EPS, costUnit COST_INF) : FLOW_INF(FLOW_INF), FLOW_EPS(FLOW_EPS), COST_INF(COST_INF) {}
    struct Edge {
        int from, to; flowUnit cap, resCap; costUnit cost; int ind, rev; Edge() {}
        Edge(int from, int to, flowUnit cap, costUnit cost) : from(from), to(to), cap(cap), resCap(cap), cost(cost) {}
        bool operator < (const Edge &other) const { return from < other.from; }
    };
    int E, prev[MAXV], index[MAXV], st[MAXV], ind[MAXE * 2]; Edge e[MAXE * 2];
    flowUnit maxFlow; costUnit phi[MAXV], dist[MAXV], minCost; bool hasNegativeEdgeCost; typename heap::point_iterator ptr[MAXV];
    void addEdge(int v, int w, flowUnit flow, costUnit cost) {
        if (cost < 0) hasNegativeEdgeCost = true;
        e[E++] = Edge(v, w, flow, cost); e[E++] = Edge(w, v, 0, -cost);
        e[E - 2].ind = E - 2; e[E - 1].ind = E - 1; e[E - 2].rev = E - 1; e[E - 1].rev = E - 2;
    }
    void bellmanFord(int V, int s, int t) {
        fill(phi, phi + V, COST_INF); phi[s] = 0;
        for (int j = 0; j < V - 1; j++) for (int i = 0; i < E; i++)
            if (e[i].resCap > FLOW_EPS) phi[e[i].to] = min(phi[e[i].to], phi[e[i].from] + e[i].cost);
    }
    bool dijkstra(int V, int s, int t) {
        fill(dist, dist + V, COST_INF); fill(prev, prev + V, -1); fill(index, index + V, -1);
        heap PQ; fill(ptr, ptr + V, PQ.end()); ptr[s] = PQ.push({dist[s] = 0, s});
        while (!PQ.empty()) {
            int v = PQ.top().second; PQ.pop();
            for (int i = st[v]; i < st[v + 1]; i++) {
                if (e[i].resCap <= FLOW_EPS) continue;
                int w = e[i].to; costUnit d = dist[v] + e[i].cost + phi[v] - phi[w];
                if (dist[w] <= d) continue;
                prev[w] = v; index[w] = i;
                if (ptr[w] == PQ.end()) ptr[w] = PQ.push({dist[w] = d, w});
                else PQ.modify(ptr[w], {dist[w] = d, w});
            }
        }
        return dist[t] != COST_INF;
    }
    void init(int V) { E = 0; hasNegativeEdgeCost = false; }
    void build(int V) {
        fill(st, st + V + 1, 0); sort(e, e + E);
        for (int i = 0; i < E; i++) st[e[ind[e[i].ind] = i].to + 1]++;
        partial_sum(st, st + V + 1, st);
        for (int i = 0; i < E; i++) e[e[i].ind = i].rev = ind[e[i].rev];
    }
    pair<flowUnit, costUnit> getMaxFlowMinCost(int V, int s, int t) {
        build(V); maxFlow = 0; minCost = 0; fill(phi, phi + V, 0);
        if (hasNegativeEdgeCost) bellmanFord(V, s, t);
        while (dijkstra(V, s, t)) {
            flowUnit aug = FLOW_INF; int cur = t;
            while (prev[cur] != -1) { aug = min(aug, e[index[cur]].resCap); cur = prev[cur]; }
            maxFlow += aug; cur = t;
            while (prev[cur] != -1) {
                e[index[cur]].resCap -= aug; e[e[index[cur]].rev].resCap += aug; minCost += aug * e[index[cur]].cost; cur = prev[cur];
            }
            for (int v = 0; v < V; v++) if (dist[v] != COST_INF) phi[v] += dist[v];
        }
        return {maxFlow, minCost};
    }
};
