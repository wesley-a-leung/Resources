#pragma once
#include <bits/stdc++.h>
using namespace std;

// Support queries for the number of components in a graph, after edges have been added or removed
// Divide and Conquer Solution
// Time Complexity: O(V + Q log Q)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ> struct DynamicConnectivityDivAndConq {
    int Q = 0, cnt, UF[MAXV]; vector<int> ans; unordered_map<int, int> present[MAXV]; stack<pair<pair<int ,int>, int>> history;
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    int find(int v) { while (UF[v] >= 0) v = UF[v]; return v; }
    bool join(int v, int w) {
        v = find(v); w = find(w);
        if (v == w) return false;
        if (UF[v] > UF[w]) swap(v, w);
        history.push({{v, w}, UF[w]}); UF[v] += UF[w]; UF[w] = v; cnt--;
        return true;
    }
    void undo() {
        int v = history.top().first.first, w = history.top().first.second, ufw = history.top().second;
        history.pop(); UF[w] = ufw; UF[v] -= UF[w]; cnt++;
    }
    void solve(int l, int r) {
        if (l == r && q[l].type == 0) ans.push_back(cnt);
        if (l >= r) return;
        int m = l + (r - l) / 2, curSize = history.size();
        for (int i = m + 1; i <= r; i++) if (q[i].otherTime < l) join(q[i].v, q[i].w);
        solve(l, m);
        while ((int) history.size() > curSize) undo();
        for (int i = l; i <= m; i++) if (q[i].otherTime > r) join(q[i].v, q[i].w);
        solve(m + 1, r);
        while ((int) history.size() > curSize) undo();
    }
    void clear() { ans.clear(); Q = 0; for (int i = 0; i < MAXV; i++) present[i].clear(); }
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
        cnt = V; fill(UF, UF + MAXV, -1);
        solve(0, Q - 1);
    }
};
