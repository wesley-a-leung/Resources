#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bidirectional Breadth First Search of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct BidirectionalBFS {
    int dist[MAXV], to[MAXV], q[MAXV], vis[MAXV], stamp = 0; vector<int> adj[MAXV]; pair<int, int> edgeOnPath;
    void addBiEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void clear(int V = MAXV) { stamp = 0; for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    int run(int V, int s, int t) {
        if (s == t) return 0;
        if (stamp == 0) fill(vis, vis + V, stamp);
        stamp++; int front = 0, back = 0; dist[s] = dist[t] = 0; q[back++] = s; q[back++] = t; vis[s] = stamp; vis[t] = -stamp;
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v]) {
                if (vis[v] == -vis[w]) { edgeOnPath = make_pair(v, w); return dist[v] + dist[w] + 1; }
                else if (vis[v] != vis[w]) { dist[w] = dist[v] + 1; vis[w] = vis[v]; to[w] = v; q[back++] = w; }
            }
        }
        return -1;
    }
};
