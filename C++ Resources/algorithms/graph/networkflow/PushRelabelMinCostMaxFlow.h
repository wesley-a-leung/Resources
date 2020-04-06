#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow using a path with the minimum cost using the Push Relabel algorithm with look ahead heuristics
// Adds circulations to negative cost cycles
// Time Complexity: O(E V^2 log (V * C)), much faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class flowUnit, class costUnit, const int SCALE = 8> struct PushRelabelMinCostMaxFlow {
    struct Edge {
        int to; flowUnit cap, resCap; costUnit cost; int rev;
        Edge(int to, flowUnit cap, costUnit cost, int rev) : to(to), cap(cap), resCap(cap), cost(cost), rev(rev) {}
    };
    int cnt[MAXV * 2], h[MAXV], stk[MAXV], top; flowUnit FLOW_EPS, maxFlow, ex[MAXV]; costUnit COST_INF, COST_EPS, phi[MAXV], bnd, minCost, negCost;
    vector<int> hs[MAXV * 2]; vector<Edge> adj[MAXV]; typename vector<Edge>::iterator cur[MAXV];
    PushRelabelMinCostMaxFlow(flowUnit FLOW_EPS, costUnit COST_INF, costUnit COST_EPS) : FLOW_EPS(FLOW_EPS), COST_INF(COST_INF), COST_EPS(COST_EPS) {}
    void addEdge(int v, int w, flowUnit flow, costUnit cost) {
        if (v == w) {
            if (cost < 0) negCost += flow * cost;
            return;
        }
        adj[v].emplace_back(w, flow, cost, int(adj[w].size())); adj[w].emplace_back(v, 0, -cost, int(adj[v].size()) - 1);
    }
    void init(int V) { negCost = 0; for (int i = 0; i < V; i++) adj[i].clear(); }
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
    pair<flowUnit, costUnit> getFlowMinCost(int V, int s = -1, int t = -1) {
        auto costP = [&] (int v, const Edge &e) { return e.cost + phi[v] - phi[e.to]; };
        auto push = [&] (int v, Edge &e, flowUnit df, bool pushToStack) {
            if (e.resCap < df) df = e.resCap;
            int w = e.to; e.resCap -= df; adj[w][e.rev].resCap += df; ex[v] -= df; ex[w] += df;
            if (pushToStack && FLOW_EPS < ex[e.to] && ex[e.to] <= df + FLOW_EPS) stk[top++] = e.to;
        };
        auto relabel = [&] (int v, costUnit delta) { phi[v] -= delta + bnd; };
        auto lookAhead = [&] (int v) {
            if (abs(ex[v]) > FLOW_EPS) return false;
            costUnit delta = COST_INF;
            for (auto &&e : adj[v]) {
                if (e.resCap <= FLOW_EPS) continue;
                costUnit c = costP(v, e);
                if (c < -COST_EPS) return false;
                else delta = min(delta, c);
            }
            relabel(v, delta); return true;
        };
        auto discharge = [&] (int v) {
            costUnit delta = COST_INF;
            for (int i = 0; i < int(adj[v].size()); i++) {
                Edge &e = adj[v][i];
                if (e.resCap <= FLOW_EPS) continue;
                if (costP(v, e) < -COST_EPS) {
                    if (lookAhead(e.to)) { i--; continue; }
                    push(v, e, ex[v], true);
                    if (abs(ex[v]) <= FLOW_EPS) return;
                } else delta = min(delta, costP(v, e));
            }
            relabel(v, delta); stk[top++] = v;
        };
        minCost = 0; bnd = 0; costUnit mul = 2 << __lg(V);
        for (int v = 0; v < V; v++) for (auto &&e : adj[v]) { minCost += e.cost * e.resCap; e.cost *= mul; bnd = max(bnd, e.cost); }
        maxFlow = (s == -1 || t == -1) ? 0 : getFlow(V, s, t); fill(phi, phi + V, 0); fill(ex, ex + V, 0);
        while (bnd > 1) {
            bnd = max(bnd / SCALE, costUnit(1)); top = 0;
            for (int v = 0; v < V; v++) for (auto &&e : adj[v]) if (costP(v, e) < -COST_EPS && e.resCap > FLOW_EPS) push(v, e, e.resCap, false);
            for (int v = 0; v < V; v++) if (ex[v] > FLOW_EPS) stk[top++] = v;
            while (top > 0) discharge(stk[--top]);
        }
        for (int v = 0; v < V; v++) for (auto &&e : adj[v]) { e.cost /= mul; minCost -= e.cost * e.resCap; }
        return make_pair(maxFlow, (minCost /= 2) += negCost);
    }
};
