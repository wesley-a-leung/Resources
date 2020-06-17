#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching in a general undirected graph, such that each vertex
// is incident with at most one edge in the matching
// Time Complexity: O(V^2 * E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct EdmondsMatching {
    int cardinality, mate[MAXV], par[MAXV], id[MAXV], vis2[MAXV], q[MAXV], stamp; bool vis[MAXV], blossom[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void markPath(int i, int b, int j) {
        for (; id[i] != b; i = par[mate[i]]) { blossom[id[i]] = blossom[id[mate[i]]] = true; par[i] = j; j = mate[i]; }
    }
    int lca(int i, int j) {
        stamp++;
        while (true) {
            vis2[i = id[i]] = stamp;
            if (mate[i] == -1) break;
            i = par[mate[i]];
        }
        while (true) {
            if (vis2[j = id[j]] == stamp) return j;
            j = par[mate[j]];
        }
    }
    int getAugmentingPath(int V, int s) {
        fill(par, par + V, -1); fill(vis, vis + V, false); iota(id, id + V, 0); vis[s] = true; int front = 0, back = 0; q[back++] = s;
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v]) {
                if (id[v] == id[w] || mate[v] == w) continue;
                if (w == s || (mate[w] != -1 && par[mate[w]] != -1)) {
                    int newBase = lca(v, w); fill(blossom, blossom + V, false); markPath(v, newBase, w); markPath(w, newBase, v);
                    for (int i = 0; i < V; i++) if (blossom[id[i]]) {
                        id[i] = newBase;
                        if (!vis[i]) { vis[i] = true; q[back++] = i; }
                    }
                } else if (par[w] == -1) {
                    par[w] = v;
                    if (mate[w] == -1) return w;
                    else if (!vis[mate[w]]) { vis[mate[w]] = true; q[back++] = mate[w]; }
                }
            }
        }
        return -1;
    }
    int getMaxMatch(int V) {
        fill(mate, mate + V, -1); fill(vis2, vis2 + V, 0); stamp = 0; cardinality = 0;
        for (int i = 0, v; i < V; i++) if (mate[i] == -1) for (v = getAugmentingPath(V, i); v != -1;) {
            int pv = par[v], ppv = mate[pv]; mate[v] = pv; mate[pv] = v; v = ppv;
        }
        for (int i = 0; i < V; i++) if (mate[i] != -1) cardinality++;
        return cardinality / 2;
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
};
