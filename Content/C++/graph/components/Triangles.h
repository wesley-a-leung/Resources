#pragma once
#include <bits/stdc++.h>
using namespace std;

// Counts the number of triangles in a simple graph
// Vertices are 0-indexed
// Template Arguments:
//   F: the type of the function f (can be deduced automatically in some cases)
// Function Arguments:
//   V: number of vertices in the simple graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the simple graph (no self loops or parallel edges)
//     between vertices v and w
//   f(a, b, c, i, j, k): the function to run a callback on for each triangle
//     where a, b, c are the vertices in the triangle, and i, j, k are the
//     indices of the edges in the triangle
// In practice, has a very small constant
// Time Complexity: O(V + E sqrt E)
// Memory Complexity: O(V + E)
// Tested:
//   Fuzz and Stress Tested
//   https://mcpt.ca/problem/lcc19c3s4
//   https://vietnam-national18.kattis.com/problems/gottacatchemall
//   https://judge.yosupo.jp/problem/enumerate_triangles
//   https://dmoj.ca/problem/year2019p7
template <class F>
void triangles(int V, const vector<pair<int, int>> &edges, F f) {
  vector<int> st(V + 1, 0), ind(V, 0), to(edges.size()), eInd(edges.size());
  vector<int> &d = ind; for (auto &&e : edges) { d[e.first]++; d[e.second]++; }
  auto cmp = [&] (int v, int w) { return d[v] == d[w] ? v > w : d[v] > d[w]; };
  for (auto &&e : edges) st[cmp(e.first, e.second) ? e.second : e.first]++;
  partial_sum(st.begin(), st.end(), st.begin());
  for (int i = 0, v, w; i < int(edges.size()); i++) {
    tie(v, w) = edges[i]; if (cmp(v, w)) swap(v, w);
    to[--st[v]] = w; eInd[st[v]] = i;
  }
  fill(ind.begin(), ind.end(), -1); for (int v = 0; v < V; v++) {
    for (int e1 = st[v]; e1 < st[v + 1]; e1++) ind[to[e1]] = eInd[e1];
    for (int e1 = st[v]; e1 < st[v + 1]; e1++)
      for (int w = to[e1], e2 = st[w]; e2 < st[w + 1]; e2++)
        if (ind[to[e2]] != -1)
          f(v, w, to[e2], eInd[e1], eInd[e2], ind[to[e2]]);
    for (int e1 = st[v]; e1 < st[v + 1]; e1++) ind[to[e1]] = -1;
  }
}
