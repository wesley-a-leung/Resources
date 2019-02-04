#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow and minimum cut in a flow network using Edmonds Karp's algorithm
// Time Complexity: O(V * E^2)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct EdmondsKarpMaxFlow {
    unit INF, EPS; EdmondsKarpMaxFlow(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    struct Edge {
        int to; unit cap, origCap; int next;
        Edge(int to, unit cap, int next) : to(to), cap(cap), origCap(cap), next(next) {}
    };
    int to[MAXV], last[MAXV], q[MAXV]; bool vis[MAXV], cut[MAXV]; vector<Edge> e; unit maxFlow, minCut;
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e.emplace_back(w, vw, last[v]); last[v] = int(e.size()) - 1;
        e.emplace_back(v, wv, last[w]); last[w] = int(e.size()) - 1;
    }
    bool bfs(int V, int s, int t) {
        fill(vis, vis + V, false); fill(to, to + V, -1); int front = 0, back = 0; q[back++] = s; vis[s] = true;
        while (front < back) {
            int v = q[front++];
            for (int i = last[v]; i != -1; i = e[i].next)
                if (e[i].cap > EPS && !vis[e[i].to]) { vis[e[i].to] = true; to[e[i].to] = i; q[back++] = e[i].to; }
        }
        return vis[t];
    }
    void init(int V) { fill(last, last + V, -1); fill(cut, cut + V, false); }
    void clear() { e.clear(); }
    unit getFlow(int V, int s, int t) {
        maxFlow = 0;
        while (bfs(V, s, t)) {
            unit bottle = INF;
            for (int v = t; v != s; v = e[to[v] ^ 1].to) bottle = min(bottle, e[to[v]].cap);
            for (int v = t; v != s; v = e[to[v] ^ 1].to) { e[to[v]].cap -= bottle; e[to[v] ^ 1].cap += bottle; }
            maxFlow += bottle;
        }
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
