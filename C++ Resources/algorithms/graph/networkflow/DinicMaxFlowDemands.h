#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow in a flow network where edges have both a capcity and demand, using Dinic's algorithm
// Time Complexity:
//   O(V^2 E), much faster in practice
//   O(E min(V^(2/3), E^(1/2))) for unit capacities
//   O(VE log C) with capacity scaling, where C is the maximum capacity of any edge
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit, const bool SCALING> struct DinicMaxFlowDemands {
    unit INF, EPS; DinicMaxFlowDemands(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    struct Edge {
        int from, to; unit dem, cap, resCap, maxCap; int ind, rev; char isRev, type; Edge() {}
        Edge(int from, int to, unit dem, unit cap, unit maxCap, char isRev, char type) :
            from(from), to(to), dem(dem), cap(cap), resCap(cap), maxCap(maxCap), isRev(isRev), type(type) {}
        bool operator < (const Edge &other) const {
            if (from != other.from) return from < other.from;
            if (isRev != other.isRev) return isRev < other.isRev;
            return maxCap > other.maxCap;
        }
    };
    int E, cur[MAXV], level[MAXV], q[MAXV], st[MAXV], ind[MAXE * 2 + MAXV * 4];
    Edge e[MAXE * 2 + MAXV * 4]; unit maxFlow, maxCap, outDem[MAXV], inDem[MAXV];
    void addEdge(int v, int w, unit vwDem, unit vwCap, int type = 1) {
        e[E++] = Edge(v, w, vwDem, vwCap, vwCap, 0, type); e[E++] = Edge(w, v, -vwDem, -vwDem, vwCap - vwDem, 1, type);
        e[E - 2].ind = E - 2; e[E - 1].ind = E - 1; e[E - 2].rev = E - 1; e[E - 1].rev = E - 2; st[v + 1]++; st[w + 1]++;
        if (type == 1) { outDem[v] += vwDem; inDem[w] += vwDem; }
    }
    bool bfs(int V, int s, int t, unit lim, char r) {
        fill(level, level + V, -1); level[s] = 0; int front = 0, back = 0; q[back++] = s;
        while (front < back && level[t] == -1) {
            int v = q[front++];
            for (int i = st[v]; i < st[v + 1] && e[i].maxCap > lim && e[i].isRev <= r; i++) {
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
        for (int &i = cur[v]; i < st[v + 1] && e[i].maxCap > lim && e[i].isRev <= r; i++) {
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
    void init(int V) { E = 0; maxCap = 0; fill(st, st + V + 3, 0); fill(outDem, outDem + V, 0); fill(inDem, inDem + V, 0); }
    void build(int V) {
        sort(e, e + E); partial_sum(st, st + V + 1, st);
        for (int i = 0; i < E; i++) ind[e[i].ind] = i;
        for (int i = 0; i < E; i++) { e[e[i].ind = i].rev = ind[e[i].rev]; maxCap = max(maxCap, e[i].maxCap); }
    }
    unit dinic(int V, int s, int t) {
        for (char r = 1 - int(SCALING); r <= 1; r++) for (unit lim = SCALING ? maxCap : EPS; ; lim = max(lim / 2, EPS)) {
            while (bfs(V, s, t, lim, r)) { copy(st, st + V, cur); maxFlow += dfs(s, t, INF, lim, r); }
            if (lim <= EPS) break;
        }
        return maxFlow;
    }
    pair<bool, unit> findFeasibleFlow(int V, int s = -1, int t = -1) {
        int ss = V, tt = V + 1, EE = E;
        for (int i = 0; i < E; i++) { e[i].cap -= e[i].dem; e[i].resCap -= e[i].dem; e[i].maxCap -= e[i].dem; }
        for (int v = 0; v < V; v++) { addEdge(ss, v, 0, inDem[v], 2); addEdge(v, tt, 0, outDem[v], 2); }
        if (s != -1 && t != -1) addEdge(t, s, 0, INF, 2);
        build(V + 2); dinic(V + 2, ss, tt); pair<bool, unit> ret(true, 0);
        for (int i = st[ss]; i < st[ss + 1]; i++) if (e[i].resCap > EPS) ret.first = false;
        if (s != -1 && t != -1) for (int i = st[t]; i < st[t + 1]; i++) if (e[i].to == s) ret.second = e[i].cap - e[i].resCap;
        sort(e, e + E, [&] (const Edge &a, const Edge &b) { return a.type < b.type; }); E = EE; fill(st, st + V + 1, 0);
        for (int i = 0; i < E; i++) { st[e[i].to + 1]++; e[i].cap += e[i].dem; e[i].maxCap += e[i].dem; }
        build(V); return ret;
    }
    unit getFlow(int V, int s, int t) {
        pair<bool, unit> feasibleFlow = findFeasibleFlow(V, s, t); maxFlow = 0;
        if (feasibleFlow.first) { maxFlow += feasibleFlow.second; dinic(V, s, t); }
        return maxFlow;
    }
};
