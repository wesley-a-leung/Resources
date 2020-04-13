#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes whether an undirected graph is bipartite, or whether it has an odd cycle.
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct Bipartite {
    bool bipartite, color[MAXV], vis[MAXV]; int to[MAXV], q[MAXV]; vector<int> oddCycle, adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void bfs(int s) {
        color[s] = false; vis[s] = true; int front = 0, back = 0; q[back++] = s;
        while (front < back) {
            int v = q[front++];
            for (int w : adj[v]) {
                if (!vis[w]) { vis[w] = true; to[w] = v; color[w] = !color[v]; q[back++] = w; }
                else if (color[w] == color[v]) {
                    bipartite = false; oddCycle.clear(); stack<int, vector<int>> stk; int x = v, y = w;
                    while (x != y) { stk.push(x); oddCycle.push_back(y); x = to[x]; y = to[y]; }
                    stk.push(x);
                    while (!stk.empty()) { oddCycle.push_back(stk.top()); stk.pop(); }
                    oddCycle.push_back(w);
                    return;
                }
            }
        }
    }
    bool run(int V) { // returns true if there the graph is bipartite
        bipartite = true; fill(color, color + V, false); fill(vis, vis + V, false);
        for (int v = 0; v < V && bipartite; v++) if (!vis[v]) bfs(v);
        return oddCycle.empty();
    }
    void clear(int V = MAXV) { oddCycle.clear(); for (int i = 0; i < V; i++) adj[i].clear(); }
};
