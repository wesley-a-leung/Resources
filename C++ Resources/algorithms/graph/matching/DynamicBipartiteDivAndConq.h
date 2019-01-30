#pragma once
#include <bits/stdc++.h>
using namespace std;

// Support queries for whether a graph is bipartite or not, after edges have been added or removed
// Divide and Conquer Solution
// Time Complexity: O(V + Q (log Q) ^ 2)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXQ> struct DynamicBipartiteDivAndConq {
    int Q = 0, cnt, UF[MAXV]; bool P[MAXV]; vector<bool> ans; unordered_map<int, int> present[MAXV];
    stack<pair<pair<int ,int>, pair<int, bool>>> history; stack<bool> history2;
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    int find(int v) { while (UF[v] >= 0) v = UF[v]; return v; }
    bool parity(int v) { bool p = P[v]; for (; UF[v] >= 0; p ^= P[v]) v = UF[v]; return p; }
    bool join(int v, int w) {
        int fv = find(v), fw = find(w); bool p = parity(v) ^ parity(w) ^ 1;
        if (fv == fw) { history2.push(p); cnt += history2.top(); return false; }
        if (UF[fv] > UF[fw]) swap(fv, fw);
        history.push({{fv, fw}, {UF[fw], P[fw]}}); UF[fv] += UF[fw]; UF[fw] = fv; P[fw] = p;
        return true;
    }
    void undo() {
        int v = history.top().first.first, w = history.top().first.second, ufw = history.top().second.first;
        bool pfw = history.top().second.second; history.pop(); UF[w] = ufw; UF[v] -= UF[w]; P[w] = pfw;
    }
    void solve(int l, int r) {
        if (l == r && q[l].type == 0) ans.push_back(cnt == 0);
        if (l >= r) return;
        int m = l + (r - l) / 2, curSize = history.size(), curSize2 = history2.size();
        for (int i = m + 1; i <= r; i++) if (q[i].otherTime < l) join(q[i].v, q[i].w);
        solve(l, m);
        while ((int) history.size() > curSize) undo();
        while ((int) history2.size() > curSize2) { cnt -= history2.top(); history2.pop(); }
        for (int i = l; i <= m; i++) if (q[i].otherTime > r) join(q[i].v, q[i].w);
        solve(m + 1, r);
        while ((int) history.size() > curSize) undo();
        while ((int) history2.size() > curSize2) { cnt -= history2.top(); history2.pop(); }
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
    void solve() { cnt = 0; fill(UF, UF + MAXV, -1); fill(P, P + MAXV, 0); solve(0, Q - 1); }
};
