#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for whether a graph component is bipartite after edges have been added
// Time Complexity:
//   init: O(V)
//   find, addEdge, connected, pathParity, getSize, isComponentBipartite: alpha(V)
//   isGraphBipartite: O(1)
// Memory Complexity: O(V)
template <const int MAXV, const bool ONE_INDEXED> struct SemiDynamicBipartite {
    int UF[MAXV], cnt; bool parity[MAXV]; bool bipartite[MAXV], bipartiteGraph;
    void init(int V) {
        fill(UF, UF + V + ONE_INDEXED, -1); fill(parity, parity + V + ONE_INDEXED, false);
        fill(bipartite, bipartite + V + ONE_INDEXED, true); bipartiteGraph = true; cnt = V;
    }
    pair<int, bool> find(int v) {
        bool p = parity[v];
        for (; UF[v] >= 0; p ^= parity[v = UF[v]]) if (UF[UF[v]] >= 0) {
            p ^= parity[UF[v]]; parity[v] ^= parity[UF[v]]; UF[v] = UF[UF[v]];
        }
        return make_pair(v, p);
    }
    void addEdge(int v, int w) {
        bool pv, pw; tie(v, pv) = find(v); tie(w, pw) = find(w);
        if (v == w) { bipartiteGraph &= (bipartite[v] &= (pv ^ pw)); return; }
        if (UF[v] > UF[w]) { swap(v, w); swap(pv, pw); }
        UF[v] += UF[w]; UF[w] = v; cnt--; parity[w] = pv ^ pw ^ 1; bipartite[v] &= bipartite[w];
    }
    bool connected(int v, int w) { return find(v).first == find(w).first; }
    bool pathParity(int v, int w) { assert(connected(v, w)); return find(v).second ^ find(w).second; }
    int getSize(int v) { return -UF[find(v).first]; }
    bool isComponentBipartite(int v) { return bipartite[find(v).first]; }
    bool isGraphBipartite() { return bipartiteGraph; }
};
