#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the centroid of each component of a tree, and splits the component at that vertex
// Can be used to create a centroid tree, which has depth O(log V)
// Time Complexity:
//    getCentroid: O(ComponentSize)
//    bfs: O(V log V)
// Memory Complexity: O(V)
template <const int MAXV> struct CentroidDecomposition {
    vector<int> adj[MAXV]; bool exclude[MAXV]; int par[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    int getSize(int v, int prev) {
        int size = 1;
        for (int w : adj[v]) if (w != prev && !exclude[w]) size += getSize(w, v);
        return size;
    }
    int getCentroid(int v, int prev, int treeSize) {
        int n = treeSize, size = 1; bool hasCentroid = true;
        for (int w : adj[v]) if (w != prev && exclude[w]) {
            int ret = getCentroid(w, v, treeSize);
            if (ret >= 0) return ret;
            hasCentroid &= -ret <= n / 2; size -= ret;
        }
        return (hasCentroid &= n - size <= n / 2) ? v : -size;
    }
    void init(int V = MAXV) { fill(exclude, exclude + V, false); fill(par, par + V, -1); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    int getCentroid(int v) {
        int c = getCentroid(v, -1, getSize(v, -1)); exclude[c] = true;
        return c;
    }
    void bfs(int root = 0) {
        queue<pair<int, int>> q; q.emplace(root, -1);
        while (!q.empty()) {
            int v = q.front().first, c = getCentroid(v, -1, getSize(v, -1));
            par[c] = q.front().second; q.pop(); exclude[c] = true;
            for (int w : adj[c]) if (!exclude[w]) q.emplace(w, c);
        }
    }
};
