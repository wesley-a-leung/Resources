#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the diameter of a component, assuming the graph is a forest
// Time Complexity: O(V)
// Memory Complexity: O(V)
template <const int MAXV, class unit> struct TreeDiameter {
    vector<pair<int, unit>> adj[MAXV];
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); adj[w].emplace_back(v, weight); }
    pair<unit, int> dfs(int v, int prev, unit dist) {
        pair<unit, int> ret(dist, v);
        for (auto &&e : adj[v]) if (e.first != prev) ret = max(ret, dfs(e.first, v, dist + e.second));
        return ret;
    }
    pair<unit, pair<int, int>> getDiameter(int s = 0) { // returns the diameter, along with 2 vertices with that diameter
        pair<unit, int> t = dfs(s, -1, 0), u = dfs(t.second, -1, 0); return make_pair(u.first, make_pair(t.second, u.second));
    }
};
