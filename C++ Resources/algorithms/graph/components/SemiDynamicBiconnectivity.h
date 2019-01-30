#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the number of bridges in a graph after edges have been added
// Time Complexity: O(V + Q)
// Memory Complexity O(V + Q)
template <const int MAXV, const int MAXQ> struct SemiDynamicBiconnectivity {
    int Q = 0, UF[MAXV], par[MAXV], dep[MAXV]; vector<int> adj[MAXV], ans;
    struct Query { int type, v, w; } q[MAXQ];
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    bool join(int v, int w) {
        v = find(v); w = find(w);
        if (v == w) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v;
        return true;
    }
    void dfs(int v, int prev, int d) {
        dep[v] = d; par[v] = prev;
        for (int w : adj[v]) if (w != prev) dfs(w, v, d + 1);
    }
    void init(int V = MAXV) { fill(UF, UF + V, -1); Q = 0; }
    void clear(int V = MAXV) { ans.clear(); for (int i = 0; i < V; i++) adj[i].clear(); }
    void addEdge(int v, int w) {
        if (join(v, w)) { adj[v].push_back(w); adj[w].push_back(v); q[Q++] = {1, v, w}; }
        else q[Q++] = {2, v, w};
    }
    void query() { q[Q++] = {0, -1, -1}; }
    void solve(int V) {
        fill(par, par + MAXV, -1); fill(UF, UF + MAXV, -1); int cnt = 0;
        for (int v = 0; v < V; v++) if (par[v] == -1) dfs(v, -1, 0);
        for (int i = 0; i < Q; i++) {
            if (q[i].type == 0) ans.push_back(cnt);
            else if (q[i].type == 1) cnt++;
            else {
                int a = q[i].v, b = q[i].w;
                while ((a = find(a)) != (b = find(b))) {
                    if (dep[a] < dep[b]) swap(a, b);
                    UF[a] = find(par[a]); a = par[a]; cnt--;
                }
            }
        }
    }
};
