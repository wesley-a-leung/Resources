#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum weighted independent set for a tree
// Time Complexity: O(V)
// Memory Complexity: O(V)
template <const int MAXV> struct MaxWeightedIndependentSet {
    vector<int> adj[MAXV]; int val[MAXV], dp[MAXV][2];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(w); }
    int dfs(int v, int prev, bool take) {
        if (dp[v][take] != -1) return dp[v][take];
        int ret = INT_MIN, cnt = 0;
        for (int w : adj[v]) {
            if (w == prev) continue;
            cnt++;
            if (take) ret = max(ret, dfs(w, false) + val[v]);
            ret = max(ret, dfs(w, true));
        }
        if (cnt == 0) return take ? val[v] : 0;
        return dp[v][take] = ret;
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    int solve(int V, int root = 0) {
        for (int i = 0; i < V; i++) dp[i][0] = dp[i][1] = -1;
        return max(dfs(root, true), dfs(root, false));
    }
};
