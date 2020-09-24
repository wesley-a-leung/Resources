#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Supports queries on 2-edge connected components after edges have been added
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   ans: a vector of integers with the answer for
//     each query (1 is true, 0 is false for boolean queries)
//   id: a vector of the index of the 2-edge connected component each vertex
//     is part of
//   components: a vector of vectors containing the vertices in each 2-edge
//     connected component
//   bridges: a vector of pairs that stores the bridges in the graph
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w
//   addConnectedQuery(v, w): adds a query asking whether v and w are in the
//     same 2-edge connected component
//   addSizeQuery(v): adds a query asking for the number of vertices in the
//     same 2-edge connected component as vertex v
//   addCntQuery(): adds a query asking for the number of
//     2-edge connected components
//   addBridgeQuery(): adds a query for the number of bridges in the graph
//   solveQueries(): solves all queries asked so far
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V)
//   addEdge, addConnectedQuery, addSizeQuery: O(1)
//   addCntQuery, addBridgeQuery: O(1)
//   solveQueries: O(Q alpha V) after Q edge additions and queries
// Memory Complexity: O(V + Q) for Q edge additions and queries
// Tested:
//   https://codeforces.com/gym/100551/problem/B
//   https://judge.yosupo.jp/problem/two_edge_connected_components
struct SemiDynamicBridges {
  int V; vector<tuple<int, int, int>> queries; vector<int> ans, id;
  vector<vector<int>> components; vector<pair<int, int>> bridges;
  SemiDynamicBridges(int V) : V(V), id(V) {}
  void addEdge(int v, int w) { queries.emplace_back(0, v, w); }
  void addConnectedQuery(int v, int w) { queries.emplace_back(2, v, w); }
  void addSizeQuery(int v) { queries.emplace_back(3, v, v); }
  void addCntQuery() { queries.emplace_back(4, -1, -1); }
  void addBridgeQuery() { queries.emplace_back(5, -1, -1); }
  void solveQueries() {
    UnionFind uf(V); int E = 0; for (auto &&q : queries)
      if (get<0>(q) == 0) { E++; get<0>(q) = uf.join(get<1>(q), get<2>(q)); }
    vector<int> par(V, -1), dep(V), st(V + 1, 0), to(E * 2), &mn = id;
    function<void(int, int, int)> dfs = [&] (int v, int prev, int d) {
      par[v] = prev; dep[v] = d; for (int e = st[v]; e < st[v + 1]; e++) {
        int w = to[e]; if (w != prev) dfs(w, v, d + 1);
      }
    };
    uf = UnionFind(V); ans.clear(); ans.reserve(queries.size() - E);
    iota(mn.begin(), mn.end(), 0); for (auto &&q : queries)
      if (get<0>(q) == 1) { st[get<1>(q)]++; st[get<2>(q)]++; }
    partial_sum(st.begin(), st.end(), st.begin());
    for (auto &&q : queries) if (get<0>(q) == 1) {
      to[--st[get<1>(q)]] = get<2>(q); to[--st[get<2>(q)]] = get<1>(q);
    }
    for (int v = 0; v < V; v++) if (par[v] == -1) dfs(v, -1, 0);
    int bridgeCnt = 0; for (auto &&q : queries) {
      int t, v, w; tie(t, v, w) = q; if (t == 0) {
        for (v = mn[uf.find(v)], w = mn[uf.find(w)]; v != w;) {
          if (dep[v] < dep[w]) swap(v, w);
          int p = mn[uf.find(par[v])]; uf.join(v, p); bridgeCnt--;
          v = mn[uf.find(v)] = dep[v] < dep[p] ? v : p;
        }
      } else if (t == 1) bridgeCnt++;
      else if (t == 2) ans.push_back(uf.connected(v, w));
      else if (t == 3) ans.push_back(uf.getSize(v));
      else if (t == 4) ans.push_back(uf.cnt);
      else ans.push_back(bridgeCnt);
    }
    components.clear(); fill(id.begin(), id.end(), -1);
    for (int v = 0; v < V; v++) if (uf.find(v) == v) {
      id[v] = components.size(); components.emplace_back(1, v);
      components.back().reserve(uf.getSize(v));
    }
    for (int v = 0; v < V; v++) if (id[v] == -1)
      components[id[v] = id[uf.find(v)]].push_back(v);
    bridges.clear(); bridges.reserve(bridgeCnt); for (auto &&q : queries) {
      int t, v, w; tie(t, v, w) = q;
      if (t == 1 && !uf.connected(v, w)) bridges.emplace_back(v, w);
    }
  }
};
