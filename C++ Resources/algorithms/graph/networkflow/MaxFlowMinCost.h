#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow using a path with the minimum cost
// Time Complexity: O(VEB log V) where B the the upper bound on the largest supply of any node
// Memory Complexity: O(V + E)
template <const int MAXV, class flowUnit, class costUnit> struct MaxFlowMinCost {
    flowUnit FLOW_INF, FLOW_EPS; costUnit COST_INF, COST_SMALL_INF;
    MaxFlowMinCost(flowUnit FLOW_INF, flowUnit FLOW_EPS, costUnit COST_INF, costUnit COST_SMALL_INF) :
        FLOW_INF(FLOW_INF), FLOW_EPS(FLOW_EPS), COST_INF(COST_INF), COST_SMALL_INF(COST_SMALL_INF) {}
    struct Edge {
        int from, to; flowUnit cap; costUnit origCost, cost; int next;
        Edge(int from, int to, flowUnit cap, costUnit cost, int next) :
            from(from), to(to), cap(cap), origCost(cost), cost(cost), next(next) {}
    };
    int last[MAXV], prev[MAXV], index[MAXV]; costUnit phi[MAXV], dist[MAXV]; vector<Edge> e; bool hasNegativeEdgeCost;
    void addEdge(int u, int v, flowUnit flow, costUnit cost) {
        if (cost < 0) hasNegativeEdgeCost = true;
        e.emplace_back(u, v, flow, cost, last[u]); last[u] = int(e.size()) - 1;
        e.emplace_back(v, u, 0, -cost, last[v]); last[v] = int(e.size()) - 1;
    }
    void bellmanFord(int V, int s, int t) {
        fill(phi, phi + V, COST_SMALL_INF); phi[s] = 0;
        for (int j = 0; j < V - 1; j++) for (int i = 0; i < int(e.size()); i++)
            if (e[i].cap > FLOW_EPS) phi[e[i].to] = min(phi[e[i].to], phi[e[i].from] + e[i].cost);
    }
    bool dijkstra(int V, int s, int t) {
        fill(dist, dist + V, COST_INF); fill(prev, prev + V, -1); fill(index, index + V, -1);
        priority_queue<pair<costUnit, int>, vector<pair<costUnit, int>>, greater<pair<costUnit, int>>> PQ;
        PQ.emplace(dist[s] = 0, s);
        while (!PQ.empty()) {
            pair<costUnit, int> v = PQ.top(); PQ.pop();
            if (v.first > dist[v.second]) continue;
            for (int next = last[v.second]; next != -1; next = e[next].next) {
                if (abs(e[next].cap) <= FLOW_EPS) continue;
                costUnit d = dist[v.second] + e[next].cost + phi[v.second] - phi[e[next].to];
                if (dist[e[next].to] <= d) continue;
                prev[e[next].to] = v.second; index[e[next].to] = next;
                PQ.emplace(dist[e[next].to] = d, e[next].to);
            }
        }
        return dist[t] != COST_INF;
    }
    void init(int V = MAXV) { fill(last, last + V, -1); hasNegativeEdgeCost = false; }
    void clear() { e.clear(); }
    pair<flowUnit, costUnit> getMaxFlowMinCost(int V, int s, int t) {
        flowUnit flow = 0; costUnit cost = 0; fill(phi, phi + V, 0);
        if (hasNegativeEdgeCost) bellmanFord(V, s, t);
        while (dijkstra(V, s, t)) {
            flowUnit aug = FLOW_INF; int cur = t;
            while (prev[cur] != -1) { aug = min(aug, e[index[cur]].cap); cur = prev[cur]; }
            flow += aug; cur = t;
            while (prev[cur] != -1) {
                e[index[cur]].cap -= aug; e[index[cur] ^ 1].cap += aug;
                cost += aug * e[index[cur]].origCost; cur = prev[cur];
            }
            for (int v = 0; v < V; v++) if (dist[v] != COST_INF) phi[v] += dist[v];
        }
        return {flow, cost};
    }
};
