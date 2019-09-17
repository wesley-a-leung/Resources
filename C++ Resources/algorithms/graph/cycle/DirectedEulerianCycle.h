#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a directed eulerian cycle (every edge travered exactly once, returning to the start vertex) in an directed graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct DirectedEulerianCycle {
    int front[MAXV], inDeg[MAXV], E; vector<int> cycle, adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); inDeg[w]++; E++; }
    void init(int V = MAXV) { E = 0; fill(inDeg, inDeg + V, 0); }
    void clear(int V = MAXV) { cycle.clear(); for (int v = 0; v < V; v++) adj[v].clear(); }
    bool run(int V) { // returns true if there is a directed eulerian cycle
        int s = -1;
        for (int v = 0; v < V; v++) if (int(adj[v].size()) > 0) { s = v; break; }
        for (int v = 0; v < V; v++) if (int(adj[v].size()) != inDeg[v]) return false;
        if (s == -1) s = 0;
        fill(front, front + V, 0); stack<int> stk; stk.push(s);
        while (!stk.empty()) {
            int v = stk.top(); stk.pop();
            for (; front[v] < int(adj[v].size()); v = adj[v][front[v]++]) stk.push(v);
            cycle.push_back(v);
        }
        if (int(cycle.size()) != E + 1) cycle.clear();
        reverse(cycle.begin(), cycle.end());
        return !cycle.empty();
    }
};
