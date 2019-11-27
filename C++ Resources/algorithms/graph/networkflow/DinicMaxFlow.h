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
        int from, to; unit cap, resCap, maxCap; int ind, rev; char isRev; Edge() {}
        Edge(int from, int to, unit cap, unit maxCap, char isRev) :
            from(from), to(to), cap(cap), resCap(cap), maxCap(maxCap), isRev(isRev) {}
        bool operator < (const Edge &other) const {
            if (from != other.from) return from < other.from;
            if (isRev != other.isRev) return isRev < other.isRev;
            return maxCap > other.maxCap;
        }
    };
    int E, cur[MAXV], level[MAXV], q[MAXV], st[MAXV], ind[MAXE * 2]; bool cut[MAXV]; Edge e[MAXE * 2]; unit maxFlow, maxCap;
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e[E++] = Edge(v, w, vw, max(vw, wv), 0); e[E++] = Edge(w, v, wv, max(vw, wv), 1);
        e[E - 2].ind = E - 2; e[E - 1].ind = E - 1; e[E - 2].rev = E - 1; e[E - 1].rev = E - 2;
    }
    bool bfs(int V, int s, int t, unit lim, char r) {
        fill(level, level + V, -1); level[s] = 0; int front = 0, back = 0; q[back++] = s;
        while (front < back && level[t] == -1) {
            int v = q[front++];
            for (int i = st[v]; i < st[v + 1] && e[i].maxCap >= lim && e[i].isRev <= r; i++) {
                if (level[e[i].to] == -1 && e[i].resCap > EPS) {
                    level[e[i].to] = level[v] + 1; q[back++] = e[i].to;
                    if (level[t] != -1) return true;
                }
            }
        }
        return level[t] != -1;
    }
    unit dfs(int v, int t, unit flow, unit lim, char r) {
        if (v == t || flow <= EPS) return flow;
        unit ret = 0;
        for (int &i = cur[v]; i < st[v + 1] && e[i].maxCap >= lim && e[i].isRev <= r; i++) {
            if (e[i].resCap > EPS && level[e[i].to] == level[v] + 1) {
                unit res = dfs(e[i].to, t, min(flow, e[i].resCap), lim, r);
                if (res > EPS) {
                    e[i].resCap -= res; e[e[i].rev].resCap += res; flow -= res; ret += res;
                    if (flow <= EPS) break;
                }
            }
        }
        return ret;
    }
    void init(int V) { E = 0; maxCap = 0; fill(cut, cut + V, false); }
    void build(int V) {
        fill(st, st + V + 1, 0); sort(e, e + E);
        for (int i = 0; i < E; i++) st[e[ind[e[i].ind] = i].to + 1]++;
        partial_sum(st, st + V + 1, st);
        for (int i = 0; i < E; i++) { e[e[i].ind = i].rev = ind[e[i].rev]; maxCap = max(maxCap, e[i].maxCap); }
    }
    unit getFlow(int V, int s, int t) {
        build(V); maxFlow = 0;
        for (char r = 1 - int(SCALING); r <= 1; r++) for (unit lim = SCALING ? maxCap : EPS; ; lim = max(lim / 2, EPS)) {
            while (bfs(V, s, t, lim, r)) { copy(st, st + V, cur); maxFlow += dfs(s, t, INF, lim, r); }
            if (lim <= EPS) break;
        }
        return maxFlow;
    }
    void inferMinCutDfs(int v) {
        cut[v] = true;
        for (int i = st[v]; i < st[v + 1]; i++) if (e[i].resCap > EPS && !cut[e[i].to]) inferMinCutDfs(e[i].to);
    }
    unit inferMinCut(int V, int s) {
        inferMinCutDfs(s); unit minCut = 0;
        for (int v = 0; v < V; v++) if (cut[v]) for (int i = st[v]; i < st[v + 1]; i++) if (!cut[e[i].to]) minCut += e[i].cap;
        return minCut;
    }
};
