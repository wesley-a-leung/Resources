#pragma once
#include <bits/stdc++.h>
using namespace std;

// Cuthill-McKee reordering of a graph to reduce the bandwith
// Time Complexity: O(V log E + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct CuthillMcKee {
    int P[MAXV], ord[MAXV], ind[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void clear(int V = MAXV) { for (int v = 0; v < V; v++) adj[v].clear(); }
    void run(int V, bool rev) {
        auto cmpDeg = [&] (const int &v, const int &w) {
            return int(adj[v].size()) == int(adj[w].size()) ? v < w : int(adj[v].size()) < int(adj[w].size());
        };
        int front = 0, back = 0; fill(ind, ind + V, 0); iota(P, P + V, 0); sort(P, P + V, cmpDeg);
        for (int v = 0; v < V; v++) sort(adj[v].begin(), adj[v].end(), cmpDeg);
        for (int i = 0; i < V; i++) {
            int s = P[i];
            if (ind[s]) continue;
            ind[ord[back++] = s] = 1;
            while (front < back) {
                int v = ord[front++];
                for (int w : adj[v]) if (!ind[w]) ind[ord[back++] = w] = 1;
            }
        }
        if (rev) reverse(ord, ord + V);
        for (int i = 0; i < V; i++) ind[ord[i]] = i;
    }
};
