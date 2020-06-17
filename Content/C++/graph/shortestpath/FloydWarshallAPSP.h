#pragma once
#include <bits/stdc++.h>
using namespace std;

// Floyd Warshall's all pairs shortest path algorithm for weighted graphs with negative cycles
// Can be used to detect negative cycles
// Time Complexity: O(V^3)
// Memory Complexity: O(V^2)
template <const int MAXV, class unit> struct FloydWarshallAPSP {
    unit INF, dist[MAXV][MAXV]; int nxt[MAXV][MAXV]; bool hasNegativeCycle; FloydWarshallAPSP(unit INF) : INF(INF) {}
    void init(int V = MAXV) { for (int i = 0; i < V; i++) { fill(dist[i], dist[i] + V, INF); fill(nxt[i], nxt[i] + V, -1); dist[i][i] = 0; } }
    void addEdge(int v, int w, unit weight) { dist[v][w] = min(dist[v][w], weight); nxt[v][w] = w; }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void run(int V) {
        hasNegativeCycle = false;
        for (int u = 0; u < V; u++) for (int v = 0; v < V; v++) if (dist[v][u] < INF) for (int w = 0; w < V; w++)
            if (dist[u][w] < INF && dist[v][w] > dist[v][u] + dist[u][w]) { dist[v][w] = dist[v][u] + dist[u][w]; nxt[v][w] = nxt[v][u]; }
        for (int u = 0; u < V; u++) for (int v = 0; v < V; v++) for (int w = 0; w < V; w++)
            if (dist[w][w] < 0 && dist[u][w] < INF && dist[w][v] < INF) { dist[u][v] = -INF; hasNegativeCycle = true; break; }
    }
    vector<int> getPath(int v, int w) {
        if (nxt[v][w] == -1) return vector<int>();
        vector<int> path; path.push_back(v);
        while (v != w) path.push_back(v = nxt[v][w]);
        return path;
    }
};
