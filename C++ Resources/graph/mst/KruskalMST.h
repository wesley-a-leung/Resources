#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Computes the minimum spanning tree using Kruskal's algorithm
// Time Complexity: O(E log E)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct KruskalMST {
    struct Edge {
        int v, w; unit weight;
        bool operator < (const Edge &e) const { return weight < e.weight; }
    };
    UnionFind<MAXV> uf; vector<Edge> edges, mst; unit weight;
    void addEdge(int v, int w, unit weight) { edges.push_back({v, w, weight}); }
    unit run(int V) {
        weight = 0; uf.init(V); sort(edges.begin(), edges.end());
        for (auto &&e : edges) {
            if (int(mst.size()) >= V - 1) break;
            if (uf.join(e.v, e.w)) { mst.push_back(e); weight += e.weight; }
        }
        return weight;
    }
    void clear() { edges.clear(); mst.clear(); }
};
