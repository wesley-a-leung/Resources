#pragma once
#include <bits/stdc++.h>
using namespace std;

// Decomposes a tree into chains, such that a path from any vertex to the root will cover at most log V chains
// Time Complexity:
//   run: O(V)
//   lca: O(log V)
//   updatePath, queryPath: O(log V) * (complexity of update/query)
//   updateVertex, queryVertex, updateSubtree, querySubtree: O(1) * (complexity of update)
// Memory Complexity: O(V)
template <const int MAXV, const bool ONE_INDEXED, const bool VALUES_ON_EDGES> struct HLD {
    using Data = int; using Lazy = int; const Data qdef = 0;
    int dep[MAXV], par[MAXV], size[MAXV], head[MAXV], pre[MAXV], post[MAXV], vert[MAXV], curInd; vector<int> adj[MAXV];
    void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
    void dfs(int v, int prev, int d) {
        dep[v] = d; par[v] = prev; size[v] = 1; head[v] = -1;
        for (int w : adj[v]) if (w != prev) { dfs(w, v, d + 1); size[v] += size[w]; }
    }
    void hld(int v, int prev) {
        if (head[v] == -1) head[v] = v;
        vert[pre[v] = ++curInd] = v; int maxInd = -1;
        for (int w : adj[v]) if (w != prev && (maxInd == -1 || size[maxInd] < size[w])) maxInd = w;
        if (maxInd != -1) { head[maxInd] = head[v]; hld(maxInd, v); }
        for (int w : adj[v]) if (w != prev && w != maxInd) hld(w, v);
        post[v] = curInd;
    }
    Data merge(const Data &l, const Data &r); // to be implemented
    void update(int l, int r, bool up, const Lazy &val); // to be implemented
    Data query(int l, int r, bool up); // to be implemented
    Data queryPath(int v, int w) {
        Data up = qdef, down = qdef;
        while (head[v] != head[w]) {
            if (dep[head[v]] < dep[head[w]]) { down = merge(query(pre[head[w]], pre[w], false), down); w = par[head[w]]; }
            else { up = merge(up, query(pre[head[v]], pre[v], true)); v = par[head[v]]; }
        }
        if (v != w) {
            if (dep[v] < dep[w]) up = merge(up, query(pre[v] + VALUES_ON_EDGES, pre[w], false));
            else down = merge(query(pre[w] + VALUES_ON_EDGES, pre[v], true), down);
        } else if (!VALUES_ON_EDGES) { int i = pre[dep[v] < dep[w] ? v : w]; up = merge(up, query(i, i, true)); }
        return merge(up, down);
    }
    Data queryVertex(int v) { return query(pre[v], pre[v], true); }
    Data querySubtree(int v) { return query(pre[v], post[v], true); }
    void updatePath(int v, int w, const Lazy &val) {
        while (head[v] != head[w]) {
            if (dep[head[v]] < dep[head[w]]) { update(pre[head[w]], pre[w], false, val); w = par[head[w]]; }
            else { update(pre[head[v]], pre[v], true, val); v = par[head[v]]; }
        }
        if (v != w) {
            if (dep[v] < dep[w]) update(pre[v] + VALUES_ON_EDGES, pre[w], false, val);
            else update(pre[w] + VALUES_ON_EDGES, pre[v], true, val);
        } else if (!VALUES_ON_EDGES) { int i = pre[dep[v] < dep[w] ? v : w]; update(i, i, true, val); }
    }
    void updateVertex(int v, const Lazy &val) { update(pre[v], pre[v], true, val); }
    void updateSubtree(int v, const Lazy &val) { update(pre[v] + VALUES_ON_EDGES, post[v], true, val); }
    int lca(int v, int w) {
        while (head[v] != head[w]) {
            if (dep[head[v]] < dep[head[w]]) w = par[head[w]];
            else v = par[head[v]];
        }
        return dep[v] < dep[w] ? v : w;
    }
    int kthUp(int v, int w, int k) {
        while (head[v] != head[w]) {
            if (pre[v] - pre[head[v]] >= k) return vert[pre[v] - k];
            k -= pre[v] - pre[head[v]] + 1; v = par[head[v]];
        }
        return vert[pre[v] - k];
    }
    int kth(int v, int w, int k) {
        int LCA = lca(v, w);
        if (dep[v] - dep[LCA] >= k) return kthUp(v, LCA, k);
        else return kthUp(w, LCA, dep[v] + dep[w] - 2 * dep[LCA] - k);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, int root = 0) { curInd = int(ONE_INDEXED) - 1; dfs(root, -1, 0); hld(root, -1); }
};
