#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/heaps/SkewHeapIncremental.h"
#include "../../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Computes the minimum arborescence, or directed minimum spanning tree using
//   Gabow's variant of Edmonds' algorithm
// The directed minimum spanning tree is a set of edges such that every vertex
//   is reachable from the root, the sum of the edge weights is minimized
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the directed graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     a directed edge in the graph from vertex v to w with
//     weight of weight
//   root: the root of the directed mst to find
//   INF: a value for infinity
// Fields:
//   mstWeight: the weight of the directed mst, or INF if none exist
//   mstEdges: a vector of tuples of the edges in the directed mst
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O((V + E) log V)
// Memory Complexity: O(V + E)
// Tested:
//   https://open.kattis.com/contests/nwerc18open/problems/fastestspeedrun
//   https://codeforces.com/contest/240/problem/E
//   https://judge.yosupo.jp/problem/directedmst
template <class T> struct GabowMinArborescence {
  using Edge = tuple<int, int, T>;
  struct Pair {
    Edge e; int ind;
    Pair(const Edge &e, int ind) : e(e), ind(ind) {}
    bool operator > (const Pair &p) const { return get<2>(e) > get<2>(p.e); }
    Pair &operator += (const T &add) { get<2>(e) += add; return *this; }
  };
  using Heap = SkewHeapIncremental<Pair, greater<Pair>, T>;
  T INF, mstWeight; vector<Edge> mstEdges;
  GabowMinArborescence(int V, const vector<Edge> &edges, int root,
                       T INF = numeric_limits<T>::max())
      : INF(INF), mstWeight() {
    UnionFindUndo uf(V); vector<int> vis(V, -1); vis[root] = root;
    vector<Edge> in(V, Edge(-1, -1, T())); vector<Heap> H(V);
    for (int i = 0; i < int(edges.size()); i++)
      H[get<1>(edges[i])].push(Pair(edges[i], i));
    vector<tuple<int, int, vector<Edge>>> cycs; for (int s = 0; s < V; s++) {
      int v = s; vector<pair<int, Edge>> path; while (vis[v] < 0) {
        if (H[v].empty()) { mstWeight = INF; return; }
        vis[v] = s; Pair p = H[v].pop(); path.emplace_back(v, edges[p.ind]);
        H[v].increment(-get<2>(p.e)); v = uf.find(get<0>(p.e));
        mstWeight += get<2>(p.e); if (vis[v] == s) {
          Heap h; vector<Edge> E; int w, t = uf.history.size(); do {
            h.merge(H[w = path.back().first]);
            E.push_back(path.back().second); path.pop_back();
          } while (uf.join(v, w));
          H[v = uf.find(v)] = move(h); vis[v] = -1; cycs.emplace_back(v, t, E);
        }
      }
      for (auto &&p : path) in[uf.find(get<1>(p.second))] = p.second;
    }
    while (!cycs.empty()) {
      int v, t; vector<Edge> E; tie(v, t, E) = cycs.back(); cycs.pop_back();
      while (int(uf.history.size()) > t) uf.undo();
      Edge inEdge = in[v]; for (auto &&e : E) in[uf.find(get<1>(e))] = e;
      in[uf.find(get<1>(inEdge))] = inEdge;
    }
    for (int v = 0; v < V; v++)
      if (get<1>(in[v]) != -1) mstEdges.push_back(in[v]);
  }
};
