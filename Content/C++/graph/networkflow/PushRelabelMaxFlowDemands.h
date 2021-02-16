#pragma once
#include <bits/stdc++.h>
#include "PushRelabelMaxFlow.h"
using namespace std;

// A sample edge struct for maximum flow with demands
// Flow of edge can be found with cap - resCap
// Template Arguments:
//   _FlowUnit: the type of the flow
// Constructor Arguments:
//   to: the vertex that this directed edge ends at
//   rev: the index in the adjacency list of vertex to of the reverse edge
//   dem: the initial demand of this edge
//   cap: the initial capacity of this edge
// Fields:
//   FlowUnit: the type of the flow
//   to: the vertex that this directed edge ends at
//   rev: the index in the adjacency list of vertex to of the reverse edge
//   dem: the initial demand of this edge
//   cap: the initial capacity of this edge
//   resCap: the residual capacity of this edge
// Tested:
//   https://loj.ac/p/117
//   https://dmoj.ca/problem/wac4p6
//   https://codeforces.com/gym/100199/problem/B
template <class _FlowUnit> struct FlowEdgeDemands {
  using FlowUnit = _FlowUnit; int to, rev; FlowUnit dem, cap, resCap;
  FlowEdgeDemands(int to, int rev, FlowUnit dem, FlowUnit cap)
      : to(to), rev(rev), dem(dem), cap(cap), resCap(cap) {}
};

// Computes the maximum flow in a flow network with edge demands using the
//   Push Relabel algorithm with the highest label selection rule and
//   gap relabelling heuristic
// Vertices are 0-indexed
// Template Arguments:
//   Edge: a generic edge class (such as FlowEdgeDemands)
//     Required Fields:
//       FlowUnit: the type of the flow
//       to: the vertex that this directed edge ends at
//       rev: the index in the adjacency list of vertex to of the reverse edge
//       dem: the initial demand of this edge
//       cap: the initial capacity of this edge
//       resCap: the residual capacity of this edge
//     Required Functions:
//       constructor(to, rev, dem, cap): initializes the edge to vertex to with
//         the reverse index rev, demand dem, and capacity cap
// Constructor Arguments:
//   V: the number of vertices in the flow network
//   FLOW_INF: a value for the flow infinity
//   FLOW_EPS: a value for the flow epsilon
// Fields:
//   V: the number of vertices in the flow network
//   FLOW_INF: a value for the flow infinity
//   FLOW_EPS: a value for the flow epsilon
//   G: an adjacency list of all edges and reverse edges in the flow network
// Functions:
//   addEdge(v, w, vwDem, wvCap): adds an edge from v to w with capacity
//     vwCap and a demand of vwDem
//   getFeasibleFlow(s, t, flowType): computes a feasible flow (or circulation
//     if s and t are -1), maximizes the flow value if flowType is positive,
//     minimizes it if flowType is negative, and finds any flow if 0; returns
//     a pair of a boolean and FlowUnit indicating whether a flow is feasible
//     and the flow value
//   getMinFlow(s, t): returns a pair of a boolean and FlowUnit indicating
//     whether a flow is feasible and the minimum flow value
//   getMaxFlow(s, t): returns a pair of a boolean and FlowUnit indicating
//     whether a flow is feasible and the maximum flow value
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(1)
//   getFeasibleFlow, getMinFlow, getMaxFlow: O(V^2 sqrt E),
//     much faster in practice
// Memory Complexity: O(V + E)
// Tested:
//   https://loj.ac/p/117
//   https://dmoj.ca/problem/wac4p6
//   https://codeforces.com/gym/100199/problem/B
template <class Edge>
struct PushRelabelMaxFlowDemands : public PushRelabelMaxFlow<Edge> {
  using MF = PushRelabelMaxFlow<Edge>;
  using FlowUnit = typename Edge::FlowUnit; using MF::V; using MF::G;
  FlowUnit FLOW_INF; vector<FlowUnit> outDem, inDem;
  PushRelabelMaxFlowDemands(
      int V, FlowUnit FLOW_INF = numeric_limits<FlowUnit>::max(),
      FlowUnit FLOW_EPS = FlowUnit(1e-9))
      : MF(V, FLOW_EPS), FLOW_INF(FLOW_INF),
        outDem(V, FlowUnit()), inDem(V, FlowUnit()) {}
  void addEdge(int v, int w, FlowUnit vwDem, FlowUnit vwCap, int type = 1) {
    if (v == w) return;
    G[v].emplace_back(w, int(G[w].size()), vwDem, vwCap);
    G[w].emplace_back(v, int(G[v].size()) - 1, -vwDem, -vwDem);
    if (type == 1) { outDem[v] += vwDem; inDem[w] += vwDem; }
  }
  pair<bool, FlowUnit> getFeasibleFlow(int s = -1, int t = -1,
                                       int flowType = 0) {
    int ss = V, tt = V + 1; G.emplace_back(); G.emplace_back();
    FlowUnit bnd = FLOW_INF, sm = 0;
    pair<bool, FlowUnit> ret(true, flowType < 0 ? FLOW_INF : FlowUnit());
    for (int v = 0; v < V; v++) {
      for (auto &&e : G[v]) e.cap -= e.dem;
      addEdge(ss, v, FlowUnit(), inDem[v], 2);
      addEdge(v, tt, FlowUnit(), outDem[v], 2); sm += inDem[v];
    }
    for (int h = 0; h < 2; h++) {
      for (int v = 0; v < V + 2; v++) for (auto &&e : G[v]) e.resCap = e.cap;
      if (s != -1 && t != -1) addEdge(t, s, FlowUnit(), bnd, 2);
      V += 2;
      if (sm - (bnd = MF::getFlow(ss, tt)) > MF::FLOW_EPS) ret.first = false;
      V -= 2; if (s != -1 && t != -1) {
        G[s].pop_back();
        if (ret.first) ret.second = G[t].back().cap - G[t].back().resCap;
        G[t].pop_back();
      }
      if (flowType >= 0 || !ret.first || h > 0) break;
      for (int v = 0; v < V + 2; v++) for (auto &&e : G[v]) e.resCap = e.cap;
      V += 2; bnd -= MF::getFlow(ss, tt); V -= 2;
    }
    G.pop_back(); G.pop_back();
    for (int v = 0; v < V; v++) {
      G[v].pop_back(); G[v].pop_back(); for (auto &&e : G[v]) e.cap += e.dem;
    }
    if (flowType > 0 && ret.first) ret.second += MF::getFlow(s, t);
    return ret;
  }
  pair<bool, FlowUnit> getMinFlow(int s, int t) {
    return getFeasibleFlow(s, t, -1);
  }
  pair<bool, FlowUnit> getMaxFlow(int s, int t) {
    return getFeasibleFlow(s, t, 1);
  }
};
