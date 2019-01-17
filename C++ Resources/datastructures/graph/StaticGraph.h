#pragma once
#include <bits/stdc++.h>
using namespace std;

// Static Graph with fixed size arrays
// Edges incident to vertex v can be traversed with the following code:
//   for (int e = last[v]; e != -1; e = nxt[e])
// Time Complexity:
//   init: O(V)
//   addEdge: O(1)
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit = int> struct StaticGraph {
    int outDeg[MAXV], inDeg[MAXV], last[MAXV], to[MAXE], nxt[MAXE], E; unit weight[MAXE];
    void init(int V) { fill(last, last + V, -1); fill(outDeg, outDeg + V, 0); fill(inDeg, inDeg + V, 0); E = 0; }
    void addEdge(int v, int w, unit cost = 1) {
        outDeg[v]++; inDeg[w]++; to[E] = w; nxt[E] = last[v]; last[v] = E; weight[E++] = cost;
    }
};
