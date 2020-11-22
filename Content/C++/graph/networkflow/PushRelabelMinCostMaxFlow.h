#pragma once
#include <bits/stdc++.h>
#include "PushRelabelMaxFlow.h"
using namespace std;

// A sample edge struct for minimum cost maximum flow
// Flow of edge can be found with cap - resCap
// Template Arguments:
//   _FlowUnit: the type of the flow
//   _CostUnit: the type of the cost
// Constructor Arguments:
//   to: the vertex that this directed edge ends at
//   rev: the index in the adjacency list of vertex to of the reverse edge
//   cap: the initial capacity of this edge
//   cost: the cost of this edge
// Fields:
//   FlowUnit: the type of the flow
//   CostUnit: the type of the cost
//   to: the vertex that this directed edge ends at
//   rev: the index in the adjacency list of vertex to of the reverse edge
//   cap: the initial capacity of this edge
//   resCap: the residual capacity of this edge
//   cost: the cost of this edge
// Tested:
//   https://open.kattis.com/problems/mincostmaxflow
//   https://loj.ac/p/102
//   https://uoj.ac/problem/487
//   https://www.spoj.com/problems/SCITIES/
template <class _FlowUnit, class _CostUnit> struct FlowCostEdge {
  using FlowUnit = _FlowUnit; using CostUnit = _CostUnit;
  int to, rev; FlowUnit cap, resCap; CostUnit cost;
  FlowCostEdge(int to, int rev, FlowUnit cap, CostUnit cost)
      : to(to), rev(rev), cap(cap), resCap(cap), cost(cost) {}
};

