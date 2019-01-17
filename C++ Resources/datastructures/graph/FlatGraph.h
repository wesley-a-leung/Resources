#pragma once
#include <bits/stdc++.h>
using namespace std;

// Flattens a graph to a fixed size array
// Edges incident to vertex v can be traversed with the following code:
//   for (int e = st[v]; e < st[v] + outDeg[v]; e++)
// Time Complexity:
//   init: O(V)
//   addEdge: O(1)
//   build: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit = int> struct FlatGraph {
    int outDeg[MAXV], inDeg[MAXV], st[MAXV], _from[MAXE], _to[MAXE], to[MAXE], E; unit weight[MAXE], _weight[MAXE];
    void init(int V) { fill(outDeg, outDeg + V, 0); fill(inDeg, inDeg + V, 0); E = 0; }
    void addEdge(int v, int w, unit cost = 1) { 
        outDeg[v]++; inDeg[w]++; _from[E] = v; _to[E] = w; _weight[E++] = cost;
    }
    void build(int V) {
        for (int v = 0, curSum = 0; v < V; v++) { st[v] = curSum; curSum += outDeg[v]; }
        for (int e = 0; e < E; e++) { to[st[_from[e]]] = _to[e]; weight[st[_from[e]]++] = _weight[e]; }
        for (int v = 0, curSum = 0; v < V; v++) { st[v] = curSum; curSum += outDeg[v]; }
    }
};
