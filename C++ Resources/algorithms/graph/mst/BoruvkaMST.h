#pragma once
#include <bits/stdc++.h>
#include "../../../datastructures/UnionFind.h"
using namespace std;

// Computes the minimum spanning tree using Boruvka's algorithm
// Time Complexity: O(E log V)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct BoruvkaMST {
    struct Edge { int v, w; unit weight; };
    UnionFind<MAXV> uf; int closest[MAXV]; vector<Edge> edges, mst; unit weight;
    void addEdge(int v, int w, unit weight) { edges.push_back({v, w, weight}); }
    unit run(int V) {
        weight = 0; uf.init(V);
        for (int t = 1; t < V && int(mst.size()) < V - 1; t *= 2) {
            fill(closest, closest + V, -1);
            for (int e = 0; e < int(edges.size()); e++) {
                int i = uf.find(edges[e].v), j = uf.find(edges[e].w);
                if (i == j) continue;
                if (closest[i] == -1 || edges[e].weight < edges[closest[i]].weight) closest[i] = e;
                if (closest[j] == -1 || edges[e].weight < edges[closest[j]].weight) closest[j] = e;
            }
            for (int i = 0; i < V; i++) if (closest[i] != -1) {
                int v = edges[closest[i]].v, w = edges[closest[i]].w;
                if (uf.join(v, w)) { mst.push_back(edges[closest[i]]); weight += edges[closest[i]].weight; }
            }
        }
        return weight;
    }
    void clear() { edges.clear(); mst.clear(); }
};
