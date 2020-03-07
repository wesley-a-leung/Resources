#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching (and minimum vertex cover) on an unweighted bipartite graph
// Time Complexity: O((V + E) sqrt V)
// Memory Complexity: O(V + E)
template <const int MAXV> struct HopcroftKarpMaxMatch {
    int cardinality, mate[MAXV], dist[MAXV], q[MAXV], pathDist; vector<int> adj[MAXV], typeA, typeB;
    bool color[MAXV], inCover[MAXV], vis[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    bool hasPath() {
        pathDist = INT_MAX; int front = 0, back = 0;
        for (int v : typeA) {
            if (mate[v] == -1) { dist[v] = 0; q[back++] = v; }
            else dist[v] = INT_MAX;
        }
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v]) {
                if (mate[w] == -1) {
                    if (pathDist == INT_MAX) pathDist = dist[v] + 1;
                } else if (dist[mate[w]] == INT_MAX) {
                    dist[mate[w]] = dist[v] + 1;
                    if (pathDist == INT_MAX) q[back++] = mate[w];
                }
            }
        }
        return pathDist != INT_MAX;
    }
    bool dfs(int v) {
        for (int w : adj[v]) {
            if (mate[w] == -1) {
                if (pathDist == dist[v] + 1) { mate[w] = v; mate[v] = w; return true; }
            } else if (dist[mate[w]] == dist[v] + 1) {
                if (dfs(mate[w])) { mate[w] = v; mate[v] = w; return true; }
            }
        }
        dist[v] = INT_MAX; return false;
    }
    void init(int V = MAXV) {
        fill(mate, mate + V, -1); fill(color, color + V, false); typeA.clear(); typeB.clear();
        for (int i = 0; i < V; i++) adj[i].clear();
    }
    int getMaxMatch(int V) {
        cardinality = 0;
        for (int v = 0; v < V; v++) {
            if (color[v]) typeA.push_back(v);
            else typeB.push_back(v);
        }
        while (hasPath()) for (int v : typeA) if (mate[v] == -1 && dfs(v)) cardinality++;
        return cardinality;
    }
    void dfsVertexCover(int v) {
        if (vis[v]) return;
        vis[v] = true;
        for (int w : adj[v]) if ((mate[v] != w) == color[v]) dfsVertexCover(w);
    }
    void getVertexCover(int V) {
        fill(inCover, inCover + V, false); fill(vis, vis + V, false);
        for (int v : typeA) if (mate[v] == -1) dfsVertexCover(v);
        for (int v : typeA) inCover[v] = !vis[v];
        for (int v : typeB) inCover[v] = vis[v];
    }
};
