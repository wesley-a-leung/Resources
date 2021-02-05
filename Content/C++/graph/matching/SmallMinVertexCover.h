#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds small minimum vertex covers
// Vertices are 0-indexed
// Constructor Arguments:
//   V: the number of vertices in the graph
//   e: a vector of pairs in the form (v, w) representing
//     an undirected edge in the simple graph (no self loops or parallel edges)
//     between vertices v and w
//   K: the maximum size of the minimum vertex cover to find
// Fields:
//   V: the number of vertices in the graph 
//   E: the number of vertices in the graph 
//   minCover: the size of a minimum vertex cover with at most K vertices, or
//     V if no such cover exists
//   inCover: a vector of booleans representing whether each vertex is in the
//     minimum vertex cover with at most K vertices or not
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(E log E + 2^K KV)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/occ19g5
struct SmallMinVertexCover {
  vector<pair<int, int>> edges; int V, E, curE, minCover; vector<int> nxt;
  vector<bool> cur, inCover, temp; vector<pair<int, int>> history;
  vector<pair<int, int>> init(vector<pair<int, int>> e) {
    for (auto &&ei : e) if (ei.first > ei.second) swap(ei.first, ei.second);
    e.reserve(e.size() + 1); e.emplace_back(-1, -1); sort(e.begin(), e.end());
    e.erase(unique(e.begin(), e.end()), e.end()); return e;
  }
  void cover(int v) {
    inCover[v] = 1; for (int e = nxt[0], last = 0; e <= E; e = nxt[e])
      if (!cur[e] && (edges[e].first == v || edges[e].second == v)) {
        cur[e] = 1; curE--; nxt[last] = nxt[e]; history.emplace_back(e, last);
      } else last = e;
  }
  void uncover(int v, int revertSize) {
    inCover[v] = 0;
    for (; int(history.size()) > revertSize; history.pop_back()) {
      cur[nxt[history.back().second] = history.back().first] = 0; curE++;
    }
  }
  void solve(int K, int k) {
    if (k >= minCover) return;
    if (curE == 0) { minCover = k; temp = inCover; return; }
    if (curE > (K - k) * V) return;
    int v = -1, w = -1, curSize = history.size();
    for (int e = nxt[0]; e <= E; e = nxt[e])
      if (!cur[e]) { tie(v, w) = edges[e]; break; }
    cover(v); solve(K, k + 1); uncover(v, curSize);
    cover(w); solve(K, k + 1); uncover(w, curSize);
  }
  SmallMinVertexCover(int V, const vector<pair<int, int>> &e, int K)
      : edges(init(e)), V(V), E(edges.size() - 1), curE(E), minCover(V),
        nxt(E + 1), cur(E + 1, 0), inCover(V, 0), temp(V, 1) {
    iota(nxt.begin(), nxt.end(), 1); solve(K, 0); temp = inCover;
  }
};
