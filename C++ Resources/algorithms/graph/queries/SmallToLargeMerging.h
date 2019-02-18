#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves queries on a tree by merging the smaller subtrees into the larger subtrees
// Time Complexity: O(V log V * (complexity of moving from one container to another))
template <const int MAXV> struct SmallToLargeMerging {
    vector<int> adj[MAXV]; int SZ[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void clearContainer(int v); // to be implemented
    void merge(int large, int small); // to be implemented
    void add(int v); // to be implemented
    void swapContainers(int v, int w); // to be implemented
    void solveQueries(int v); // to be implemented
    void dfs(int v, int prev) {
        SZ[v] = 1; add(v);
        for (int w : adj[v]) if (w != prev) {
            dfs(w, v);
            if (SZ[v] < SZ[w]) swapContainers(v, w);
            merge(v, w); SZ[v] += SZ[w];
        }
        solveQueries(v);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) { adj[i].clear(); clearContainer(i); } }
};
