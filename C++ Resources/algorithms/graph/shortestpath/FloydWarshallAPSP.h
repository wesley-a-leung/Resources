#pragma once
#include <bits/stdc++.h>
using namespace std;

// Floyd Warshall's all pairs shortest path algorithm for weighted graphs
// Can be used to detect negative cycles
// Time Complexity: O(V^3)
// Memory Complexity: O(V^2)
template <const int MAXV, class unit> struct FloydWarshallAPSP {
    unit INF, dist[MAXV][MAXV]; bool hasNegativeCycle; FloydWarshallAPSP(unit INF) : INF(INF) {}
    void init(int V = MAXV) { for (int i = 0; i < V; i++) fill(dist[i], dist[i] + V, INF); }
    void addEdge(int v, int w, unit weight) { dist[v][w] = min(dist[v][w], weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void run(int V) {
        hasNegativeCycle = false;
        for (int v = 0; v < V; v++) dist[v][v] = 0;
        for (int u = 0; u < V; u++) for (int v = 0; v < V; v++) if (dist[v][u] < INF) for (int w = 0; w < V; w++)
            if (dist[u][w] < INF && dist[v][w] > dist[v][u] + dist[u][w]) dist[v][w] = dist[v][u] + dist[u][w]; 
        for (int u = 0; u < V; u++) for (int v = 0; v < V; v++) for (int w = 0; w < V; w++)
            if (dist[w][w] < 0 && dist[u][w] < INF && dist[w][v] < INF) { dist[u][v] = -INF; hasNegativeCycle = true; break; }
    }
};
