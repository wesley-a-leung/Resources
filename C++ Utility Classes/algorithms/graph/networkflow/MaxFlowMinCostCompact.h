#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_MAXFLOWMINCOSTCOMPACT_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_MAXFLOWMINCOSTCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Computes the maximum flow using that path with the minimum cost.
 *
 *
 * Time: O(VEB log V) where B is upper bound on the largest supply of any node
 * Memory: O(V + E)
 */
typedef int flowUnit;
typedef int costUnit;
const flowUnit FLOW_INF = 1 << 30;
const flowUnit FLOW_EPS = 0;
const costUnit COST_INF = 1 << 30;
const costUnit COST_SMALL_INF = 1 << 25;

class MaxFlowMinCost {
private:
    struct Edge {
        int from, to;
        costUnit origCost, cost;
        flowUnit cap;
        int next;
        Edge(int from, int to, costUnit cost, flowUnit cap, int next) : from(from), to(to), origCost(cost), cost(cost), cap(cap), next(next) {}
    };

    int N, src = 0, sink = 0;
    vector<Edge> e;
    vector<int> last, prev, index;
    vector<costUnit> phi, dist;
    bool hasNegativeEdgeCost = false;

    void bellmanFord() {
        fill(phi.begin(), phi.end(), COST_SMALL_INF);
        phi[src] = 0;
        for (int j = 0; j < N - 1; j++) for (int i = 0; i < (int) e.size(); i++) if (e[i].cap > FLOW_EPS) phi[e[i].to] = min(phi[e[i].to], phi[e[i].from] + e[i].cost);
    }

    bool dijkstra() {
        fill(dist.begin(), dist.end(), COST_INF);
        fill(prev.begin(), prev.end(), -1);
        fill(index.begin(), index.end(), -1);
        dist[src] = 0;
        priority_queue<pair<costUnit, int>, vector<pair<costUnit, int>>, greater<pair<costUnit, int>>> PQ;
        PQ.push({dist[src], src});
        while (!PQ.empty()) {
            pair<costUnit, int> v = PQ.top();
            PQ.pop();
            if (v.first > dist[v.second]) continue;
            for (int next = last[v.second]; next != -1; next = e[next].next) {
                if (abs(e[next].cap) <= FLOW_EPS) continue;
                costUnit d = dist[v.second] + e[next].cost + phi[v.second] - phi[e[next].to];
                if (dist[e[next].to] <= d) continue;
                dist[e[next].to] = d;
                prev[e[next].to] = v.second;
                index[e[next].to] = next;
                PQ.push({dist[e[next].to], e[next].to});
            }
        }
        return dist[sink] != COST_INF;
    }

public:
    MaxFlowMinCost(int N) : N(N), last(N, -1), prev(N), index(N), phi(N), dist(N) {}

    void addEdge(int u, int v, flowUnit flow, costUnit cost) {
        if (cost < 0) hasNegativeEdgeCost = true;
        e.push_back({u, v, cost, flow, last[u]});
        last[u] = (int) e.size() - 1;
        e.push_back({v, u, -cost, 0, last[v]});
        last[v] = (int) e.size() - 1;
    }

    pair<flowUnit, costUnit> getMaxFlowMinCost(int src, int sink) {
        this->src = src;
        this->sink = sink;
        flowUnit flow = 0;
        costUnit cost = 0;
        fill(phi.begin(), phi.end(), 0);
        if (hasNegativeEdgeCost) bellmanFord();
        while (dijkstra()) {
            flowUnit aug = FLOW_INF;
            int cur = sink;
            while (prev[cur] != -1) {
                aug = min(aug, e[index[cur]].cap);
                cur = prev[cur];
            }
            flow += aug;
            cur = sink;
            while (prev[cur] != -1) {
                e[index[cur]].cap -= aug;
                e[index[cur] ^ 1].cap += aug;
                cost += aug * e[index[cur]].origCost;
                cur = prev[cur];
            }
            for (int v = 0; v < N; v++) if (dist[v] != COST_INF) phi[v] += dist[v];
        }
        return {flow, cost};
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_MAXFLOWMINCOSTCOMPACT_H_ */
