#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow and minimum cut in a flow network using the tidal flow algorithm
// Time Complexity:
//   O(V E^2), much faster in practice
//   O(E^2 log C) with capacity scaling, where C is the maximum capacity of any edge
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit, const bool SCALING> struct TidalMaxFlow {
    unit INF, EPS; TidalMaxFlow(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    struct Edge {
        int from, to; unit origCap, cap, maxCap, promised; int rev; char isRev; Edge() {}
        Edge(int from, int to, unit cap, unit maxCap, char isRev) : from(from), to(to), origCap(cap), cap(cap), maxCap(maxCap), isRev(isRev) {}
    };
    int E, qesz, level[MAXV], q[MAXV], qe[MAXE * 2], st[MAXV], deg[MAXV], ord[MAXE * 2], ind[MAXE * 2];
    bool cut[MAXV]; Edge e[MAXE * 2]; unit maxFlow, minCut, maxCap, h[MAXV], pool[MAXV];
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e[E++] = Edge(v, w, vw, max(vw, wv), 0); e[E++] = Edge(w, v, wv, max(vw, wv), 1);
        e[E - 2].rev = E - 1; e[E - 1].rev = E - 2; deg[v]++; deg[w]++; maxCap = max(maxCap, max(vw, wv));
    }
    bool bfs(int V, int s, int t, unit lim, char r) {
        fill(level, level + V, -1); level[s] = 0; int front = 0, back = 0; qesz = 0; q[back++] = s;
        while (front < back) {
            int v = q[front++];
            for (int i = st[v]; i < st[v] + deg[v] && e[i].maxCap > lim && e[i].isRev <= r; i++) {
                if (e[i].cap > EPS && level[e[i].to] == -1) { level[e[i].to] = level[v] + 1; q[back++] = e[i].to; }
                if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) qe[qesz++] = i;
            }
        }
        return level[t] != -1;
    }
    unit tideCycle(int V, int s, int t) {
        fill(h, h + V, 0); h[s] = INF; // high tide
        for (int qind = 0; qind < qesz; qind++) { Edge &ei = e[qe[qind]]; h[ei.to] += ei.promised = min(ei.cap, h[ei.from]); }
        if (h[t] <= EPS) return 0;
        fill(pool, pool + V, 0); pool[t] = h[t]; // low tide
        for (int qind = qesz - 1; qind >= 0; qind--) {
            Edge &ei = e[qe[qind]]; ei.promised = min(ei.promised, min(h[ei.from] - pool[ei.from], pool[ei.to]));
            pool[ei.to] -= ei.promised; pool[ei.from] += ei.promised;
        }
        fill(h, h + V, 0); h[s] = pool[s]; // erosion
        for (int qind = 0; qind < qesz; qind++) {
            Edge &ei = e[qe[qind]]; ei.promised = min(ei.promised, h[ei.from]); h[ei.from] -= ei.promised; h[ei.to] += ei.promised;
            ei.cap -= ei.promised; e[ei.rev].cap += ei.promised;
        }
        return h[t];
    }
    void init(int V = MAXV) { E = 0; maxCap = 0; fill(cut, cut + V, false); fill(deg, deg + V, 0); }
    unit getFlow(int V, int s, int t) {
        maxFlow = 0; iota(ord, ord + E, 0);
        stable_sort(ord, ord + E, [&] (const int &a, const int &b) {
            if (e[a].from != e[b].from) return e[a].from < e[b].from;
            if (e[a].isRev != e[b].isRev) return e[a].isRev < e[b].isRev;
            return e[a].maxCap > e[b].maxCap;
        });
        for (int i = 0; i < E; i++) ind[ord[i]] = i;
        for (int i = 0; i < E; i++) e[i].rev = ind[e[i].rev];
        stable_sort(e, e + E, [&] (const Edge &a, const Edge &b) {
            if (a.from != b.from) return a.from < b.from;
            if (a.isRev != b.isRev) return a.isRev < b.isRev;
            return a.maxCap > b.maxCap;
        });
        for (int v = 0, curSum = 0; v < V; v++) { st[v] = curSum; curSum += deg[v]; }
        for (char r = 1 - int(SCALING); r <= 1; r++) for (unit lim = SCALING ? maxCap : EPS; ; lim = max(lim / 2, EPS)) {
            while (bfs(V, s, t, lim, r)) maxFlow += tideCycle(V, s, t);
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
