#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bidirectional Breadth First Search of a graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct BFS {
    int dist[MAXV], src[MAXV], to[MAXV]; vector<int> adj[MAXV]; pair<int, int> edgeOnPath;
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear() { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    int bfs(int s, int t) {
        if (s == t) return 0;
        fill(dist, dist + MAXV, INT_MAX); fill(to, to + MAXV, -1);
        queue<int> q; dist[s] = dist[t] = 0; q.push(src[s] = s); q.push(src[t] = t);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int w : adj[v]) {
                if (dist[w] == INT_MAX) { dist[w] = dist[v] + 1; src[w] = v; to[w] = v; q.push(w); }
                else if (src[v] != src[w]) { edgeOnPath = make_pair(v, w); return dist[v] + dist[w] + 1; }
            }
        }
        return -1;
    }
};
