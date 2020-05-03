#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a feasible flow and computes the minimum and maximum flow in a flow network with demands using the Push Relabel algorithm
// with the highest label selection rule and gap relabelling heuristic
// Time Complexity: O(V^2 sqrt E), faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct PushRelabelMaxFlowDemands {
    struct Edge {
        int to; unit dem, cap, resCap; int rev;
        Edge(int to, unit dem, unit cap, int rev) : to(to), dem(dem), cap(cap), resCap(cap), rev(rev) {}
    };
    unit INF, EPS, ex[MAXV], outDem[MAXV], inDem[MAXV]; PushRelabelMaxFlowDemands(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    int h[MAXV], cnt[MAXV * 2]; vector<int> hs[MAXV * 2]; vector<Edge> adj[MAXV]; typename vector<Edge>::iterator cur[MAXV];
    void addEdge(int v, int w, unit vwDem, unit vwCap, int type = 1) {
        assert(v != w);
        adj[v].emplace_back(w, vwDem, vwCap, int(adj[w].size())); adj[w].emplace_back(v, -vwDem, -vwDem, int(adj[v].size()) - 1);
        if (type == 1) { outDem[v] += vwDem; inDem[w] += vwDem; }
    }
    void init(int V) { for (int i = 0; i < V + 2; i++) { outDem[i] = inDem[i] = 0; adj[i].clear(); } }
    unit getFlow(int V, int s, int t) {
        auto push = [&] (int v, Edge &e, unit df) {
            int w = e.to;
            if (abs(ex[w]) <= EPS && df > EPS) hs[h[w]].push_back(w);
            e.resCap -= df; adj[w][e.rev].resCap += df; ex[v] -= df; ex[w] += df;
        };
        if (s == t) return 0;
        fill(h, h + V, 0); h[s] = V; fill(ex, ex + V, 0); ex[t] = 1; fill(cnt, cnt + V * 2, 0); cnt[0] = V - 1;
        for (int v = 0; v < V; v++) cur[v] = adj[v].begin();
        for (int i = 0; i < V * 2; i++) hs[i].clear();
        for (auto &&e : adj[s]) push(s, e, e.resCap);
        if (!hs[0].empty()) for (int hi = 0; hi >= 0;) {
            int v = hs[hi].back(); hs[hi].pop_back();
            while (ex[v] > EPS) {
                if (cur[v] == adj[v].end()) {
                    h[v] = INT_MAX;
                    for (auto e = adj[v].begin(); e != adj[v].end(); e++)
                        if (e->resCap > EPS && h[v] > h[e->to] + 1) { h[v] = h[e->to] + 1; cur[v] = e; }
                    cnt[h[v]]++;
                    if (--cnt[hi] == 0 && hi < V) for (int i = 0; i < V; i++) if (hi < h[i] && h[i] < V) { cnt[h[i]]--; h[i] = V + 1; }
                    hi = h[v];
                } else if (cur[v]->resCap > EPS && h[v] == h[cur[v]->to] + 1) push(v, *cur[v], min(ex[v], cur[v]->resCap));
                else cur[v]++;
            }
            while (hi >= 0 && hs[hi].empty()) hi--;
        }
        return -ex[s];
    }
    pair<bool, unit> getFeasibleFlow(int V, int s = -1, int t = -1, int flowType = 0) {
        int ss = V, tt = V + 1; unit bnd = INF, sm = 0; pair<bool, unit> ret(true, flowType < 0 ? INF : 0);
        for (int v = 0; v < V; v++) {
            for (auto &&e : adj[v]) e.cap -= e.dem;
            addEdge(ss, v, 0, inDem[v], 2); addEdge(v, tt, 0, outDem[v], 2); sm += inDem[v];
        }
        for (int h = 0; h < 2; h++) {
            for (int v = 0; v < V + 2; v++) for (auto &&e : adj[v]) e.resCap = e.cap;
            if (s != -1 && t != -1) addEdge(t, s, 0, bnd, 2);
            if (sm - (bnd = getFlow(V + 2, ss, tt)) > EPS) ret.first = false;
            if (s != -1 && t != -1) {
                adj[s].pop_back();
                if (ret.first) ret.second = adj[t].back().cap - adj[t].back().resCap;
                adj[t].pop_back();
            }
            if (flowType >= 0 || !ret.first || h > 0) break;
            for (int v = 0; v < V + 2; v++) for (auto &&e : adj[v]) e.resCap = e.cap;
            bnd -= getFlow(V + 2, ss, tt);
        }
        adj[ss].clear(); adj[tt].clear();
        for (int v = 0; v < V; v++) {
            adj[v].pop_back(); adj[v].pop_back();
            for (auto &&e : adj[v]) e.cap += e.dem;
        }
        if (flowType > 0 && ret.first) ret.second += getFlow(V, s, t);
        return ret;
    }
    pair<bool, unit> getMinFlow(int V, int s, int t) { return getFeasibleFlow(V, s, t, -1); }
    pair<bool, unit> getMaxFlow(int V, int s, int t) { return getFeasibleFlow(V, s, t, 1); }
};
