#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching in a general undirected graph, such that each
//   vertex is incident with at most one edge in the matching
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic undirected graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   V: the number of vertices in the graph
//   cardinality: the cardinality of the maximum matching
//   mate: the other vertex in the matching, or -1 if unmatched
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(VE log V)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/general_matching
//   https://codeforces.com/contest/1089/problem/B
//   https://uoj.ac/problem/79
struct GeneralUnweightedMaxMatching {
  int V, cardinality, front, back; vector<int> mate, first, q;
  vector<pair<int, int>> label;
  void rematch(int v, int w) {
    int t = mate[v]; mate[v] = w; if (mate[t] != v) return;
    if (label[v].second == -1) rematch(mate[t] = label[v].first, t);
    else { int x, y; tie(x, y) = label[v]; rematch(x, y); rematch(y, x); }
  }
  int find(int v) {
    return label[first[v]].first < 0 ? first[v] : first[v] = find(first[v]);
  }
  void relabel(int x, int y) {
    int r = find(x), s = find(y), join = 0; if (r == s) return;
    pair<int, int> h = label[r] = label[s] = make_pair(~x, y); while (true) {
      if (s != V) swap(r, s);
      r = find(label[mate[r]].first); if (label[r] == h) { join = r; break; }
      else label[r] = h;
    }
    for (int v : {first[x], first[y]})
      for (; v != join; v = first[label[mate[v]].first]) {
        label[v] = make_pair(x, y); first[q[back++] = v] = join;
      }
  }
  template <class Graph> bool augment(const Graph &G, int s) {
    front = back = 0; label[q[back++] = s] = make_pair(first[s] = V, -1);
    while (front < back) {
      int v = q[front++]; for (int w : G[v]) {
        if (mate[w] == V && w != s) { rematch(mate[w] = v, w); return true; }
        else if (label[w].first >= 0) relabel(v, w);
        else if (label[mate[w]].first == -1)
          label[mate[first[q[back++] = mate[w]] = w]].first = v;
      }
    }
    return false;
  }
  template <class Graph> GeneralUnweightedMaxMatching(const Graph &G)
      : V(G.size()), cardinality(0), front(0), back(0), mate(V + 1, V),
        first(V + 1, V), q(V), label(V + 1, make_pair(-1, -1)) {
    for (int v = 0; v < V; v++) if (mate[v] == V && augment(G, v)) {
      cardinality++; for (int i = 0; i < back; i++)
        label[q[i]] = label[mate[q[i]]] = make_pair(-1, -1);
      label[V] = make_pair(-1, -1);
    }
    mate.pop_back(); for (auto &&m : mate) if (m == V) m = -1;
  }
};
