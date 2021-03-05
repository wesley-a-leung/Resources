#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/heaps/PersistentRandomizedHeap.h"
using namespace std;

// Computes the K shortest walks from s to t in a directed graph
// Template Arguments:
//   T: the type of the weight of the edges
// Function Arguments:
//   V: number of vertices in the directed graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     an directed edge in the graph from vertex v to w with
//     weight of weight
//   K: the number of shortest walks to compute
//   s: the source vertex
//   t: the destination vertex
//   INF: a value for infinity
// Return Value: a vector of type T of size K with the K shortest walks
// In practice, has a moderate constant
// Time Complexity: O((V + E + K) log V)
// Memory Complexity: O((V + E) log V + K)
// Tested:
//   https://judge.yosupo.jp/problem/k_shortest_walk
template <class T>
vector<T> kShortestWalks(int V, const vector<tuple<int, int, T>> &edges, int K,
                         int s, int t, T INF = numeric_limits<T>::max()) {
  if (K == 0) return vector<T>();
  using Heap = PersistentRandomizedHeap<pair<T, int>, greater<pair<T, int>>>;
  using ptr = typename Heap::ptr; vector<Heap> cand(V);
  vector<T> dist(V, INF), ret; ret.reserve(K); vector<int> ord; ord.reserve(V);
  vector<pair<int, int>> par(V, make_pair(-1, -1));
  vector<vector<tuple<int, T, int>>> G(V), H(V);
  std::priority_queue<pair<T, int>, vector<pair<T, int>>,
                      greater<pair<T, int>>> PQ;
  std::priority_queue<tuple<T, int, ptr>, vector<tuple<T, int, ptr>>,
                      greater<tuple<T, int, ptr>>> ans;
  for (int i = 0; i < int(edges.size()); i++) {
    int v, w; T weight; tie(v, w, weight) = edges[i];
    G[v].emplace_back(w, weight, i); H[w].emplace_back(v, weight, i);
  }
  PQ.emplace(dist[t] = T(), t); while (!PQ.empty()) {
    T d; int v; tie(d, v) = PQ.top(); PQ.pop(); if (d > dist[v]) continue;
    ord.push_back(v);
    for (auto &&e : H[v]) if (dist[get<0>(e)] > dist[v] + get<1>(e)) {
      PQ.emplace(dist[get<0>(e)] = dist[v] + get<1>(e), get<0>(e));
      par[get<0>(e)] = make_pair(get<2>(e), v);
    }
  }
  for (int v : ord) {
    for (auto &&e : G[v])
      if (get<2>(e) != par[v].first && dist[get<0>(e)] < INF) {
        T d = dist[get<0>(e)] - dist[v] + get<1>(e);
        cand[v].push(make_pair(d, get<0>(e)));
      }
    if (par[v].first != -1) cand[v].merge(cand[par[v].second]);
    if (v == s) {
      ret.push_back(dist[v]); if (cand[v].root)
        ans.emplace(dist[v] + cand[v].top().first, v, cand[v].root);
    }
  }
  while (int(ret.size()) < K) {
    if (ans.empty()) { ret.push_back(INF); continue; }
    T d; int v; ptr p; tie(d, v, p) = ans.top(); ans.pop(); ret.push_back(d);
    if (p->l) ans.emplace(d + p->l->val.first - p->val.first, v, p->l);
    if (p->r) ans.emplace(d + p->r->val.first - p->val.first, v, p->r);
    v = p->val.second;
    if (cand[v].root) ans.emplace(d + cand[v].top().first, v, cand[v].root);
  }
  return ret;
}
