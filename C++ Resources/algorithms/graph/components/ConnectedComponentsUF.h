#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the connected components of a graph using Union Find
// Time Complexity: O(V + E)
// Memory Complexity: O(V * Inverse Ackerman)
template <const int MAXV> struct ConnectedComponentsUF {
    int UF[MAXV], id[MAXV]; vector<vector<int>> components;
    void init(int N) { fill(UF, UF + N, -1); }
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    bool addEdge(int v, int w) {
        if ((v = find(v)) == (w = find(w))) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v;
        return true;
    }
    bool connected(int v, int w) { return find(v) == find(w); }
    int getSize(int v) { return -UF[find(v)]; }
    void assign(int V) { // assigns ids to each component
        fill(id, id + V, -1); int cnt = 0;
        for (int v = 0; v < V; v++) if (find(v) == v) { id[v] = cnt++; components.emplace_back(1, v); }
        for (int v = 0; v < V; v++) if (id[v] == -1) components[id[v] = id[find(v)]].push_back(v);
    }
    void clear() { components.clear(); }
};
