#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a directed eulerian path (every edge travered exactly once) in an directed graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct DirectedEulerianPath {
    int front[MAXV], inDeg[MAXV], E; vector<int> path, adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); inDeg[w]++; E++; }
    void init(int V = MAXV) { E = 0; fill(inDeg, inDeg + V, 0); }
    void clear(int V = MAXV) { path.clear(); for (int v = 0; v < V; v++) adj[v].clear(); }
    bool run(int V) { // returns true if there is a directed eulerian path
        int deficit = 0, s = -1;
        for (int v = 0; v < V; v++) if (int(adj[v].size()) > 0) { s = v; break; }
        for (int v = 0; v < V; v++) if (int(adj[v].size()) > inDeg[v]) { deficit += int(adj[v].size()) - inDeg[v]; s = v; }
        if (deficit > 1) return false;
        if (s == -1) s = 0;
        fill(front, front + V, 0); stack<int> stk; stk.push(s);
        while (!stk.empty()) {
            int v = stk.top(); stk.pop();
            for (; front[v] < int(adj[v].size()); v = adj[v][front[v]++]) stk.push(v);
            path.push_back(v);
        }
        if (int(path.size()) != E + 1) path.clear();
        reverse(path.begin(), path.end());
        return !path.empty();
    }
};
