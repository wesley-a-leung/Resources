#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of non adjacent vertices on any path between
// a source an (optional) destination vertex on a Directed Acyclic Graph
// Time Complexity: O(V)
// Memory Complexity: O(V)
template <const int MAXV> struct MaxWeightedIndependentSet {
    vector<int> adj[MAXV]; int val[MAXV], dp[MAXV][2];
    void addEdge(int v, int w) { adj[v].push_back(w); }
    int dfs(int v, int t, bool take) {
        if (v == t) return take;
        if (dp[v][take] != -1) return dp[v][take];
        int ret = INT_MIN;
        for (int w : adj[v]) {
            if (take) ret = max(ret, dfs(w, false) + val[v]);
            ret = max(ret, dfs(w, true));
        }
        return dp[v][take] = ret;
    }
    void clear() { for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    int solve(int s, int t) {
        fill(dp, dp + MAXV, -1);
        return dfs(s, t, true);
    }
};
