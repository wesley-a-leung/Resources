#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of non adjacent vertices on any path between
// a source an (optional) destination vertex on a Directed Acyclic Graph
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct MaxWeightedIndependentPathDAG {
    vector<int> adj[MAXV]; int val[MAXV], dp[MAXV][2];
    void addEdge(int v, int w) { adj[v].push_back(w); }
    int dfs(int v, int t, bool take) {
        if (v == t) return take ? val[v] : 0;
        if (dp[v][take] != -1) return dp[v][take];
        int ret = INT_MIN;
        for (int w : adj[v]) {
            if (!take) ret = max(ret, dfs(w, true));
            ret = max(ret, dfs(w, false));
        }
        return take ? (dp[v][take] = ret + val[v]) : (dp[v][take] = ret);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    int solve(int V, int s, int t) {
        for (int i = 0; i < V; i++) dp[i][0] = dp[i][1] = -1;
        return dfs(s, t, true);
    }
};
