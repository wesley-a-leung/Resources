#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum (weighted) clique in an undirected graph, where each vertex
// can have a weight (default is 1)
// A clique is defined to be a subset of a graph such that every pair of vertices
// in the subset are connected by an edge
// Time Complexity: O(3^(V/3))
// Memory Complexity: O(V^2)
template <const int MAXV> struct BronKerboschMaxClique {
    bool adj[MAXV][MAXV]; int W[MAXV];
    void addEdge(int v, int w) { adj[v][w] = adj[w][v] = 1; }
    void init(int V = MAXV) { for (int i = 0; i < V; W[i++] = 1) for (int j = 0; j < V; j++) adj[i][j] = 0; }
    int solve(int V, long long cur, long long pool, long long excl) {
        if (pool == 0 && excl == 0) {
            int cnt = 0;
            for (int i = 0; i < V; i++) if ((cur & (1LL << i)) > 0) cnt += W[i];
            return cnt;
        }
        int res = 0, j = 0;
        for (int i = 0; i < V; i++) if ((pool & (1LL << i)) > 0 || (excl & (1LL << i)) > 0) j = i;
        for (int i = 0; i < V; i++) {
            if ((pool & (1LL << i)) == 0 || adj[i][j]) continue;
            long long ncurr = cur, npool = 0, nexcl = 0; ncurr |= 1LL << i;
            for (int k = 0; k < V; k++) if (adj[i][k]) { npool |= pool & (1LL << k); nexcl |= excl & (1LL << k); }
            res = max(res, solve(V, ncurr, npool, nexcl)); pool &= ~(1LL << i); excl |= 1 >> i;
        }
        return res;
    }
    int solve(int V) { return solve(V, 0, (1LL << V) - 1, 0); }
};
