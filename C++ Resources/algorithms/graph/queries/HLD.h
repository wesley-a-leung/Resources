#pragma once
#include <bits/stdc++.h>
using namespace std;

// Decomposes a tree into chains, such that a path from any vertex to the root will cover at most log V chains
// Time Complexity:
//   run: O(V)
//   lca: O(log V)
//   updatePath, queryPath: O(log V) * (complexity of update/query)
//   updateVertex: O(1) * (complexity of update)
// Memory Complexity: O(V)
template <const int MAXV, const bool ONE_INDEXED, const bool VALUES_ON_EDGES> struct HLD {
    using Data = int; using Lazy = int; const Data vdef = 0;
    int dep[MAXV], par[MAXV], chain[MAXV], size[MAXV], head[MAXV], ind[MAXV], vert[MAXV], chainNum, curInd; vector<int> adj[MAXV];
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
    Data merge(const Data &a, const Data &b); // to be implemented
    void update(int i, const Lazy &val); // to be implemented
    void update(int l, int r, bool up, const Lazy &val); // to be implemented
    Data query(int l, int r, bool up); // to be implemented
    Data queryUp(int v, int w, bool up, bool includeW) {
        Data ans = vdef;
        while (chain[v] != chain[w]) {
            ans = up ? merge(ans, query(ind[head[chain[v]]], ind[v], up)) : merge(query(ind[head[chain[v]]], ind[v], up), ans);
            v = par[head[chain[v]]];
        }
        if (!includeW && v == w) return ans;
        return up ? merge(ans, query(ind[w] + !includeW, ind[v], up)) : merge(query(ind[w] + !includeW, ind[v], up), ans);
    }
    void updateUp(int v, int w, bool up, bool includeW, const Lazy &val) {
        while (chain[v] != chain[w]) {
            update(ind[head[chain[v]]], ind[v], up, val);
            v = par[head[chain[v]]];
        }
        if (!includeW && v == w) return;
        update(ind[w] + !includeW, ind[v], up, val);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) { chainNum = 0; curInd = ONE_INDEXED; fill(head, head + V, -1); dfs(root, -1, 0); hld(root, -1); }
    void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
    int lca(int v, int w) {
        while (chain[v] != chain[w]) {
            if (dep[head[chain[v]]] < dep[head[chain[w]]]) w = par[head[chain[w]]];
            else v = par[head[chain[v]]];
        }
        return dep[v] < dep[w] ? v : w;
    }
    Data queryPath(int v, int w) {
        int lcavertex = lca(v, w); return merge(queryUp(v, lcavertex, true, false), queryUp(w, lcavertex, false, !VALUES_ON_EDGES));
    }
    void updatePath(int v, int w, const Lazy &val) {
        int lcavertex = lca(v, w); updateUp(v, lcavertex, true, false, val); updateUp(w, lcavertex, false, !VALUES_ON_EDGES, val);
    }
    void updateVertex(int v, const Lazy &val) { update(ind[v], val); }
};
