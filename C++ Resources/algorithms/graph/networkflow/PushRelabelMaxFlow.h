#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow and minimum cut in a flow network using the Push Relabel algorithm
// with the highest label selection rule and gap relabelling heuristic
// Time Complexity: O(V^2 sqrt E), faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct PushRelabelMaxFlow {
    struct Edge {
        int to; unit origCap, cap; int rev;
        Edge(int to, unit cap, int rev) : to(to), origCap(cap), cap(cap), rev(rev) {}
    };
    unit EPS, maxFlow, minCut, ex[MAXV]; PushRelabelMaxFlow(unit EPS) : EPS(EPS) {}
    int h[MAXV], cnt[MAXV * 2]; bool cut[MAXV]; vector<int> hs[MAXV * 2]; vector<Edge> adj[MAXV]; typename vector<Edge>::iterator cur[MAXV];
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        if (v == w) return;
        adj[v].emplace_back(w, vw, int(adj[w].size())); adj[w].emplace_back(v, wv, int(adj[v].size()) - 1);
    }
    void init(int V = MAXV) { fill(cut, cut + V, false); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void push(int v, Edge &e, unit df) {
        int w = e.to;
        if (abs(ex[w]) <= EPS && df > EPS) hs[h[w]].push_back(w);
        e.cap -= df; adj[w][e.rev].cap += df; ex[v] -= df; ex[w] += df;
    }
    unit getFlow(int V, int s, int t) {
        if (s == t) return maxFlow = 0;
        fill(h, h + V, 0); h[s] = V; fill(ex, ex + V, 0); ex[t] = 1; fill(cnt, cnt + V * 2, 0); cnt[0] = V - 1;
        for (int v = 0; v < V; v++) cur[v] = adj[v].begin();
        for (int i = 0; i < V * 2; i++) hs[i].clear();
        for (auto &&e : adj[s]) push(s, e, e.cap);
        if (!hs[0].empty()) for (int hi = 0; hi >= 0;) {
            int v = hs[hi].back(); hs[hi].pop_back();
            while (ex[v] > EPS) {
                if (cur[v] == adj[v].end()) {
                    h[v] = INT_MAX;
                    for (auto e = adj[v].begin(); e != adj[v].end(); e++)
                        if (e->cap > EPS && h[v] > h[e->to] + 1) { h[v] = h[e->to] + 1; cur[v] = e; }
                    cnt[h[v]]++;
                    if (--cnt[hi] == 0 && hi < V) for (int i = 0; i < V; i++) if (hi < h[i] && h[i] < V) { cnt[h[i]]--; h[i] = V + 1; }
                    hi = h[v];
                } else if (cur[v]->cap > EPS && h[v] == h[cur[v]->to] + 1) push(v, *cur[v], min(ex[v], cur[v]->cap));
                else cur[v]++;
            }
            while (hi >= 0 && hs[hi].empty()) hi--;
        }
        return maxFlow = -ex[s];
    }
    void inferMinCutDfs(int v) {
        cut[v] = true;
        for (auto &&e : adj[v]) if (e.cap > EPS && !cut[e.to]) inferMinCutDfs(e.to);
    }
    unit inferMinCut(int V, int s) {
        inferMinCutDfs(s); minCut = 0;
        for (int v = 0; v < V; v++) if (cut[v]) for (auto &&e : adj[v]) if (!cut[e.to]) minCut += e.origCap;
        return minCut;
    }
};
