#pragma once
#include <bits/stdc++.h>
#include "../../../datastructures/IndexedPQ.h"
#include "../../../datastructures/UnionFind.h"
using namespace std;

// Computes the minimum cut for a weighted graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edges is an edge with endpoints in both subsets
// The cost of a cut is the sum of the weights of the crossing edges
// Time Complexity: O(V (V + E) log V)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct StoerWagnerMinCut {
    struct Graph {
        vector<pair<int, unit>> adj[MAXV];
        void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); adj[w].emplace_back(v, weight); }
        void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    } G;
    bool vis[MAXV], cut[MAXV]; UnionFind<MAXV, 0> uf; unit INF; StoerWagnerMinCut(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { G.addEdge(v, w, weight); }
    struct CutPhase { unit weight; int s, t; };
    void makeCut(int V, int t, UnionFind<MAXV, 0> &uf) { for (int v = 0; v < V; v++) cut[v] = uf.connected(v, t); }
    void minCutPhase(int V, CutPhase &cp) {
        IndexedPQ<unit, less<unit>> pq(V);
        for (int v = 0; v < V; v++) if (v != cp.s && !vis[v]) pq.push(v, 0);
        pq.push(cp.s, INF);
        while (!pq.empty()) {
            int v = pq.top().first; pq.pop(); cp.s = cp.t; cp.t = v;
            for (auto &&e : G.adj[v]) if (pq.containsIndex(e.first)) pq.changeKey(e.first, pq.keyOf(e.first) + e.second);
        }
        cp.weight = 0;
        for (auto &&e : G.adj[cp.t]) cp.weight += e.second;
    }
    Graph contractEdge(int V, int s, int t) {
        Graph H;
        for (int v = 0; v < V; v++) {
            for (auto &&e : G.adj[v]) {
                if ((v == s && e.first == t) || (v == t && e.first == s)) continue;
                if (v < e.first) {
                    if (e.first == t) H.addEdge(v, s, e.second);
                    else if (v == t) H.addEdge(e.first, s, e.second);
                    else H.addEdge(v, e.first, e.second);
                }
            }
        }
        return H;
    }
    unit minCut(int V, int root = 0) {
        unit ret = INF; CutPhase cp = {0, root, root}; uf.init(V); fill(vis, vis + V, false); fill(cut, cut + V, false); 
        for (int v = V; v > 1; v--) {
            minCutPhase(V, cp);
            if (cp.weight < ret) { ret = cp.weight; makeCut(V, cp.t, uf); }
            G = contractEdge(V, cp.s, cp.t); vis[cp.t] = true; uf.join(cp.s, cp.t);
        }
        return ret;
    }
    void clear(int V = MAXV) { G.clear(V); }
};
