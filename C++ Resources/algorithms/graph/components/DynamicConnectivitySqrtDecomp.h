#pragma once
#include <bits/stdc++.h>
#include "../../../datastructures/UnionFindUndo.h"
using namespace std;

// Support queries for the number of components in a graph, after edges have been added or removed
// Sqrt Decomposition Solution
// Time Complexity: O((Q / B) * (V + Q + B^2))
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ, const int BLOCKSZ> struct DynamicConnectivitySqrtDecomp {
    int Q = 0; vector<int> ans; unordered_map<int, int> present[MAXV]; UnionFindUndo<MAXV, 0> uf;
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    void clear(int V = MAXV) { ans.clear(); Q = 0; uf.clearHistory(); for (int i = 0; i < V; i++) present[i].clear(); }
    void addEdge(int v, int w) {
        if (v > w) swap(v, w);
        present[v][w] = Q; q[Q++] = {1, v, w, INT_MAX};
    }
    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = present[v][w]; q[Q] = {-1, v, w, insTime}; q[insTime].otherTime = Q++; present[v].erase(w);
    }
    void query() { q[Q] = {0, -1, -1, Q}; Q++; }
    void solve(int V) {
        for (int st = 0; st < Q; st += BLOCKSZ) {
            int en = min(Q, st + BLOCKSZ); uf.init(V);
            for (int i = 0; i < st; i++) if (q[i].type == 1 && q[i].otherTime >= en) uf.join(q[i].v, q[i].w);
            for (int i = st; i < en; i++) if (q[i].type == 0) {
                int curSize = uf.history.size();
                for (int j = st; j <= i; j++) if (q[j].type == 1 && q[j].otherTime > i) uf.join(q[j].v, q[j].w);
                for (int j = i + 1; j < en; j++) if (q[j].type == -1 && q[j].otherTime < st) uf.join(q[j].v, q[j].w);
                ans.push_back(uf.cnt);
                while (int(uf.history.size()) > curSize) uf.undo();
            }
            uf.clearHistory();
        }
    }
};
