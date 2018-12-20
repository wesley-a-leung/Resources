#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow and minimum cut in a flow network using Dinic's algorithm
// Time Complexity: O(V^2 * E), O(min(V^(2/3), E^(1/2)) * E) for unit capacities
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct DinicMaxFlow {
    unit INF, EPS; DinicMaxFlow(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    struct Edge {
        int to; unit cap, origCap; int next;
        Edge(int to, unit cap, int next) : to(to), cap(cap), origCap(cap), next(next) {}
    };
    int level[MAXV], last[MAXV]; bool vis[MAXV], cut[MAXV]; vector<Edge> e; unit maxFlow, minCut;
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e.emplace_back(w, vw, last[v]); last[v] = int(e.size()) - 1;
        e.emplace_back(v, wv, last[w]); last[w] = int(e.size()) - 1;
    }
    bool bfs(int V, int s, int t) {
        fill(level, level + V, -1); level[s] = 0; queue<int> q; q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int i = last[v]; i != -1; i = e[i].next)
                if (e[i].cap > EPS && level[e[i].to] == -1) { level[e[i].to] = level[v] + 1; q.push(e[i].to); }
        }
        return level[t] != -1;
    }
    unit dfs(int v, int t, unit flow) {
        if (v == t) return flow;
        unit ret = 0;
        for (int i = last[v]; i != -1; i = e[i].next) if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) {
            unit res = dfs(e[i].to, t, min(flow, e[i].cap));
            ret += res; e[i].cap -= res; e[i ^ 1].cap += res; flow -= res;
            if (abs(flow) <= EPS) break;
        }
        return ret;
    }
    void init() { fill(last, last + MAXV, -1); fill(cut, cut + MAXV, false); }
    void clear() { e.clear(); }
    unit getFlow(int V, int s, int t) {
        maxFlow = 0;
        while (bfs(V, s, t)) maxFlow += dfs(s, t, INF);
        return maxFlow;
    }
    void inferMinCutDfs(int v) {
        cut[v] = true;
        for (int i = last[v]; i != -1; i = e[i].next) if (e[i].cap > 0 && !cut[e[i].to]) inferMinCutDfs(e[i].to);
    }
    unit inferMinCut(int V, int s) {
        inferMinCutDfs(s); minCut = 0;
        for (int v = 0; v < V; v++) if (cut[v]) for (int i = last[v]; i != -1; i = e[i].next)
            if (!cut[e[i].to]) minCut += e[i].origCap;
        return minCut;
    }
};
