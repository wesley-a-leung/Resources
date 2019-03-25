#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bidirectional Breadth First Search of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct DirectedBidirectionalBFS {
    int dist[MAXV], src[MAXV], to[MAXV], q[MAXV]; vector<int> adj[MAXV][2]; pair<int, int> edgeOnPath;
    void addEdge(int v, int w) { adj[v][0].push_back(w); adj[w][1].push_back(v); }
    void clear(int V = MAXV) { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    int bfs(int V, int s, int t) {
        if (s == t) return 0;
        fill(dist, dist + V, INT_MAX); fill(to, to + V, -1); int front = 0, back = 0;
        dist[s] = dist[t] = 0; q[back++] = src[s] = s; q[back++] = src[t] = t;
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v][src[v] == t]) {
                if (dist[w] == INT_MAX) { dist[w] = dist[v] + 1; src[w] = src[v]; to[w] = v; q[back++] = w; }
                else if (src[v] != src[w]) { edgeOnPath = make_pair(v, w); return dist[v] + dist[w] + 1; }
            }
        }
        return -1;
    }
};
