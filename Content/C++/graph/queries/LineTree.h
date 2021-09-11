#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/FischerHeunStructure.h"
#include "../../datastructures/unionfind/WeightedUnionFind.h"
using namespace std;

// Converts a tree or forest into a line graph to query for the maximum edge
//   value on a path between two vertices
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges 
//   Cmp: the comparator to compare two values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   V: number of vertices in the graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     an undirected edge in the graph between vertices v and w with
//     weight of weight
//   cmp: an instance of the Cmp struct
// Functions:
//   connected(v, w): returns true if and only if v and w are connected
//   query(v, w): returns the maximum edge weight (based on the comparator) on
//     the path from vertices v and w assuming v and w are connected
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V)
//   connected, query: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/acc2p3
//   https://codeforces.com/problemsets/acmsguru/problem/99999/383
template <class T, class Cmp = less<T>> struct LineTree {
  using Edge = tuple<int, int, T>;
  vector<int> root, ind; vector<T> weights; FischerHeunStructure<int> FHS;
  vector<int> init(int V, vector<Edge> edges, Cmp cmp) {
    sort(edges.begin(), edges.end(), [&] (const Edge &a, const Edge &b) {
      return cmp(get<2>(a), get<2>(b));
    });
    vector<int> nxt(V, -1), A(V, 0), ret; int k = 0;
    auto op = [&] (const pair<int, int> &a, const pair<int, int> &b) {
      nxt[a.second] = b.first; A[a.second] = k;
      return make_pair(a.first, b.second);
    };
    ret.reserve(V); vector<pair<int, int>> W(V);
    for (int v = 0; v < V; v++) W[v] = make_pair(v, v);
    WeightedUnionFind<pair<int, int>, decltype(op)> uf(move(W), op);
    weights.reserve(edges.size()); for (auto &&e : edges) {
      assert(uf.join(get<0>(e), get<1>(e))); k++; weights.push_back(get<2>(e));
    }
    for (int v = 0; v < V; v++) if ((root[v] = uf.find(v)) == v)
      for (int w = uf.getWeight(v).first; w != -1; w = nxt[w]) {
        ind[w] = ret.size(); ret.push_back(A[w]);
      }
    return ret;
  }
  LineTree(int V, vector<Edge> edges, Cmp cmp = Cmp())
      : root(V), ind(V), weights(), FHS(init(V, move(edges), cmp)) {}
  bool connected(int v, int w) { return root[v] == root[w]; }
  T query(int v, int w) {
    assert(v != w); if (ind[v] > ind[w]) swap(v, w);
    return weights[FHS.query(ind[v], ind[w] - 1)];
  }
};
