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
        int from, to; unit origCap, cap, promised; int rev; char isRev; Edge() {}
        Edge(int from, int to, unit origCap, unit cap, char isRev) :
            from(from), to(to), origCap(origCap), cap(cap), isRev(isRev) {}
    };
    int E, front, back, level[MAXV], q[MAXV], st[MAXV], deg[MAXV], ord[MAXE * 2], ind[MAXE * 2];
    bool cut[MAXV]; Edge e[MAXE * 2]; unit maxFlow, minCut, maxCap, h[MAXV], pool[MAXV];
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e[E++] = Edge(v, w, max(vw, wv), vw, 0); e[E++] = Edge(w, v, max(vw, wv), wv, 1);
        e[E - 2].rev = E - 1; e[E - 1].rev = E - 2; deg[v]++; deg[w]++; maxCap = max(maxCap, max(vw, wv));
    }
    bool bfs(int V, int s, int t, unit lim, char r) {
        fill(level, level + V, -1); level[s] = 0; front = 0; back = 0; q[back++] = s;
        while (front < back && level[t] == -1) {
            int v = q[front++];
            for (int i = st[v]; i < st[v] + deg[v] && e[i].origCap > lim && e[i].isRev <= r; i++)
                if (level[e[i].to] == -1 && e[i].cap > EPS) { level[e[i].to] = level[v] + 1; q[back++] = e[i].to; }
        }
        return level[t] != -1;
    }
    unit tideCycle(int V, int s, int t, unit lim, char r) {
        fill(h, h + V, 0); h[s] = INF; // high tide
        for (int qind = 0; qind < back; qind++) {
            int v = q[qind];
            for (int i = st[v]; i < st[v] + deg[v] && e[i].origCap > lim && e[i].isRev <= r; i++)
                if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) { h[e[i].to] += e[i].promised = min(e[i].cap, h[e[i].from]); }
        }
        if (h[t] <= EPS) return 0;
        fill(pool, pool + V, 0); pool[t] = h[t]; // low tide
        for (int qind = back - 1; qind >= 0; qind--) {
            int v = q[qind];
            for (int i = st[v]; i < st[v] + deg[v] && e[i].origCap > lim && e[i].isRev <= r; i++) {
                if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) {
                    e[i].promised = min(e[i].promised, min(h[v] - pool[v], pool[e[i].to]));
                    pool[e[i].to] -= e[i].promised; pool[v] += e[i].promised;
                }
            }
        }
        fill(h, h + V, 0); h[s] = pool[s]; // erosion
        for (int qind = 0; qind < back; qind++) {
            int v = q[qind];
            for (int i = st[v]; i < st[v] + deg[v] && e[i].origCap > lim && e[i].isRev <= r; i++) {
                if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) {
                    e[i].promised = min(e[i].promised, h[v]); h[v] -= e[i].promised; h[e[i].to] += e[i].promised;
                    e[i].cap -= e[i].promised; e[e[i].rev].cap += e[i].promised;
                }
            }
        }
        return h[t];
    }
    void init(int V = MAXV) { E = 0; maxCap = 0; fill(cut, cut + V, false); fill(deg, deg + V, 0); }
    unit getFlow(int V, int s, int t) {
        maxFlow = 0; iota(ord, ord + E, 0);
        stable_sort(ord, ord + E, [&] (const int &a, const int &b) {
            if (e[a].from != e[b].from) return e[a].from < e[b].from;
            if (e[a].isRev != e[b].isRev) return e[a].isRev < e[b].isRev;
            return e[a].origCap > e[b].origCap;
        });
        for (int i = 0; i < E; i++) ind[ord[i]] = i;
        for (int i = 0; i < E; i++) e[i].rev = ind[e[i].rev];
        stable_sort(e, e + E, [&] (const Edge &a, const Edge &b) {
            if (a.from != b.from) return a.from < b.from;
            if (a.isRev != b.isRev) return a.isRev < b.isRev;
            return a.origCap > b.origCap;
        });
        for (int v = 0, curSum = 0; v < V; v++) { st[v] = curSum; curSum += deg[v]; }
        for (char r = 1 - int(SCALING); r <= 1; r++) for (unit lim = SCALING ? maxCap : EPS; ; lim /= 2) {
            while (bfs(V, s, t, lim, r)) maxFlow += tideCycle(V, s, t, lim, r);
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
