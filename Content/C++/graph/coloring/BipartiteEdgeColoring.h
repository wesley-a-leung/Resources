#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
#include "../matching/HopcroftKarpMaxMatch.h"
using namespace std;

// Assigns colors to the edges of a bipartite graph such that
//   no edges that share an endpoint have the same color
// If D is the maximum degree of any vertex, then exactly D colors
//   will be used
// Vertices and colors are 0-indexed
// Constructor Arguments:
//   V: number of vertices in the simple graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the graph; side[v] != side[w] must hold
//   side: the side of the bipartition each vertex is part of
// Fields:
//   color: a vector of integers in the range [0, D) that has a length equal
//     to the length of edges representing coloring of the edges
// In practice, has a small constant
// Time Complexity: O(V log V + E log D sqrt (E / D)) where D is the maximum
//   degree of any vertex
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/bipartite_edge_coloring
struct BipartiteEdgeColoring {
  int makeDRegular(int &V, vector<pair<int, int>> &edges, vector<bool> &side) {
    vector<int> deg(V, 0); for (auto &&e : edges) {
      if (side[e.first]) swap(e.first, e.second);
      deg[e.first]++; deg[e.second]++;
    }
    int D = *max_element(deg.begin(), deg.end()); UnionFind uf(V);
    for (int s = 0; s < 2; s++) {
      priority_queue<pair<int, int>, vector<pair<int, int>>,
                     greater<pair<int, int>>> PQ;
      for (int v = 0; v < V; v++) if (side[v] == s) PQ.emplace(deg[v], v);
      while (int(PQ.size()) >= 2) {
        pair<int, int> v = PQ.top(); PQ.pop();
        pair<int, int> w = PQ.top(); PQ.pop(); if (v.first + w.first <= D) {
          uf.join(v.second, w.second); PQ.emplace(v.first + w.first, v.second);
        }
      }
    }
    vector<int> cnt(2, 0), id(V, -1); int curId = 0;
    for (int s = 0; s < 2; s++) for (int v = 0; v < V; v++)
      if (uf.find(v) == v && side[v] == s) { id[v] = curId++; cnt[s]++; }
    deg.assign(V = max(cnt[0], cnt[1]) * 2, 0); edges.reserve(V * D / 2);
    side.reserve(V); side.assign(cnt[0] + cnt[1], true);
    fill(side.begin(), side.begin() + cnt[0], false);
    for (int s = 0; s < 2; s++) for (; cnt[s] * 2 < V; cnt[s]++)
      side.push_back(s);
    for (auto &&e : edges) {
      deg[e.first = id[uf.find(e.first)]]++;
      deg[e.second = id[uf.find(e.second)]]++;
    }
    for (int v = 0, w = 0; v < V; v++) while (!side[v] && deg[v] < D) {
      while (!side[w] || deg[w] == D) w++;
      edges.emplace_back(v, w); deg[v]++; deg[w]++;
    }
    return D;
  }
  vector<int> eulerianCircuit(
      int V, const vector<pair<int, int>> &edges, const vector<int> &inds) {
    vector<vector<pair<int, int>>> G(V); vector<int> circuit;
    for (int i = 0; i < int(inds.size()); i++) {
      int v, w; tie(v, w) = edges[inds[i]];
      G[v].emplace_back(w, i); G[w].emplace_back(v, i);
    }
    vector<bool> vis1(V, false), vis2(inds.size(), false);
    vector<pair<int, int>> stk; for (int s = 0; s < V; s++) if (!vis1[s]) {
      stk.clear(); stk.emplace_back(s, -1); while (!stk.empty()) {
        int v, w, e; tie(v, e) = stk.back(); vis1[v] = true;
        if (G[v].empty()) { circuit.emplace_back(e); stk.pop_back(); }
        else {
          tie(w, e) = G[v].back(); G[v].pop_back();
          if (!vis2[e]) { vis2[e] = true; stk.emplace_back(w, e); }
        }
      }
      circuit.pop_back();
    }
    for (auto &&e : circuit) e = inds[e];
    return circuit;
  }
  vector<int> color;
  BipartiteEdgeColoring(int V, vector<pair<int, int>> edges, vector<bool> side)
      : color(edges.size(), 0) {
    for (auto &&e : edges) assert(side[e.first] != side[e.second]);
    int D = makeDRegular(V, edges, side), curCol = 0;
    function<void(int, const vector<int> &)> rec = [&] (
        int d, const vector<int> &inds) {
      if (d == 0) return;
      else if (d == 1) {
        for (int e : inds) if (e < int(color.size())) color[e] = curCol;
        curCol++;
      } else if (d % 2 == 0) {
        vector<int> circuit = eulerianCircuit(V, edges, inds), half1, half2;
        half1.reserve(circuit.size() / 2); half2.reserve(circuit.size() / 2);
        for (int i = 0; i < int(circuit.size()); i += 2) {
          half1.push_back(circuit[i]); half2.push_back(circuit[i + 1]);
        }
        rec(d / 2, half1); rec(d / 2, half2);
      } else {
        vector<vector<int>> G(V); for (int e : inds) {
          int v, w; tie(v, w) = edges[e]; G[v].push_back(w); G[w].push_back(v);
        }
        vector<int> unmatched; HopcroftKarpMaxMatch mm(G, side);
        for (int e : inds) {
          int v, w; tie(v, w) = edges[e]; if (mm.mate[v] == w) {
            mm.mate[v] = -1; if (e < int(color.size())) color[e] = curCol;
          } else unmatched.push_back(e);
        }
        curCol++; rec(d - 1, unmatched);
      }
    };
    vector<int> inds(edges.size()); iota(inds.begin(), inds.end(), 0);
    rec(D, inds);
  }
};
