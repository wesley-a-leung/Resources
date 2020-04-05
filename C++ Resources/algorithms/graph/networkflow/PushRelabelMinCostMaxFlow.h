#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow using a path with the minimum cost using the Push Relabel algorithm with look ahead heuristics
// Time Complexity: O(E V^2 log (V * C)), much faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class flowUnit, class costUnit, const int SCALE = 3> struct PushRelabelMinCostMaxFlow {
    static_assert(is_integral<costUnit>::value, "costUnit must be an integral type");
    struct Edge {
        int to; flowUnit cap, resCap; costUnit cost; int rev;
        Edge(int to, flowUnit cap, costUnit cost, int rev) : to(to), cap(cap), resCap(cap), cost(cost), rev(rev) {}
    };
    int cnt[MAXV * 2], stk[MAXV], top; flowUnit FLOW_EPS, maxFlow, ex[MAXV]; costUnit COST_INF, h[MAXV], bnd, minCost;
    vector<int> hs[MAXV * 2]; vector<Edge> adj[MAXV]; typename vector<Edge>::iterator cur[MAXV];
    PushRelabelMinCostMaxFlow(flowUnit FLOW_EPS, costUnit COST_INF) : FLOW_EPS(FLOW_EPS), COST_INF(COST_INF) {}
    void addEdge(int v, int w, flowUnit flow, costUnit cost) {
        adj[v].emplace_back(w, flow, cost, int(adj[w].size())); adj[w].emplace_back(v, 0, -cost, int(adj[v].size()) - 1);
    }
    void init(int V) { for (int i = 0; i < V; i++) adj[i].clear(); }
    flowUnit getFlow(int V, int s, int t) {
        auto push = [&] (int v, Edge &e, flowUnit df) {
            int w = e.to;
            if (abs(ex[w]) <= FLOW_EPS && df > FLOW_EPS) hs[h[w]].push_back(w);
            e.resCap -= df; adj[w][e.rev].resCap += df; ex[v] -= df; ex[w] += df;
        };
        if (s == t) return maxFlow = 0;
        fill(h, h + V, 0); h[s] = V; fill(ex, ex + V, 0); ex[t] = 1; fill(cnt, cnt + V * 2, 0); cnt[0] = V - 1;
        for (int v = 0; v < V; v++) cur[v] = adj[v].begin();
        for (int i = 0; i < V * 2; i++) hs[i].clear();
        for (auto &&e : adj[s]) push(s, e, e.resCap);
        if (!hs[0].empty()) for (int hi = 0; hi >= 0;) {
            int v = hs[hi].back(); hs[hi].pop_back();
            while (ex[v] > FLOW_EPS) {
                if (cur[v] == adj[v].end()) {
                    h[v] = INT_MAX;
                    for (auto e = adj[v].begin(); e != adj[v].end(); e++)
                        if (e->resCap > FLOW_EPS && h[v] > h[e->to] + 1) { h[v] = h[e->to] + 1; cur[v] = e; }
                    cnt[h[v]]++;
                    if (--cnt[hi] == 0 && hi < V) for (int i = 0; i < V; i++) if (hi < h[i] && h[i] < V) { cnt[h[i]]--; h[i] = V + 1; }
                    hi = h[v];
                } else if (cur[v]->resCap > FLOW_EPS && h[v] == h[cur[v]->to] + 1) push(v, *cur[v], min(ex[v], cur[v]->resCap));
                else cur[v]++;
            }
            while (hi >= 0 && hs[hi].empty()) hi--;
        }
        return maxFlow = -ex[s];
    }
    pair<flowUnit, costUnit> getMaxFlowMinCost(int V, int s = -1, int t = -1) {
        auto costH = [&] (int v, const Edge &e) { return e.cost + h[v] - h[e.to]; };
        auto push = [&] (int v, Edge &e, flowUnit df, bool pushToStack) {
            if (e.resCap < df) df = e.resCap;
            int w = e.to; e.resCap -= df; adj[w][e.rev].resCap += df; ex[v] -= df; ex[w] += df;
            if (pushToStack && FLOW_EPS < ex[e.to] && ex[e.to] <= df + FLOW_EPS) stk[top++] = e.to;
        };
        auto relabel = [&] (int v, costUnit delta) { h[v] -= delta + bnd; };
        auto lookAhead = [&] (int v) {
            if (abs(ex[v]) > FLOW_EPS) return false;
            costUnit delta = COST_INF;
            for (auto &&e : adj[v]) {
                if (e.resCap <= FLOW_EPS) continue;
                costUnit c = costH(v, e);
                if (c < 0) return false;
                else delta = min(delta, c);
            }
            relabel(v, delta); return true;
        };
        auto discharge = [&] (int v) {
            costUnit delta = COST_INF;
            for (int i = 0; i < int(adj[v].size()); i++) {
                Edge &e = adj[v][i];
                if (e.resCap <= FLOW_EPS) continue;
                if (costH(v, e) < 0) {
                    if (lookAhead(e.to)) { i--; continue; }
                    push(v, e, ex[v], true);
                    if (abs(ex[v]) <= FLOW_EPS) return;
                } else delta = min(delta, costH(v, e));
            }
            relabel(v, delta); stk[top++] = v;
        };
        minCost = 0; bnd = 0; costUnit mul = 2 << __lg(V);
        for (int v = 0; v < V; v++) for (auto &&e : adj[v]) { minCost += e.cost * e.resCap; e.cost *= mul; bnd = max(bnd, e.cost); }
        maxFlow = (s == -1 || t == -1) ? 0 : getFlow(V, s, t); fill(h, h + V, 0); fill(ex, ex + V, 0);
        while (bnd > 1) {
            bnd = max(costUnit(1), bnd >> SCALE); top = 0;
            for (int v = 0; v < V; v++) for (auto &&e: adj[v]) if (costH(v, e) < 0 && e.resCap > FLOW_EPS) push(v, e, e.resCap, false);
            for (int v = 0; v < V; v++) if (ex[v] > FLOW_EPS) stk[top++] = v;
            while (top > 0) discharge(stk[--top]);
        }
        for (int v = 0; v < V; v++) for (auto &&e: adj[v]) { e.cost /= mul; minCost -= e.cost * e.resCap; }
        return make_pair(maxFlow, minCost /= 2);
    }
};
