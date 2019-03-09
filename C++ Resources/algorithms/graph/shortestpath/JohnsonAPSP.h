#pragma once
#include <bits/stdc++.h>
using namespace std;

// Johnson's all pairs shortest path algorithm for weighted graphs with negative weights
// Can also detect negative cycles
// Time Complexity:
//   O(VE log E) if a regular priority queue is used
//   O(VE log V) if an indexed priority queue or pairing heap is used
//   O(VE log (sum of weights)) if all weights are integers and a radix heap is used
// Memory Complexity: O(V^2 + E)
template <const int MAXV, class unit> struct JohnsonAPSP {
    unit INF, dist[MAXV][MAXV], h[MAXV]; int id[MAXV], low[MAXV], pre;
    stack<int> s; vector<vector<int>> components; bitset<MAXV> dp[MAXV], neg[MAXV];
    bool hasNegativeCycle = false, hasNegativeWeight = false, vis[MAXV], G[MAXV][MAXV], compInNegCyc[MAXV], inNegCyc[MAXV];
    pair<int, unit> to[MAXV][MAXV]; vector<pair<int, unit>> adj[MAXV]; vector<int> DAG[MAXV]; JohnsonAPSP(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); hasNegativeWeight |= weight < 0; }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { hasNegativeCycle = hasNegativeWeight = false; for (int i = 0; i < V; i++) adj[i].clear(); }
    void dfs(int v) {
        vis[v] = true; int mn = low[v] = pre++; s.push(v);
        for (auto &&e: adj[v]) {
            if (!vis[e.first]) dfs(e.first);
            if (low[e.first] < mn) mn = low[e.first];
        }
        if (mn < low[v]) { low[v] = mn; return; }
        int w; components.emplace_back();
        do { w = s.top(); s.pop(); id[w] = components.size() - 1; components.back().push_back(w); low[w] = INT_MAX; } while (w != v);
    }
    void markNegativeCycles(int V) {
        fill(vis, vis + V, false); pre = 0; components.clear(); fill(h, h + V, 0); fill(compInNegCyc, compInNegCyc + V, false);
        for (int v = 0; v < V; v++) if (!vis[v]) dfs(v);
        for (int i = 0; i < V - 1; i++) for (int v = 0; v < V; v++) for (auto &&e : adj[v])
            if (id[v] == id[e.first] && h[e.first] > h[v] + e.second) h[e.first] = h[v] + e.second;
        for (int v = 0; v < V; v++) for (auto &&e : adj[v]) if (id[v] == id[e.first] && h[e.first] > h[v] + e.second) compInNegCyc[id[v]] = true;
        for (int v = 0; v < V; v++) hasNegativeCycle |= (inNegCyc[v] = compInNegCyc[id[v]]);
    }
    void tcDfs(int v) {
        if (vis[v]) return;
        vis[v] = true;
        for (int w : components[v]) dp[v][w] = 1;
        for (int w : DAG[v]) { tcDfs(w); dp[v] |= dp[w]; neg[v] |= neg[w]; }
        if (compInNegCyc[v]) neg[v] = dp[v];
    }
    void run(int V) {
        for (int v = 0; v < V; v++) { fill(dist[v], dist[v] + V, INF); fill(to[v], to[v] + V, make_pair(-1, 0)); }
        if (hasNegativeWeight) {
            markNegativeCycles(V); fill(h, h + V, 0);
            for (int i = 0; i < V - 1; i++) for (int v = 0; v < V; v++) for (auto &&e : adj[v])
                if (!inNegCyc[v] && !inNegCyc[e.first] && h[e.first] > h[v] + e.second) h[e.first] = h[v] + e.second;
            for (int v = 0; v < V; v++) for (auto &&e : adj[v]) if (id[v] != id[e.first]) G[id[v]][id[e.first]] = true;
            for (int i = 0; i < int(components.size()); i++) {
                dp[i].reset(); neg[i].reset(); vis[i] = false;
                for (int j = 0; j < int(components.size()); j++) if (G[i][j]) DAG[i].push_back(j);
            }
            for (int i = 0; i < int(components.size()); i++) tcDfs(i);
        } else fill(h, h + V, 0);
        for (int s = 0; s < V; s++) {
            if (!inNegCyc[s]) {
                std::priority_queue<pair<unit, int>, vector<pair<unit, int>>, greater<pair<unit, int>>> PQ; PQ.emplace(dist[s][s] = 0, s);
                while (!PQ.empty()) {
                    unit d = PQ.top().first; int v = PQ.top().second; PQ.pop();
                    if (d > dist[s][v]) continue;
                    for (auto &&e : adj[v]) if (!inNegCyc[e.first] && dist[s][e.first] > dist[s][v] + e.second + h[v] - h[e.first]) {
                        to[s][e.first] = make_pair(v, e.second); PQ.emplace(dist[s][e.first] = dist[s][v] + e.second + h[v] - h[e.first], e.first);
                    }
                }
            }
            if (hasNegativeWeight) for (int v = 0; v < V; v++) if (neg[id[s]][v]) dist[s][v] = -INF;
        }
        if (hasNegativeWeight) for (int v = 0; v < V; v++) for (int w = 0; w < V; w++)
            if (abs(dist[v][w]) < INF) dist[v][w] = dist[v][w] - h[v] + h[w];
    }
};
