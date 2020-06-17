#pragma once
#include <bits/stdc++.h>
using namespace std;

// Flattens a directed graph to a fixed size array
// Edges incident to vertex v can be traversed with the following code:
//   for (int e = st[v]; e < st[v + 1]; e++)
// Time Complexity:
//   init: O(V)
//   addEdge: O(1)
//   build: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit = int> struct FlatGraph {
    int st[MAXV], to[MAXE], A[MAXE], B[MAXE], E; unit weight[MAXE], C[MAXE];
    void init(int V) { fill(st, st + V + 1, 0); E = 0; }
    void addEdge(int v, int w, unit cost = 1) { st[v]++; A[E] = v; B[E] = w; C[E++] = cost; }
    void build(int V) {
        partial_sum(st, st + V + 1, st);
        for (int e = 0; e < E; e++) { to[--st[A[e]]] = B[e]; weight[st[A[e]]] = C[e]; }
    }
};

// Flat Graph with minimal space
// Edges incident to vertex v can be traversed with the following code:
//   for (int e = st[v]; e < st[v + 1]; e++)
// Time Complexity:
//   init: O(V)
//   addEdge: O(1)
//   build: O(V + E log E)
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit = int> struct FlatGraphMemory {
    int st[MAXV], E; pair<pair<int, int>, unit> G[MAXE];
    void init(int V) { fill(st, st + V + 1, 0); E = 0; }
    void addEdge(int v, int w, unit cost = 1) { st[v + 1]++; G[E++] = make_pair(make_pair(v, w), cost); }
    void build(int V) { partial_sum(st, st + V + 1, st); sort(G, G + E); }
};
