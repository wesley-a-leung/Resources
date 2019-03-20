#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow and minimum cut in a flow network using the Push Relabel algorithm
// with the highest label selection rule and gap relabelling heuristic
// Time Complexity: O(V^2 sqrt E), faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit> struct PushRelabelMaxFlow {
    struct Edge {
        int from, to; unit origCap, cap; int ind, rev; Edge() {}
        Edge(int from, int to, unit cap) : from(from), to(to), origCap(cap), cap(cap) {}
    };
    unit INF, EPS, maxFlow, minCut, ex[MAXV]; PushRelabelMaxFlow(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    int E, deg[MAXV], st[MAXV], ind[MAXE * 2], h[MAXV], cnt[MAXV * 2], cur[MAXV];
    bool cut[MAXV]; Edge _e[MAXE * 2], e[MAXE * 2]; vector<int> hs[MAXV * 2];
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        if (v == w) return;
        _e[E++] = Edge(v, w, vw); _e[E++] = Edge(w, v, wv); deg[v]++; deg[w]++;
        _e[E - 2].ind = E - 2; _e[E - 1].ind = E - 1; _e[E - 2].rev = E - 1; _e[E - 1].rev = E - 2;
    }
    void init(int V = MAXV) { E = 0; fill(deg, deg + V, 0); fill(cut, cut + V, false); }
    void push(int i, unit df) {
        int v = e[i].from, w = e[i].to;
        if (abs(ex[w]) <= EPS && df > EPS) hs[h[w]].push_back(w);
        e[i].cap -= df; e[e[i].rev].cap += df; ex[v] -= df; ex[w] += df;
    }
    unit getFlow(int V, int s, int t) {
        for (int v = 0, curSum = 0; v < V; v++) { st[v] = curSum; curSum += deg[v]; }
        for (int i = 0; i < E; i++) { e[ind[_e[i].ind] = st[_e[i].from]++] = _e[i]; }
        for (int i = 0; i < E; i++) e[i].rev = ind[e[i].rev];
        for (int v = 0, curSum = 0; v < V; v++) { cur[v] = st[v] = curSum; curSum += deg[v]; }
        fill(h, h + V, 0); h[s] = V; fill(ex, ex + V, 0); ex[t] = 1; fill(cnt, cnt + 2 * V, 0); cnt[0] = V - 1;
        for (int i = 0; i < 2 * V; i++) hs[i].clear();
        for (int i = st[s]; i < st[s] + deg[s]; i++) push(i, e[i].cap);
        if (!hs[0].empty()) for (int hi = 0; hi >= 0;) {
            int v = hs[hi].back(); hs[hi].pop_back();
            while (ex[v] > EPS) {
                if (cur[v] == st[v] + deg[v]) {
                    h[v] = INT_MAX;
                    for (int i = st[v]; i < st[v] + deg[v]; i++)
                        if (e[i].cap > EPS && h[v] > h[e[i].to] + 1) { h[v] = h[e[i].to] + 1; cur[v] = i; }
                    cnt[h[v]]++;
                    if (--cnt[hi] == 0 && hi < V) for (int i = 0; i < V; i++) if (hi < h[i] && h[i] < V) { cnt[h[i]]--; h[i] = V + 1; }
                    hi = h[v];
                } else if (e[cur[v]].cap > EPS && h[v] == h[e[cur[v]].to] + 1) push(cur[v], min(ex[v], e[cur[v]].cap));
                else cur[v]++;
            }
            while (hi >= 0 && hs[hi].empty()) hi--;
        }
        return maxFlow = -ex[s];
    }
    void inferMinCutDfs(int v) {
        cut[v] = true;
        for (int i = st[v]; i < st[v] + deg[v]; i++) if (e[i].cap > EPS && !cut[e[i].to]) inferMinCutDfs(e[i].to);
    }
    unit inferMinCut(int V, int s) {
        inferMinCutDfs(s); minCut = 0;
        for (int v = 0; v < V; v++) if (cut[v]) for (int i = st[v]; i < st[v] + deg[v]; i++) if (!cut[e[i].to]) minCut += e[i].origCap;
        return minCut;
    }
};
