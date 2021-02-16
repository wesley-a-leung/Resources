#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds an Eulerian walk (or a circuit) in a graph which can be
//   either undirected or directed
// A undirected walk exists iff every exactly zero of two vertices has an odd
//   degree and all nonzero degree vertices are in a single connected component
// A undirected circuit exists iff every vertex has an even degree
//   and all nonzero degree vertices are in a single connected component
// A directed walk exists iff at most one vertex has outDeg - inDeg = 1 and at
//   most one vertex has inDeg - outDeg = 1, with all other vertices having
//   inDeg = outDeg and all vertices all nonzero degree vertices are in a
//   single connected component of the underlying undirected graph
// A directed circuit exists iff all vertices have inDeg = outDeg and all
//   nonzero degree vertices are in a single strongly connected component
// Vertices are 0-indexed
// Function Arguments:
//   V: number of vertices in the graph
//   edges: a vector of pairs in the form (v, w) representing
//     an edge in the simple graph between vertices v and w, each edge should
//     be specified exactly once regardless of whether the graph is
//     directed or undirected
//   directed: a boolean indicating whether the graph is directed or not
//   circuit: a boolean indicating whether the Eulerian walk is required to be
//     a circuit (first vertex equal to last)
//   s: the starting vertex of the Eulerian walk, or -1 if any vertex can be
//     the starting vertex
// Return Value: the vertices in the Eulerian walk with the first vertex
//   either equal to s if s is non negative, or any arbitrary vertex if
//   s is -1, and the last vertex equal to the first vertex if circuit is true,
//   and any vertex otherwise
// In practice, has a moderate constant
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/eulerianpath
//   https://cses.fi/problemset/task/1691/
//   https://cses.fi/problemset/task/1693/
vector<int> eulerianWalk(int V, const vector<pair<int, int>> &edges,
                         bool directed, bool circuit, int s = -1) {
  if (V == 0) return vector<int>();
  else if (edges.empty()) return vector<int>{0};
  vector<int> st(V + 1, 0), d(V, 0), walk; walk.reserve(edges.size() + 1);
  vector<int> to(edges.size() * (directed ? 1 : 2)), eInd(to.size());
  for (auto &&e : edges) {
    st[e.first]++; d[e.second]++;
    if (!directed) { st[e.second]++; d[e.first]++; }
  }
  if (s == -1) {
    s = edges[0].first; if (circuit) {}
    else if (directed) { for (int v = 0; v < V; v++) if (st[v] > d[v]) s = v; }
    else { for (int v = 0; v < V; v++) if (d[v] % 2 == 1) s = v; }
  }
  partial_sum(st.begin(), st.end(), st.begin()); fill(d.begin(), d.end(), 0);
  for (int i = 0, v, w; i < int(edges.size()); i++) {
    tie(v, w) = edges[i]; to[--st[v]] = w; eInd[st[v]] = i;
    if (!directed) { to[--st[w]] = v; eInd[st[w]] = i; }
  }
  vector<int> cur = st, stk(edges.size() + 1);
  vector<bool> vis(edges.size(), false);
  int top = 0; d[stk[top++] = s]++; while (top > 0) {
    int v = stk[top - 1];
    if (cur[v] == st[v + 1]) { walk.push_back(v); --top; continue; }
    int w = to[cur[v]], e = eInd[cur[v]++];
    if (!vis[e]) { d[v]--; d[stk[top++] = w]++; vis[e] = true; }
  }
  if ((circuit && walk[0] != walk.back())
      || walk.size() != edges.size() + 1) return vector<int>();
  for (int di : d) if (di < 0) return vector<int>();
  reverse(walk.begin(), walk.end()); return walk;
}
