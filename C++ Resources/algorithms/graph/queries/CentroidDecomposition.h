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
    vector<int> adj[MAXV]; bool exclude[MAXV]; int size[MAXV], par[MAXV]; pair<int, int> q[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    int getSize(int v, int prev) {
        size[v] = 1;
        for (int w : adj[v]) if (w != prev && !exclude[w]) size[v] += getSize(w, v);
        return size[v];
    }
    int getCentroid(int v, int prev, int treeSize) {
        for (int w : adj[v]) if (w != prev && !exclude[w] && size[w] > treeSize / 2) return getCentroid(w, v, treeSize);
        return v;
    }
    void init(int V = MAXV) { fill(exclude, exclude + V, false); fill(par, par + V, -1); for (int i = 0; i < V; i++) adj[i].clear(); }
    int getCentroid(int v) {
        int c = getCentroid(v, -1, getSize(v, -1)); exclude[c] = true;
        return c;
    }
    void bfs(int root = 0) {
        int front = 0, back = 0; q[back++] = make_pair(root, -1);
        while (front < back) {
            int v = q[front], c = getCentroid(v, -1, getSize(v, -1));
            par[c] = q[front++]; exclude[c] = true;
            for (int w : adj[c]) if (!exclude[w]) q[back++] = make_pair(w, c);
        }
    }
};
