#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the articulation points and bridges of a graph,
// and decomposes the graph into biconnected ocmponents
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct Biconnected {
    int low[MAXV], pre[MAXV], cur; bool articulation[MAXV]; stack<pair<int, int>> s;
    vector<int> adj[MAXV]; vector<pair<int, int>> bridges; vector<unordered_set<int>> components;
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void dfs(int v, int prev) {
        int children = 0; pre[v] = low[v] = cur++;
        for (int w : adj[v]) {
            if (pre[w] == -1) {
                children++; s.emplace(v, w); dfs(w, v); low[v] = min(low[v], low[w]);
                if ((prev == v && children > 1) || (prev != v && low[w] >= pre[v])) {
                    articulation[v] = true; components.emplace_back();
                    while (s.top().first != v || s.top().second != w) {
                        components.back().insert(s.top().first); components.back().insert(s.top().second); s.pop();
                    }
                    components.back().insert(s.top().first); components.back().insert(s.top().second); s.pop();
                }
                if (low[w] == pre[w]) bridges.emplace_back(v, w);
            } else if (w != prev && pre[w] < low[v]) {
                low[v] = pre[w]; s.emplace(v, w);
            }
        }
    }
    void clear() { bridges.clear(); components.clear(); for (int i = 0; i < MAXV; i++) adj[i].clear(); }
    void run(int V) {
        cur = 0;
        for (int v = 0; v < V; v++) { low[v] = pre[v] = -1; articulation[v] = false; }
        for (int v = 0; v < V; v++) {
            if (pre[v] == -1) dfs(v, v);
            if (!s.empty()) {
                components.emplace_back();
                while (!s.empty()) {
                    components.back().insert(s.top().first); components.back().insert(s.top().second); s.pop();
                }
            }
        }
    }
};
