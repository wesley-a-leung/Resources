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
    using Data = int; using Lazy = int; const Data qdef = 0;
    int dep[MAXV], par[MAXV], chain[MAXV], size[MAXV], head[MAXV], ind[MAXV], vert[MAXV], chainNum, curInd; vector<int> adj[MAXV];
    void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
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
    Data merge(const Data &l, const Data &r); // to be implemented
    void update(int i, const Lazy &val); // to be implemented
    void update(int l, int r, bool up, const Lazy &val); // to be implemented
    Data query(int l, int r, bool up); // to be implemented
    Data queryPath(int v, int w) {
        Data up = qdef, down = qdef;
        while (chain[v] != chain[w]) {
            if (dep[head[chain[v]]] < dep[head[chain[w]]]) { 
                down = merge(query(ind[head[chain[w]]], ind[w], false), down);
                w = par[head[chain[w]]];
            } else {
                up = merge(up, query(ind[head[chain[v]]], ind[v], true));
                v = par[head[chain[v]]];
            }
        }
        if (v != w) {
            if (dep[v] < dep[w]) up = merge(up, query(ind[v] + VALUES_ON_EDGES, ind[w], false));
            else down = merge(query(ind[w] + VALUES_ON_EDGES, ind[v], true), down);
        } else if (!VALUES_ON_EDGES) up = merge(up, query(ind[dep[v] < dep[w] ? v : w], ind[dep[v] < dep[w] ? v : w], true));
        return merge(up, down);
    }
    void updatePath(int v, int w, const Lazy &val) {
        while (chain[v] != chain[w]) {
            if (dep[head[chain[v]]] < dep[head[chain[w]]]) { update(ind[head[chain[w]]], ind[w], false, val); w = par[head[chain[w]]]; }
            else { update(ind[head[chain[v]]], ind[v], true, val); v = par[head[chain[v]]]; }
        }
        if (v != w) {
            if (dep[v] < dep[w]) update(ind[v] + VALUES_ON_EDGES, ind[w], false, val);
            else update(ind[w] + VALUES_ON_EDGES, ind[v], true, val);
        } else if (!VALUES_ON_EDGES) update(ind[dep[v] < dep[w] ? v : w], ind[dep[v] < dep[w] ? v : w], true, val);
    }
    void updateVertex(int v, const Lazy &val) { update(ind[v], val); }
    int lca(int v, int w) {
        while (chain[v] != chain[w]) {
            if (dep[head[chain[v]]] < dep[head[chain[w]]]) w = par[head[chain[w]]];
            else v = par[head[chain[v]]];
        }
        return dep[v] < dep[w] ? v : w;
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) { chainNum = 0; curInd = ONE_INDEXED; fill(head, head + V, -1); dfs(root, -1, 0); hld(root, -1); }
};
