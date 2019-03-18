#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow and minimum cut in a flow network using Dinic's algorithm
// Time Complexity:
//   O(V^2 E), much faster in practice
//   O(E min(V^(2/3), E^(1/2))) for unit capacities
//   O(VE log C) with capacity scaling, where C is the maximum capacity of any edge
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit, const bool SCALING> struct DinicMaxFlow {
    unit INF, EPS; DinicMaxFlow(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    struct Edge {
        int from, to; unit origCap, cap, maxCap; int rev; Edge() {}
        Edge(int from, int to, unit cap, unit maxCap) : from(from), to(to), origCap(cap), cap(cap), maxCap(maxCap) {}
    };
    int E, cur[MAXV], level[MAXV], q[MAXV], st[MAXV], deg[MAXV], ord[MAXE * 2], ind[MAXE * 2];
    bool cut[MAXV]; Edge e[MAXE * 2]; unit maxFlow, minCut;
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e[E++] = Edge(v, w, vw, max(vw, wv)); e[E++] = Edge(w, v, wv, max(vw, wv));
        e[E - 2].rev = E - 1; e[E - 1].rev = E - 2; deg[v]++; deg[w]++;
    }
    bool bfs(int V, int s, int t, unit lim) {
        fill(level, level + V, -1); level[s] = 0; int front = 0, back = 0; q[back++] = s;
        while (front < back && level[t] == -1) {
            int v = q[front++];
            for (int i = st[v]; i < st[v] + deg[v] && e[i].maxCap > lim; i++)
                if (level[e[i].to] == -1 && e[i].cap > lim) { level[e[i].to] = level[v] + 1; q[back++] = e[i].to; }
        }
        return level[t] != -1;
    }
    unit dfs(int v, int t, unit flow, unit lim) {
        if (v == t || flow <= EPS) return flow;
        for (int i = cur[v]; i < st[v] + deg[v] && e[i].maxCap > lim; cur[v] = ++i) if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) {
            unit res = dfs(e[i].to, t, min(flow, e[i].cap), lim);
            if (res > EPS) { e[i].cap -= res; e[e[i].rev].cap += res; return res; }
        }
        return 0;
    }
    void init(int V = MAXV) { E = 0; fill(cut, cut + V, false); fill(deg, deg + V, 0); }
    unit getFlow(int V, int s, int t) {
        maxFlow = 0; iota(ord, ord + E, 0);
        stable_sort(ord, ord + E, [&] (const int &a, const int &b) {
            return e[a].from == e[b].from ? e[a].maxCap > e[b].maxCap : e[a].from < e[b].from;
        });
        for (int i = 0; i < E; i++) ind[ord[i]] = i;
        for (int i = 0; i < E; i++) e[i].rev = ind[e[i].rev];
        stable_sort(e, e + E, [&] (const Edge &a, const Edge &b) {
            return a.from == b.from ? a.maxCap > b.maxCap : a.from < b.from;
        });
        for (int v = 0, curSum = 0; v < V; v++) { st[v] = curSum; curSum += deg[v]; }
        for (unit lim = SCALING ? INF : EPS; ; lim /= 2) {
            while (bfs(V, s, t, lim)) {
                copy(st, st + V, cur);
                for (unit flow = dfs(s, t, INF, lim); flow > EPS; flow = dfs(s, t, INF, lim)) maxFlow += flow;
            }
            if (lim <= EPS) break;
        }
        return maxFlow;
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
