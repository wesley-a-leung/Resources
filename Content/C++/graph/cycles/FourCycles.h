#pragma once
#include <bits/stdc++.h>
using namespace std;

// Counts the number of 4-cycles in a simple graph
// Vertices are 0-indexed
// Function Arguments:
//   V: the number of vertices in the simple graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the simple graph (no self loops or parallel edges)
//     between vertices v and w
// In practice, has a very small constant
// Time Complexity: O(V + E sqrt E)
// Memory Complexity: O(V + E)
// Tested:
//   Fuzz and Stress Tested
long long fourCycles(int V, const vector<pair<int, int>> &edges) {
  vector<int> st1(V + 1, 0), st2(V + 1, 0), d(V, 0);
  vector<int> to1(edges.size()), to2(edges.size());
  for (auto &&e : edges) { d[e.first]++; d[e.second]++; }
  auto cmp = [&] (int v, int w) { return d[v] == d[w] ? v > w : d[v] > d[w]; };
  for (auto &&e : edges) {
    int v, w; tie(v, w) = e; if (cmp(v, w)) swap(v, w);
    st1[v]++; st2[w]++;
  }
  partial_sum(st1.begin(), st1.end(), st1.begin());
  partial_sum(st2.begin(), st2.end(), st2.begin()); for (auto &&e : edges) {
    int v, w; tie(v, w) = e; if (cmp(v, w)) swap(v, w);
    to1[--st1[v]] = w; to2[--st2[w]] = v;
  }
#define loop(h) for (int e1 = st##h[v]; e1 < st##h[v + 1]; e1++) \
    for (int w = to##h[e1], e2 = st1[w]; e2 < st1[w + 1]; e2++)
  long long ret = 0; vector<long long> cnt(V, 0); for (int v = 0; v < V; v++) {
    loop(1) if (cmp(to1[e2], v)) ret += cnt[to1[e2]]++;
    loop(2) if (cmp(to1[e2], v)) ret += cnt[to1[e2]]++;
    loop(1) cnt[to1[e2]] = 0;
    loop(2) cnt[to1[e2]] = 0;
  }
#undef loop
  return ret;
}
