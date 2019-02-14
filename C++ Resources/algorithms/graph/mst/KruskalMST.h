#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum spanning tree using Kruskal's algorithm.
// Time Complexity: O(E log E)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct KruskalMST {
    struct Edge {
        int v, w; unit weight;
        bool operator < (const Edge &e) const { return weight < e.weight; }
    };
    int UF[MAXV]; vector<Edge> edges, mst; unit weight;
    void addEdge(int v, int w, unit weight) { edges.push_back({v, w, weight}); }
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    bool join(int v, int w) {
        if ((v = find(v)) == (w = find(w))) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v; return true;
    }
    unit run(int V) {
        weight = 0; fill(UF, UF + V, -1); sort(edges.begin(), edges.end());
        for (auto &&e : edges) {
            if (int(mst.size()) >= V - 1) break;
            if (join(e.v, e.w)) { mst.push_back(e); weight += e.weight; }
        }
        return weight;
    }
    void clear() { edges.clear(); mst.clear(); }
};