// Computes the maximum flow with the minimum cost in a flow network using the
//   Push Relabel algorithm with the highest label selection rule and
//   gap relabelling heuristic
// Costs are scaled by a factor that is O(V)
// CostUnit must be integral
// Flow circulations are added to negative cost cycles
// Vertices are 0-indexed
// Template Arguments:
//   Edge: a generic edge class (such as FlowCostEdge)
//     Required Fields:
//       FlowUnit: the type of the flow
//       CostUnit: the type of the cost, must be integral
//       to: the vertex that this directed edge ends at
//       rev: the index in the adjacency list of vertex to of the reverse edge
//       resCap: the residual capacity of this edge
//       cost: the cost of this edge
//     Required Functions:
//       constructor(to, rev, cap, cost): initializes the edge to vertex to
//         with the reverse index rev, capacity cap, and cost cost
// Constructor Arguments:
//   V: the number of vertices in the flow network
//   FLOW_EPS: a value for the flow epsilon
//   COST_INF: a value for the cost infinity
//   COST_EPS: a value for the cost epsilon
// Fields:
//   V: the number of vertices in the flow network
//   FLOW_EPS: a value for the flow epsilon
//   COST_INF: a value for the cost infinity
//   COST_EPS: a value for the cost epsilon
//   G: an adjacency list of all edges and reverse edges in the flow network
// Functions:
//   addEdge(v, w, vwCap, vwCost): adds an edge from v to w with capacity
//     vwCap and cost vwCost
//   getFlowMinCost(s, t): returns the maximum flow (or circulation if s and t
//     are -1) from s to t with the minimum cost
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(1)
//   getFlowMinCost: O(E V^2 log (VC)) where C is the maximum edge cost
// Memory Complexity: O(V + E)
// Tested:
//   https://open.kattis.com/problems/mincostmaxflow
//   https://loj.ac/p/102
//   https://uoj.ac/problem/487
//   https://www.spoj.com/problems/SCITIES/
template <class Edge>
struct PushRelabelMinCostMaxFlow : public PushRelabelMaxFlow<Edge> {
  using MF = PushRelabelMaxFlow<Edge>;
  using FlowUnit = typename Edge::FlowUnit;
  using CostUnit = typename Edge::CostUnit;
  static_assert(is_integral<CostUnit>::value, "CostUnit must be integral");
  using MF::V; using MF::G;
  using MF::FLOW_EPS; CostUnit COST_INF, COST_EPS, negCost;
  PushRelabelMinCostMaxFlow(
      int V, FlowUnit FLOW_EPS = FlowUnit(1e-9),
      CostUnit COST_INF = numeric_limits<CostUnit>::max(),
      CostUnit COST_EPS = CostUnit(1e-9))
      : MF(V, FLOW_EPS), COST_INF(COST_INF), COST_EPS(COST_EPS),
        negCost(CostUnit()) {}
  void addEdge(int v, int w, FlowUnit vwCap, CostUnit vwCost) {
    if (v == w) {
      if (vwCost < CostUnit()) negCost += vwCap * vwCost;
      return;
    }
    G[v].emplace_back(w, int(G[w].size()), vwCap, vwCost);
    G[w].emplace_back(v, int(G[v].size()) - 1, FlowUnit(), -vwCost);
  }
  pair<FlowUnit, CostUnit> getFlowMinCost(int s = -1, int t = -1) {
    CostUnit minCost = CostUnit(), mul = CostUnit(2) << __lg(V);
    CostUnit bnd = CostUnit(); vector<int> stk(V), infs(V, 0); int top = 0;
    vector<CostUnit> phi(V, CostUnit()); vector<FlowUnit> ex(V, FlowUnit());
    auto costP = [&] (int v, const Edge &e) {
      int netInfs = infs[v] - infs[e.to]; if (netInfs > 0) return COST_INF;
      else if (netInfs < 0) return -COST_INF;
      else return e.cost + phi[v] - phi[e.to];
    };
    auto push = [&] (int v, Edge &e, FlowUnit df, bool pushToStack) {
      if (e.resCap < df) df = e.resCap;
      int w = e.to; e.resCap -= df; G[w][e.rev].resCap += df;
      ex[v] -= df; ex[w] += df;
      if (pushToStack && FLOW_EPS < ex[e.to] && ex[e.to] <= df + FLOW_EPS)
        stk[top++] = e.to;
    };
    auto relabel = [&] (int v, CostUnit delta) {
      if (delta < COST_INF)  phi[v] -= delta + bnd;
      else { infs[v]--; phi[v] -= bnd; }
    };
    auto lookAhead = [&] (int v) {
      if (abs(ex[v]) > FLOW_EPS) return false;
      CostUnit delta = COST_INF; for (auto &&e : G[v]) {
        if (e.resCap <= FLOW_EPS) continue;
        CostUnit c = costP(v, e); if (c < -COST_EPS) return false;
        else delta = min(delta, c);
      }
      relabel(v, delta); return true;
    };
    auto discharge = [&] (int v) {
      CostUnit delta = COST_INF; for (int i = 0; i < int(G[v].size()); i++) {
        Edge &e = G[v][i]; if (e.resCap <= FLOW_EPS) continue;
        if (costP(v, e) < -COST_EPS) {
          if (lookAhead(e.to)) { i--; continue; }
          push(v, e, ex[v], true); if (abs(ex[v]) <= FLOW_EPS) return;
        } else delta = min(delta, costP(v, e));
      }
      relabel(v, delta); stk[top++] = v;
    };
    for (int v = 0; v < V; v++) for (auto &&e : G[v]) {
      minCost += e.cost * e.resCap; e.cost *= mul; bnd = max(bnd, e.cost);
    }
    FlowUnit maxFlow = (s == -1 || t == -1) ? FlowUnit() : MF::getFlow(s, t);
    while (bnd > 1) {
      bnd = max(bnd / CostUnit(8), CostUnit(1)); top = 0;
      for (int v = 0; v < V; v++) for (auto &&e : G[v])
        if (costP(v, e) < -COST_EPS && e.resCap > FLOW_EPS)
          push(v, e, e.resCap, false);
      for (int v = 0; v < V; v++) if (ex[v] > FLOW_EPS) stk[top++] = v;
      while (top > 0) discharge(stk[--top]);
    }
    for (int v = 0; v < V; v++)
      for (auto &&e : G[v]) { e.cost /= mul; minCost -= e.cost * e.resCap; }
    return make_pair(maxFlow, (minCost /= 2) += negCost);
  }
};
