#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow in a flow network using the Push Relabel algorithm
// with the highest label selection rule, gap relabelling heuristic, and a single global relabelling
// This implementation only returns the maximum flow, and does not necessarily have the actual flow
// values on the edges
// Time Complexity: O(V^2 sqrt E), faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct PushRelabelMaxFlowGlobal {
    struct Edge {
        int to; unit cap; int rev;
        Edge(int to, unit cap, int rev) : to(to), cap(cap), rev(rev) {}
    };
    unit INF = (numeric_limits<unit>::max)(), EPS, maxFlow, ex[MAXV]; PushRelabelMaxFlowGlobal(unit EPS) : EPS(EPS) {}
    int h[MAXV], cnt[MAXV], q[MAXV], high, relabels; vector<Edge> adj[MAXV]; vector<int> hs[MAXV], gap[MAXV];
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        if (v == w) return;
        adj[v].emplace_back(w, vw, int(adj[w].size())); adj[w].emplace_back(v, wv, int(adj[v].size()) - 1);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void updateHeight(int v, int nh) {
        if (h[v] != MAXV) cnt[h[v]]--;
        if ((h[v] = nh) == MAXV) return;
        hs[nh].push_back(v); gap[nh].push_back(v); cnt[nh]++; high = nh; relabels++;
    }
    void globalRelabel(int V, int t) {
        fill(h, h + V, MAXV); h[t] = 0; fill(cnt, cnt + V, 0); int front = 0, back = 0; q[back++] = t;
        for (int i = 0; i < V; i++) { hs[i].clear(); gap[i].clear(); }
        while (front < back) {
            int v = q[front++];
            for (auto &&e : adj[v]) if (h[e.to] == MAXV && adj[e.to][e.rev].cap > EPS) updateHeight(q[back++] = e.to, h[v] + 1);
        }
        high = h[q[back - 1]]; relabels = 0;
    }
    void push(int v, Edge &e) {
        int w = e.to; unit df = min(ex[v], e.cap); e.cap -= df; adj[w][e.rev].cap += df; ex[v] -= df; ex[w] += df;
        if (EPS < ex[w] && ex[w] <= df + EPS) hs[h[w]].push_back(w);
    }
    void discharge(int V, int v) {
        int nh = MAXV;
        for (auto &&e : adj[v]) if (e.cap > EPS) {
            if (h[v] == h[e.to] + 1) {
                push(v, e);
                if (ex[v] <= EPS) return;
            } else nh = min(nh, h[e.to] + 1);
        }
        if (cnt[h[v]] > 1) updateHeight(v, nh);
        else for (int i = h[v]; i <= high; gap[i++].clear()) for (int j : gap[i]) updateHeight(j, MAXV);
    }
    unit getFlow(int V, int s, int t) {
        if (s == t) return maxFlow = 0;
        fill(ex, ex + V, 0); ex[s] = INF; ex[t] = -INF; globalRelabel(V, t);
        for (auto &&e : adj[s]) push(s, e);
        for (; high >= 0; high --) while (!hs[high].empty()) {
            int v = hs[high].back(); hs[high].pop_back(); discharge(V, v);
            if (relabels >= V * 4) globalRelabel(V, t);
        }
        return ex[t] + INF;
    }
};
