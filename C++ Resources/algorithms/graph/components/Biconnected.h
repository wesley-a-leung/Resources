#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the articulation points and bridges of a graph,
// and decomposes the graph into biconnected components
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct Biconnected {
    int low[MAXV], pre[MAXV], id[MAXV], cur, forestN; bool articulation[MAXV]; stack<pair<int, int>> s; vector<int> isBridge[MAXV];
    vector<int> adj[MAXV], rev[MAXV], forest[MAXV]; vector<vector<int>> components;
    void addEdge(int v, int w) {
        adj[v].push_back(w); adj[w].push_back(v);
        isBridge[v].push_back(false); isBridge[w].push_back(false);
        rev[v].push_back(int(rev[w].size())); rev[w].push_back(int(rev[v].size()) - 1);
    }
    void dfs(int v, int prev) {
        int children = 0; pre[v] = low[v] = cur++;
        for (int i = 0; i < int(adj[v].size()); i++) {
            int w = adj[v][i];
            if (pre[w] == -1) {
                children++; s.emplace(v, w); dfs(w, v); low[v] = min(low[v], low[w]);
                if ((prev == v && children > 1) || (prev != v && low[w] >= pre[v])) {
                    articulation[v] = true; components.emplace_back();
                    while (s.top().first != v || s.top().second != w) {
                        components.back().push_back(s.top().first); components.back().push_back(s.top().second); s.pop();
                    }
                    components.back().push_back(s.top().first); components.back().push_back(s.top().second); s.pop();
                }
                if (low[w] == pre[w]) {
                    isBridge[v][i] = true;
                    isBridge[w][rev[v][i]] = true;
                }
            } else if (w != prev && pre[w] < low[v]) {
                low[v] = pre[w]; s.emplace(v, w);
            }
        }
    }
    void dfs2(int v, int curId) {
        id[v] = curId;
        for (int i = 0; i < int(adj[v].size()); i++) if (id[adj[v][i]] == -1 && !isBridge[v][i]) dfs2(adj[v][i], curId);
    }
    void clear(int V = MAXV) { components.clear(); for (int i = 0; i < V; i++) { adj[i].clear(); isBridge[i].clear(); rev[i].clear(); } }
    void run(int V) {
        cur = 0;
        for (int v = 0; v < V; v++) { low[v] = pre[v] = -1; articulation[v] = false; }
        for (int v = 0; v < V; v++) {
            if (pre[v] == -1) dfs(v, v);
            if (!s.empty()) {
                components.emplace_back();
                while (!s.empty()) {
                    components.back().push_back(s.top().first); components.back().push_back(s.top().second); s.pop();
                }
            }
        }
        for (auto &&comp : components) {
            sort(comp.begin(), comp.end());
            comp.erase(unique(comp.begin(), comp.end()), comp.end());
        }
    }
    void genForest(int V) {
        fill(id, id + V, -1); forestN = 0;
        for (int v = 0; v < V; v++) if (id[v] == -1) dfs2(v, forestN++);
        for (int v = 0; v < V; v++) for (int w : adj[v]) if (id[v] != id[w]) forest[id[v]].push_back(id[w]);
    }
};
