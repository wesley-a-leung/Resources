#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the maximum flow using a path with the minimum cost by finding
//   Shortest Augmenting Paths
// Can handle negative edges but not negative cost cycles
//   (assertion failure if there is a negative cost cycle)
// Vertices are 0-indexed
// Template Arguments:
//   FlowUnit: the type of the flow
//   CostUnit: the type of the cost, must be integral
// Constructor Arguments:
//   V: the number of vertices in the flow network
//   FLOW_EPS: a value for the flow epsilon
//   COST_INF: a value for the cost infinity
// Fields:
//   V: the number of vertices in the flow network
//   FLOW_EPS: a value for the flow epsilon
//   COST_INF: a value for the cost infinity
//   G: an adjacency list of all edges and reverse edges in the flow network
// Functions:
//   addEdge(v, w, vwCap, vwCost): adds an edge from v to w with capacity
//     vwCap and cost vwCost
//   getFlowMinCost(s, t): returns the maximum flow from s to t with
//     the minimum cost
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(1)
//   getFlowMinCost: O(E^2 V log V)
// Memory Complexity: O(V + E)
// Tested:
//   https://open.kattis.com/problems/mincostmaxflow
//   https://loj.ac/p/102
//   https://www.spoj.com/problems/SCITIES/
//   https://dmoj.ca/problem/tle17c7p5
template <class FlowUnit, class CostUnit> struct SAPMinCostMaxFlow {
  struct Edge {
    int to, rev; FlowUnit cap, resCap; CostUnit cost;
    Edge(int to, int rev, FlowUnit cap, CostUnit cost)
        : to(to), rev(rev), cap(cap), resCap(cap), cost(cost) {}
  };
  using heap = __gnu_pbds::priority_queue<pair<CostUnit, int>,
                                          greater<pair<CostUnit, int>>,
                                          pairing_heap_tag>;
  int V; FlowUnit FLOW_EPS; CostUnit COST_INF; vector<vector<Edge>> G;
  bool hasNegativeCost; vector<CostUnit> phi, dist;
  vector<Edge*> to; vector<int> par; 
  SAPMinCostMaxFlow(int V, FlowUnit FLOW_EPS = FlowUnit(1e-9),
                    CostUnit COST_INF = numeric_limits<CostUnit>::max())
      : V(V), FLOW_EPS(FLOW_EPS), COST_INF(COST_INF), G(V),
        hasNegativeCost(false), phi(V), dist(V), to(V), par(V) {}
  void addEdge(int v, int w, FlowUnit vwCap, CostUnit vwCost) {
    if (v == w) return;
    if (vwCost < CostUnit()) hasNegativeCost = true;
    G[v].emplace_back(w, int(G[w].size()), vwCap, vwCost);
    G[w].emplace_back(v, int(G[v].size()) - 1, FlowUnit(), -vwCost);
  }
  void bellmanFord(int s) {
    fill(phi.begin(), phi.end(), COST_INF); phi[s] = CostUnit();
    for (int j = 0; j < V - 1; j++) for (int v = 0; v < V; v++)
      for (auto &&e : G[v]) if (e.resCap > FLOW_EPS && phi[v] < COST_INF)
        phi[e.to] = min(phi[e.to], phi[v] + e.cost);
    for (int v = 0; v < V; v++) for (auto &&e : G[v])
      if (e.resCap > FLOW_EPS && phi[v] < COST_INF)
        assert(phi[e.to] <= phi[v] + e.cost);
  }
  bool dijkstra(int s, int t) {
    fill(dist.begin(), dist.end(), COST_INF); fill(par.begin(), par.end(), -1);
    vector<bool> seen(V, false);
    heap PQ; vector<typename heap::point_iterator> ptr(V, PQ.end());
    ptr[s] = PQ.push(make_pair(dist[s] = CostUnit(), s)); while (!PQ.empty()) {
      int v = PQ.top().second; PQ.pop(); ptr[v] = PQ.end(); seen[v] = true;
      for (auto &&e : G[v]) {
        int w = e.to; if (seen[w] || e.resCap <= FLOW_EPS) continue;
        CostUnit d = dist[v] + e.cost + phi[v] - phi[w];
        if (dist[w] <= d) continue;
        par[w] = v; to[w] = &e;
        if (ptr[w] == PQ.end()) ptr[w] = PQ.push(make_pair(dist[w] = d, w));
        else PQ.modify(ptr[w], make_pair(dist[w] = d, w));
      }
    }
    return dist[t] < COST_INF;
  }
  pair<FlowUnit, CostUnit> getFlowMinCost(int s, int t) {
    pair<FlowUnit, CostUnit> ret = make_pair(FlowUnit(), CostUnit());
    fill(phi.begin(), phi.end(), CostUnit()); if (s == t) return ret;
    if (hasNegativeCost) bellmanFord(s);
    while (dijkstra(s, t)) {
      FlowUnit aug = FlowUnit(); for (int v = t; par[v] != -1; v = par[v])
        if (v == t || aug > to[v]->resCap) aug = to[v]->resCap;
      ret.first += aug; for (int v = t; par[v] != -1; v = par[v]) {
        to[v]->resCap -= aug; G[to[v]->to][to[v]->rev].resCap += aug;
        ret.second += aug * to[v]->cost;
      }
      for (int v = 0; v < V; v++) if (dist[v] < COST_INF) phi[v] += dist[v];
    }
    return ret;
  }
};
