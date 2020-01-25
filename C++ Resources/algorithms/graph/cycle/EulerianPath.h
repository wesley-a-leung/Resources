#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds an eulerian path (every edge travered exactly once) in an undirected graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct EulerianPath {
    struct Edge {
        int v, w; bool used;
        int other(int u) { return u == v ? w : v; }
    };
    int front[MAXV]; vector<Edge> e; vector<int> path, adj[MAXV], q[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void clear(int V = MAXV) { for (int v = 0; v < V; v++) adj[v].clear(); }
    bool run(int V) { // returns true if there is an eulerian path
        e.clear(); path.clear(); int odd = 0, s = -1;
        for (int v = 0; v < V; v++) if (int(adj[v].size()) > 0) { s = v; break; }
        for (int v = 0; v < V; v++) if (int(adj[v].size()) % 2 != 0) { odd++; s = v; }
        if (odd > 2) return false;
        if (s == -1) s = 0;
        for (int v = 0; v < V; v++) { front[v] = 0; q[v].clear(); }
        for (int v = 0; v < V; v++) {
            int selfLoops = 0;
            for (int w : adj[v]) {
                if (v == w) {
                    if (selfLoops % 2 == 0) { q[v].push_back(int(e.size())); q[w].push_back(int(e.size())); e.push_back({v, w, false}); }
                    selfLoops++;
                } else if (v < w) { q[v].push_back(int(e.size())); q[w].push_back(int(e.size())); e.push_back({v, w, false}); }
            }
        }
        stack<int, vector<int>> stk; stk.push(s);
        while (!stk.empty()) {
            int v = stk.top(); stk.pop();
            while (front[v] < int(q[v].size())) {
                int ei = q[v][front[v]++];
                if (e[ei].used) continue;
                e[ei].used = true; stk.push(v); v = e[ei].other(v);
            }
            path.push_back(v);
        }
        if (int(path.size()) != int(e.size()) + 1) path.clear();
        return !path.empty();
    }
};
