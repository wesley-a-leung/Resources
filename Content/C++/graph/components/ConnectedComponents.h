#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Computes the connected components of a graph using Union Find
// Vertices are 0-indexed
// Constructor Arguments:
//   V: number of vertices in the graph
// Fields:
//   id: a vector of the index of the component each vertex is part of
//   components: a vector of vectors containing the vertices in each component
// Functions:
//   addEdge: adds a bidirectional edge between vertices v and w
//   assign: assigns each vertex to a connected component and populates the
//     id and component vectors
// In practice, has a small constant, faster than bfs and dfs
// Time Complexity:
//   constructor: O((V + E) * alpha(V))
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/ccc03s3
//   https://codeforces.com/contest/1253/problem/D
struct CC {
  int V; vector<int> id; vector<vector<int>> components; UnionFind uf;
  CC(int V) : V(V), id(V), uf(V) {}
  void addEdge(int v, int w) { uf.join(v, w); }
  void assign() {
    components.clear(); fill(id.begin(), id.end(), -1);
    for (int v = 0; v < V; v++) if (uf.find(v) == v) {
      id[v] = components.size(); components.emplace_back(1, v);
      components.back().reserve(uf.getSize(v));
    }
    for (int v = 0; v < V; v++) if (id[v] == -1)
      components[id[v] = id[uf.find(v)]].push_back(v);
  }
};
