#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes Depth First Orders of a graph (pre order, post order,
//   topological/reverse post order)
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
// Fields:
//   preInd: a vector of the pre order index for each vertex
//   postInd: a vector of the post order index for each vertex
//   revPostInd: a vector of the topological/reverse post order index
//     for each vertex
//   preVert: a vector of the vertices sorted by pre order index
//   postVert: a vector of the vertices sorted by pre order index
//   revPostVert: a vector of the vertices sorted by topological/reverse post
//     order index
// In practice, has a moderate constant, slower than TopologicalOrder
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V)
// Tested:
//   https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_d
//   https://codeforces.com/contest/24/problem/A
struct DFSOrder {
  int V, curPre, curPost, curRevPost;
  vector<int> preInd, postInd, revPostInd, preVert, postVert, revPostVert;
  template <class Graph> void dfs(const Graph &G, int v) {
    preVert[preInd[v] = curPre++] = v;
    for (int w : G[v]) if (preInd[w] == -1) dfs(G, w);
    postVert[postInd[v] = curPost++] = v;
    revPostVert[revPostInd[v] = curRevPost--] = v;
  }
  template <class Graph> DFSOrder(const Graph &G, const vector<int> &roots)
      : V(G.size()), curPre(0), curPost(0), curRevPost(V - 1), preInd(V, -1),
        postInd(V), revPostInd(V), preVert(V), postVert(V), revPostVert(V) {
    for (int v : roots) if (preInd[v] == -1) dfs(G, v);
  }
  template <class Graph> DFSOrder(const Graph &G, int rt)
      : DFSOrder(G, vector<int>{rt}) {}
};
