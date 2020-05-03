#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching (and minimum vertex cover) on an unweighted bipartite graph
// Time Complexity: O((V + E) sqrt V)
// Memory Complexity: O(V + E)
template <const int MAXV> struct HopcroftKarpMaxMatch {
    int mate[MAXV], lvl[MAXV], q[MAXV]; vector<int> adj[MAXV], type[2]; bool color[MAXV], inCover[MAXV], vis[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    bool bfs() {
        int front = 0, back = 0;
        for (int v : type[0]) {
            if (mate[v] == -1) lvl[q[back++] = v] = 0;
            else lvl[v] = -1;
        }
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v]) {
                if (mate[w] == -1) return true;
                else if (lvl[mate[w]] == -1) lvl[q[back++] = mate[w]] = lvl[v] + 1;
            }
        }
        return false;
    }
    bool dfs(int v) {
        for (int w : adj[v]) if (mate[w] == -1 || (lvl[mate[w]] == lvl[v] + 1 && dfs(mate[w]))) { mate[mate[v] = w] = v; return true; }
        lvl[v] = -1; return false;
    }
    void init(int V) { for (int i = 0; i < V; i++) { adj[i].clear(); color[i] = false; } }
    int getMaxMatch(int V) {
        int cardinality = 0;
        for (int i = 0; i < 2; i++) type[i].clear();
        for (int v = 0; v < V; v++) { type[color[v]].push_back(v); mate[v] = -1; }
        while (bfs()) for (int v : type[0]) if (mate[v] == -1 && dfs(v)) cardinality++;
        return cardinality;
    }
    void dfsVertexCover(int v) {
        if (vis[v]) return;
        vis[v] = true;
        for (int w : adj[v]) if ((mate[v] == w) == color[v]) dfsVertexCover(w);
    }
    void getVertexCover(int V) {
        fill(vis, vis + V, false);
        for (int v : type[0]) if (mate[v] == -1) dfsVertexCover(v);
        for (int v = 0; v < V; v++) inCover[v] = vis[v] == color[v];
    }
};
