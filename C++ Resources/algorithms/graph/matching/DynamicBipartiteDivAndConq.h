#pragma once
#include <bits/stdc++.h>
using namespace std;

// Support queries for whether a graph is bipartite or not, after edges have been added or removed
// Divide and Conquer Solution
// Time Complexity: O(V + Q log Q log V)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ, const bool ONE_INDEXED> struct DynamicBipartiteDivAndConq {
    int Q = 0, cnt, UF[MAXV]; bool P[MAXV]; vector<bool> ans; unordered_map<int, int> present[MAXV];
    vector<pair<pair<int, int>, pair<int, bool>>> history; vector<bool> history2;
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    int find(int v) { return UF[v] < 0 ? v : find(UF[v]); }
    bool parity(int v) { bool p = P[v]; for (; UF[v] >= 0; p ^= P[v]) v = UF[v]; return p; }
    void join(int v, int w) {
        int fv = find(v), fw = find(w); bool p = parity(v) ^ parity(w) ^ 1;
        if (fv == fw) { history2.push_back(p); cnt += history2.back(); return; }
        if (UF[fv] > UF[fw]) swap(fv, fw);
        history.emplace_back(make_pair(fv, fw), make_pair(UF[fw], P[fw])); UF[fv] += UF[fw]; UF[fw] = fv; P[fw] = p;
    }
    void undo() {
        int v = history.back().first.first, w = history.back().first.second, ufw = history.back().second.first;
        bool pfw = history.back().second.second; history.pop_back(); UF[w] = ufw; UF[v] -= UF[w]; P[w] = pfw;
    }
    void solve(int l, int r) {
        if (l == r && q[l].type == 0) ans.push_back(cnt == 0);
        if (l >= r) return;
        int m = l + (r - l) / 2, curSize = history.size(), curSize2 = history2.size();
        for (int i = m + 1; i <= r; i++) if (q[i].otherTime < l) join(q[i].v, q[i].w);
        solve(l, m);
        while ((int) history.size() > curSize) undo();
        while ((int) history2.size() > curSize2) { cnt -= history2.back(); history2.pop_back(); }
        for (int i = l; i <= m; i++) if (q[i].otherTime > r) join(q[i].v, q[i].w);
        solve(m + 1, r);
        while ((int) history.size() > curSize) undo();
        while ((int) history2.size() > curSize2) { cnt -= history2.back(); history2.pop_back(); }
    }
    void clear(int V = MAXV) { ans.clear(); Q = 0; history.clear(); history2.clear(); for (int i = 0; i < V; i++) present[i].clear(); }
    void addEdge(int v, int w) {
        if (v > w) swap(v, w);
        present[v][w] = Q; q[Q++] = {1, v, w, INT_MAX};
    }
    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = present[v][w]; q[Q] = {-1, v, w, insTime}; q[insTime].otherTime = Q++; present[v].erase(w);
    }
    void query() { q[Q] = {0, -1, -1, Q}; Q++; }
    void solve(int V) { cnt = 0; fill(UF, UF + V + ONE_INDEXED, -1); fill(P, P + V + ONE_INDEXED, 0); solve(0, Q - 1); }
};
