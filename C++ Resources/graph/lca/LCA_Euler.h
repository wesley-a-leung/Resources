#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a tree by computing the euler tour of the tree
// Time Complexity:
//    run: O(V)
//    lca: O(log V)
template <const int MAXV> struct LCA_Euler {
    int to[MAXV], ind[MAXV], vert[MAXV], head[MAXV], ord[MAXV * 4], size, cnt, curInd; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v, int prev) {
        int cur = ind[v] = curInd++; vert[cur] = v;
        for (int w : adj[v]) if (w != prev) {
            ord[size + cnt++] = cur;
            if (head[cur] == -1) head[cur] = cnt - 1;
            dfs(w, v);
        }
        ord[size + cnt++] = cur;
        if (head[cur] == -1) head[cur] = cnt - 1;
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) {
        size = 2 * V - 1; curInd = cnt = 0; fill(head, head + V, -1); dfs(root, -1);
        for (int i = 2 * size - 2; i > 1; i -= 2) ord[i / 2] = min(ord[i], ord[i ^ 1]);
    }
    int lca(int v, int w) {
        int lo = head[ind[v]], hi = head[ind[w]], ret = INT_MAX;
        if (lo > hi) swap(lo, hi);
        for (lo += size, hi += size; lo <= hi; lo = (lo + 1) / 2, hi = (hi - 1) / 2) ret = min(ret, min(ord[lo], ord[hi]));
        return vert[ret];
    }
};
