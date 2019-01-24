#pragma once
#include <bits/stdc++.h>
using namespace std;

// Decomposes a tree into chains, such that a path from any vertex to the root will cover at most log V chains
// Time Complexity:
//   run: O(V)
//   lca: O(log V)
//   queryPath: O(log V) * (complexity of query)
// Memory Complexity: O(V)
template <const int MAXV> struct HLD {
    int dep[MAXV], par[MAXV], chain[MAXV], size[MAXV], head[MAXV], ind[MAXV], vert[MAXV], chainNum, curInd;
    vector<int> adj[MAXV];
    void dfs(int v, int prev, int d) {
        dep[v] = d; par[v] = prev; size[v] = 1;
        for (int w : adj[v]) if (w != prev) { dfs(w, v, d + 1); size[v] += size[w]; }
    }
    void hld(int v, int prev) {
        if (head[chainNum] == -1) head[chainNum] = v;
        chain[v] = chainNum; ind[v] = curInd; vert[curInd++] = v; int maxInd = -1;
        for (int w : adj[v]) if (w != prev && (maxInd == -1 || size[maxInd] < size[w])) maxInd = w;
        if (maxInd != -1) hld(maxInd, v);
        for (int w : adj[v]) if (w != prev && w != maxInd) { chainNum++; hld(w, v); }
    }
    int merge(int a, int b); // to be implemented
    int query(int l, int r, bool up); // to be implemented
    int queryUp(int v, int w, bool up, bool includeW) {
        int ans = 0;
        while (chain[v] != chain[w]) {
            ans = up ? merge(ans, query(ind[head[chain[v]]], ind[v], up)) : merge(query(ind[head[chain[v]]], ind[v], up), ans);
            v = par[head[chain[v]]];
        }
        if (!includeW && v == w) return ans;
        return up ? merge(ans, query(ind[w] + !includeW, ind[v], up)) : merge(query(ind[w] + !includeW, ind[v], up), ans);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) { chainNum = 0; curInd = 1; fill(head, head + V, -1); dfs(root, -1, 0); hld(root, -1); }
    void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
    int lca(int v, int w) {
        while (chain[v] != chain[w]) {
            if (dep[head[chain[v]]] < dep[head[chain[w]]]) w = par[head[chain[w]]];
            else v = par[head[chain[v]]];
        }
        if (dep[v] < dep[w]) return v;
        return w;
    }
    int queryPath(int v, int w) {
        int lcavertex = lca(v, w);
        return merge(queryUp(v, lcavertex, true, false), queryUp(w, lcavertex, false, true));
    }
};
