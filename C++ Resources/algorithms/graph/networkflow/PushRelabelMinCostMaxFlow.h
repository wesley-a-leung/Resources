#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow using a path with the minimum cost using the Push Relabel algorithm with look ahead heuristics
// Time Complexity: O(E V^2 log (V * C)), much faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class flowUnit, class costUnit, const bool USE_LOOK_AHEAD, const int SCALE = 2> struct PushRelabelMinCostMaxFlow {
    static_assert(is_integral<costUnit>::value, "costUnit must be an integral type");
    struct Edge {
        int to; flowUnit cap, resCap; costUnit cost; int rev;
        Edge(int to, flowUnit cap, costUnit cost, int rev) : to(to), cap(cap), resCap(cap), cost(cost), rev(rev) {}
    };
    int V, cnt[MAXV * 2], stk[MAXV]; flowUnit FLOW_EPS, maxFlow, ex[MAXV]; costUnit COST_INF, h[MAXV], bnd, minCost;
    bool inStk[MAXV]; vector<int> hs[MAXV * 2]; vector<Edge> adj[MAXV]; typename vector<Edge>::iterator cur[MAXV];
    PushRelabelMinCostMaxFlow(flowUnit FLOW_EPS, costUnit COST_INF) : FLOW_EPS(FLOW_EPS), COST_INF(COST_INF) {}
    void addEdge(int v, int w, flowUnit flow, costUnit cost) {
        if (v == w) return;
        adj[v].emplace_back(w, flow, cost, int(adj[w].size())); adj[w].emplace_back(v, 0, -cost, int(adj[v].size()) - 1);
    }
    void init(int V) { this->V = V; bnd = 0; for (int i = 0; i < V; i++) adj[i].clear(); }
    flowUnit getFlow(int s, int t) {
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
    pair<flowUnit, costUnit> getMaxFlowMinCost(int s, int t, bool circulation) {
        auto push = [&] (int v, Edge &e, flowUnit df) {
            int w = e.to;
            if (e.resCap < df) df = e.resCap;
            e.resCap -= df; adj[w][e.rev].resCap += df; ex[v] -= df; ex[w] += df;
        };
        auto relabel = [&] (int v) {
            costUnit newHeight = -COST_INF;
            for (auto e = adj[v].begin(); e != adj[v].end(); e++) if (e->resCap > FLOW_EPS && newHeight < h[e->to] - e->cost) {
                newHeight = h[e->to] - e->cost; cur[v] = e;
            }
            h[v] = newHeight - bnd;
        };
        auto lookAhead = [&] (int v) {
            if (abs(ex[v]) > FLOW_EPS) return false;
            costUnit newHeight = h[v] - V * bnd;
            for (auto &&e : adj[v]) {
                if (e.resCap <= FLOW_EPS) continue;
                if (h[v] + e.cost - h[e.to] < 0) return false;
                else if (newHeight < h[e.to] - e.cost) newHeight = h[e.to] - e.cost;
            }
            h[v] = newHeight - bnd;
            return true;
        };
        minCost = 0;
        for (int v = 0; v < V; v++) for (auto &&e : adj[v]) { e.cost *= V; bnd = max(bnd, e.cost); minCost += e.cost * e.resCap; }
        maxFlow = circulation ? 0 : getFlow(s, t);
        fill(h, h + V, 0); fill(ex, ex + V, 0); fill(inStk, inStk + V, false);
        for (; bnd > 0; bnd >>= SCALE) {
            for (int v = 0; v < V; v++) cur[v] = adj[v].begin();
            for (int v = 0; v < V; v++) for (auto &&e: adj[v]) if (h[v] + e.cost - h[e.to] < 0 && e.resCap > FLOW_EPS) push(v, e, e.resCap);
            int top = 0;
            for (int v = 0; v < V; v++) if (ex[v] > FLOW_EPS) inStk[stk[top++] = v] = true;
            while (top > 0) {
                int v = stk[--top]; inStk[v] = false;
                while (ex[v] > FLOW_EPS) {
                    if (cur[v] == adj[v].end()) relabel(v);
                    for (; cur[v] < adj[v].end(); cur[v]++) {
                        if (cur[v]->resCap <= FLOW_EPS) continue;
                        if (h[v] + cur[v]->cost - h[cur[v]->to] < 0) {
                            if (USE_LOOK_AHEAD && lookAhead(cur[v]->to)) { cur[v]--; continue; }
                            push(v, *cur[v], ex[v]);
                            if (!inStk[cur[v]->to]) inStk[stk[top++] = cur[v]->to] = true;
                            if (abs(ex[v]) <= FLOW_EPS) break;
                        }
                    }
                }
            }
            if (bnd > 1 && (bnd >> SCALE) == 0) bnd = 1 << SCALE;
        }
        for (int v = 0; v < V; v++) for (auto &&e: adj[v]) { minCost -= e.cost * e.resCap; e.cost /= V; }
        minCost /= V * 2;
        return {maxFlow, minCost};
    }
};
