#pragma once
#include <bits/stdc++.h>
using namespace std;

// Assigns colors to the edges of an undirected simple graph such that
//   no edges that share an endpoint have the same color
// If D is the maximum degree of any vertex, then at most D + 1 colors
//   will be used
// Vertices and colors are 0-indexed
// Constructor Arguments:
//   V: the number of vertices in the simple graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the simple graph (no self loops or parallel edges)
//     between vertices v and w
// Fields:
//   color: a vector of integers in the range [0, D] that has a length equal
//     to the length of edges representing the coloring of the edges
// In practice, has a very small constant
// Time Complexity: O(VE)
// Memory Complexity: O(VD) where D is the maximum degree of any vertex
// Tested:
//   https://open.kattis.com/problems/gamescheduling
struct EdgeColoring {
  vector<int> color;
  EdgeColoring(int V, const vector<pair<int, int>> &edges)
      : color(edges.size(), 0) {
    vector<int> deg(V, 0), fan(V, 0), avail(V, 0);
    for (auto &&e : edges) { deg[e.first]++; deg[e.second]++; }
    int D = *max_element(deg.begin(), deg.end());
    vector<vector<int>> adj(V, vector<int>(D + 1, -1));
    vector<int> loc(D + 1, 0); for (auto &&e : edges) {
      int v, w; tie(v, w) = e; fan[0] = w; fill(loc.begin(), loc.end(), 0);
      int at = v, en = v, d = avail[w], c = avail[v], ind = 0, i = 0;
      for (; !loc[d] && (w = adj[v][d]) != -1; d = avail[w]) {
        deg[loc[d] = ++ind] = d; fan[ind] = w;
      }
      for (int cd = d; at != -1; cd ^= c ^ d, at = adj[at][cd])
        swap(adj[at][cd], adj[en = at][cd ^ c ^ d]);
      deg[loc[d]] = c; while (adj[fan[i]][d] != -1) {
        int l = fan[i], r = fan[++i], e = deg[i];
        adj[v][e] = l; adj[l][e] = v; adj[r][e] = -1; avail[r] = e;
      }
      adj[v][d] = fan[i]; adj[fan[i]][d] = v; for (int y : {fan[0], v, en})
        for (int &z = avail[y] = 0; adj[y][z] != -1; z++);
    }
    for (int i = 0; i < int(edges.size()); i++)
      while (adj[edges[i].first][color[i]] != edges[i].second) color[i]++;
  }
};
