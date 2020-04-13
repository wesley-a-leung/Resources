#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Support queries for the number of components in a graph, after edges have been added or removed
// Divide and Conquer Solution
// Time Complexity: O(V + Q log Q log V)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ> struct DynamicConnectivityDivAndConq {
    int Q = 0; vector<int> ans; unordered_map<int, int> present[MAXV]; UnionFindUndo<MAXV> uf;
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    void solve(int l, int r) {
        if (l == r && q[l].type == 0) ans.push_back(uf.cnt);
        if (l >= r) return;
        int m = l + (r - l) / 2, curSize = uf.history.size();
        for (int i = m + 1; i <= r; i++) if (q[i].otherTime < l) uf.join(q[i].v, q[i].w);
        solve(l, m);
        while (int(uf.history.size()) > curSize) uf.undo();
        for (int i = l; i <= m; i++) if (q[i].otherTime > r) uf.join(q[i].v, q[i].w);
        solve(m + 1, r);
        while (int(uf.history.size()) > curSize) uf.undo();
    }
    void clear(int V = MAXV) { ans.clear(); Q = 0; for (int i = 0; i < V; i++) present[i].clear(); }
    void addEdge(int v, int w) {
        if (v > w) swap(v, w);
        present[v][w] = Q; q[Q++] = {1, v, w, INT_MAX};
    }
    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = present[v][w]; q[Q] = {-1, v, w, insTime}; q[insTime].otherTime = Q++; present[v].erase(w);
    }
    void query() { q[Q] = {0, -1, -1, Q}; Q++; }
    void solve(int V) { uf.init(V); solve(0, Q - 1); }
};
