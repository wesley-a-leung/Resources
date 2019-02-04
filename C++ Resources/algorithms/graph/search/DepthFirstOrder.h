#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes Depth First Orders of a graph (pre order, post order, topological / reverse post order)
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct DFSOrder {
    int preInd[MAXV], postInd[MAXV], revPostInd[MAXV], preOrd[MAXV], postOrd[MAXV], revPostOrd[MAXV], curPre, curPost, curRevPost;
    vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void dfs(int v) {
        preOrd[preInd[v] = curPre++] = v;
        for (int w : adj[v]) if (preInd[w] == -1) dfs(w);
        postOrd[postInd[v] = curPost++] = revPostOrd[revPostInd[v] = curRevPost--] = v;
    }
    void run(int V, int s = 0) {
        fill(preInd, preInd + V, -1); curPre = curPost = 0; curRevPost = V - 1; dfs(s);
        for (int v = 0; v < V; v++) if (preInd[v] == -1) dfs(v);
    }
};

// Computes the Topological of a graph iteratively
// Time Complexity: O(V + E)
// Memory Complexity: O(V + E)
template <const int MAXV> struct TopologicalOrder {
    int ind[MAXV], ord[MAXV], inDeg[MAXV]; vector<int> adj[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); inDeg[w]++; }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void init(int V = MAXV) { for (int i = 0; i < V; i++) inDeg[i] = 0; }
    void run(int V) {
        int front = 0, back = 0;
        for (int v = 0; v < V; v++) if (inDeg[v] == 0) ord[back++] = v;
        while (front < back) {
            int v = ord[front]; ind[v] = front++;
            for (int w : adj[v]) if (--inDeg[w] == 0) ord[back++] = w;
        }
    }
};
