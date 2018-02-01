#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_MAXFLOWMINCOSTCOMPACT_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_MAXFLOWMINCOSTCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

typedef int flowUnit;
typedef int costUnit;
const flowUnit FLOW_INF = (1 << 30);
const flowUnit FLOW_EPS = 0;
const costUnit COST_LARGE_INF = (1 << 30);
const costUnit COST_SMALL_INF = (1 << 25);

/**
 * Computes the maximum flow using that path with the minimum cost.
 *
 *
 * Time: O(VEB log V) where B is upper bound on the largest supply of any node
 * Memory: O(V + E)
 */
class MaxFlowMinCost {
private:
    struct Edge {
        int orig, dest;
        costUnit origCost, cost;
        flowUnit flow;
        int last;
        Edge(int orig, int dest, costUnit cost, flowUnit flow, int last) : orig(orig), dest(dest), origCost(cost), cost(cost), flow(flow), last(last) {}
    };

    struct Vertex {
        int index;
        costUnit cost;
        Vertex(int index, costUnit cost) : index(index), cost(cost) {}
        bool operator < (const Vertex& v) const { return cost < v.cost; }
    };

    int N, src, sink, cnt = 0;
    vector<Edge> e;
    vector<int> last, prev, index;
    vector<costUnit> phi, dist;

    void reduceCost() {
        for (int i = 0; i < (int) e.size(); i += 2) {
            e[i].cost += phi[e[i].orig] - phi[e[i].dest];
            e[i ^ 1].cost = 0;
        }
    }

    void bellmanFord() {
        fill(phi.begin(), phi.end(), COST_SMALL_INF);
        phi[src] = 0;
        for (int j = 0; j < N - 1; j++)
            for (int i = 0; i < (int) e.size(); i++)
                if (e[i].flow > FLOW_EPS) phi[e[i].dest] = min(phi[e[i].dest], phi[e[i].orig] + e[i].cost);
    }

    bool dijkstra() {
        fill(dist.begin(), dist.end(), COST_LARGE_INF);
        fill(prev.begin(), prev.end(), -1);
        fill(index.begin(), index.end(), -1);
        dist[src] = 0;
        priority_queue<Vertex> pq;
        pq.push({src, 0});
        while (!pq.empty()) {
            Vertex cur = pq.top();
            pq.pop();
            for (int next = last[cur.index]; next != -1; next = e[next].last) {
                if (abs(e[next].flow) <= FLOW_EPS || dist[e[next].dest] <= dist[cur.index] + e[next].cost) continue;
                dist[e[next].dest] = dist[cur.index] + e[next].cost;
                prev[e[next].dest] = cur.index;
                index[e[next].dest] = next;
                pq.push({e[next].dest, dist[e[next].dest]});
            }
        }
        return dist[sink] != COST_LARGE_INF;
    }

public:
    MaxFlowMinCost(int N, int src, int sink) : N(N), src(src), sink(sink), last(N), prev(N), index(N), phi(N), dist(N) {
        fill(last.begin(), last.end(), -1);
    }

    void addEdge(int u, int v, flowUnit flow, costUnit cost) {
        e.push_back({u, v, cost, flow, last[u]});
        last[u] = (int) e.size() - 1;
        e.push_back({v, u, -cost, 0, last[v]});
        last[v] = (int) e.size() - 1;
    }

    pair<flowUnit, costUnit> getMaxFlowMinCost() {
        flowUnit flow = 0;
        costUnit cost = 0;
        bellmanFord();
        reduceCost();
        while (dijkstra()) {
            for (int i = 0; i < N; i++) phi[i] = dist[i];
            reduceCost();
            flowUnit aug = FLOW_INF;
            int cur = sink;
            while (prev[cur] != -1) {
                aug = min(aug, e[index[cur]].flow);
                cur = prev[cur];
            }
            flow += aug;
            cur = sink;
            while (prev[cur] != -1) {
                e[index[cur]].flow -= aug;
                e[index[cur] ^ 1].flow += aug;
                cost += aug * e[index[cur]].origCost;
                cur = prev[cur];
            }
        }
        return {flow, cost};
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_MAXFLOWMINCOSTCOMPACT_H_ */
