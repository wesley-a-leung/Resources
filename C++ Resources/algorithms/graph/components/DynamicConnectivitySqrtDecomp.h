#pragma once
#include <bits/stdc++.h>
using namespace std;

// Support queries for the number of components in a graph, after edges have been added or removed
// Sqrt Decomposition Solution
// Time Complexity: O(V * (Q / B) + Q * B)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ, const int BLOCKSZ> struct DynamicConnectivitySqrtDecomp {
    int Q = 0, vis[MAXV], root[MAXV]; vector<int> ans; unordered_set<int> adj[MAXV], toRem[MAXV]; unordered_map<int, int> adj2[MAXV];
    struct Query { int type, v, w; } q[MAXQ];
    void dfs1(int v, int r) {
        root[v] = r;
        for (int w : adj[v]) if (root[w] == -1 && !toRem[v].count(w)) dfs1(w, r);
    }
    bool dfs2(int v, int t, int i) {
        if (v == t) return true;
        vis[v] = i;
        for (auto && e : adj2[v]) if (vis[e.first] != i && dfs2(e.first, t, i)) return true;
        return false;
    }
    void clear() { ans.clear(); Q = 0; for (int i = 0; i < MAXV; i++) { adj[i].clear(); toRem[i].clear(); adj2[i].clear(); } }
    void addEdge(int v, int w) { q[Q++] = {1, v, w}; }
    void removeEdge(int v, int w) { q[Q++] = {-1, v, w}; }
    void query() { q[Q++] = {0, -1, -1}; }
    void solve(int V) {
        int cnt = V;
        fill(vis, vis + MAXV, -1);
        for (int st = 0; st < Q; st += BLOCKSZ) {
            fill(root, root + V, -1);
            for (int i = st; i < min(st + BLOCKSZ, Q); i++) {
                if (q[i].type != -1) continue;
                toRem[q[i].v].insert(q[i].w); toRem[q[i].w].insert(q[i].v);
            }
            for (int v = 0; v < V; v++) {
                if (root[v] == -1) dfs1(v, v);
                adj2[v].clear();
            }
            for (int v = 0; v < V; v++) {
                for (int w : adj[v]) if (root[v] != root[w]) adj2[root[v]][root[w]]++;
                toRem[v].clear();
            }
            for (int i = st; i < min(st + BLOCKSZ, Q); i++) {
                if (q[i].type == 1) {
                    adj[q[i].v].insert(q[i].w); adj[q[i].w].insert(q[i].v);
                    int rv = root[q[i].v], rw = root[q[i].w];
                    if (rv != rw) {
                        if (!dfs2(rv, rw, i)) cnt--;
                        adj2[rv][rw]++; adj2[rw][rv]++;
                    }
                } else if (q[i].type == -1) {
                    adj[q[i].v].erase(q[i].w); adj[q[i].w].erase(q[i].v);
                    int rv = root[q[i].v], rw = root[q[i].w];
                    if (rv != rw) {
                        if (--adj2[rv][rw] == 0) adj2[rv].erase(rw);
                        if (--adj2[rw][rv] == 0) adj2[rw].erase(rv);
                        if (!dfs2(rv, rw, i)) cnt++;
                    }
                } else ans.push_back(cnt);
            }
        }
    }
};
