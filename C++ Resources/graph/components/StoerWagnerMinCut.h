#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the minimum cut for a weighted graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edges is an edge with endpoints in both subsets
// The cost of a cut is the sum of the weights of the crossing edges
// Time Complexity: O(V (V + E) log E)
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE, class unit> struct StoerWagnerGlobalMinCut {
    int A[MAXE], B[MAXE], par[MAXV], E = 0, s, t; unit C[MAXE], dist[MAXV], weight, INF; bool vis[MAXV], cut[MAXV];
    using heap = __gnu_pbds::priority_queue<pair<unit, int>, less<pair<unit, int>>, pairing_heap_tag>;
    typename heap::point_iterator ptr[MAXV]; vector<pair<int, unit>> adj[MAXV]; StoerWagnerGlobalMinCut(unit INF) : E(0), INF(INF) {}
    void addEdge(int v, int w, unit weight) { A[E] = v; B[E] = w; C[E++] = weight; }
    int find(int v) { return par[v] == v ? v : par[v] = find(par[v]); }
    void makeCut(int V, int t) { for (int v = 0; v < V; v++) cut[v] = find(v) == find(t); }
    void minCutPhase(int V) {
        heap PQ; fill(ptr, ptr + V, PQ.end()); ptr[s] = PQ.push({dist[s] = INF, s});
        for (int v = 0; v < V; v++) if (v != s && !vis[v]) ptr[v] = PQ.push({dist[v] = 0, v});
        while (!PQ.empty()) {
            int v = PQ.top().second; PQ.pop(); ptr[v] = PQ.end(); s = t; t = v;
            for (auto &&e : adj[v]) if (ptr[e.first] != PQ.end()) PQ.modify(ptr[e.first], {dist[e.first] += e.second, e.first});
        }
        weight = 0;
        for (auto &&e : adj[t]) weight += e.second;
    }
    void build(int V) {
        for (int v = 0; v < V; v++) adj[v].clear();
        for (int e = 0; e < E; e++) if ((A[e] = find(A[e])) != (B[e] = find(B[e]))) {
            adj[A[e]].emplace_back(B[e], C[e]);
            adj[B[e]].emplace_back(A[e], C[e]);
        }
    }
    unit globalMinCut(int V, int root = 0) {
        unit ret = INF; weight = 0; s = t = root; iota(par, par + V, 0); fill(vis, vis + V, false); fill(cut, cut + V, false); 
        for (int v = V; v > 1; v--) {
            build(V); minCutPhase(V);
            if (weight < ret) { ret = weight; makeCut(V, t); }
            vis[t] = true; par[t] = s;
        }
        return ret;
    }
    void clear() { E = 0; }
};
