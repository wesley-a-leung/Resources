#pragma once
#include <bits/stdc++.h>
using namespace std;

// Counts the number of triangles in a graph, ignoring duplicate edges
// Time Complexity: O(V log E + E sqrt E log E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct Triangles {
    int ord[MAXV]; vector<int> adj[MAXV], adj2[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void clear(int V = MAXV) { for (int v = 0; v < MAXV; v++) { adj[v].clear(); adj2[v].clear(); } }
    int run(int V) {
        for (int v = 0; v < V; v++) {
            sort(adj[v].begin(), adj[v].end());
            adj[v].erase(unique(adj[v].begin(), adj[v].end()), adj[v].end());
        }
        iota(ord, ord + V, 0); int cnt = 0;
        sort(ord, ord + V, [&] (const int &i, const int &j) {
            return make_pair(int(adj[i].size()), i) < make_pair(int(adj[j].size()), j);
        });
        for (int v = 0; v < V; v++) {
            adj2[v] = adj[v]; sort(adj2[v].begin(), adj2[v].end());
            sort(adj[v].begin(), adj[v].end(), [&] (const int &i, const int &j) {
                return make_pair(int(adj[i].size()), i) > make_pair(int(adj[j].size()), j);
            });
        }
        for (int i = 0; i < V; i++) {
            int v = ord[i];
            for (int j = 0; j < int(adj[v].size()); j++) {
                int w = adj[v][j];
                if (make_pair(int(adj[w].size()), w) < make_pair(int(adj[v].size()), v)) break;
                for (int k = 0; k < j; k++) {
                    int u = adj[v][k];
                    if (binary_search(adj2[u].begin(), adj2[u].end(), w)) cnt++;
                }
            }
        }
        return cnt;
    }
};
