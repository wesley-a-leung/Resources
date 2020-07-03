#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/heaps/SkewHeapIncremental.h"
#include "../../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Computes the minimum arborescence, or directed minimum spanning tree using Gabow's variant of Edmonds' algorithm
// The directed minimum spanning tree is a set of edges such that every vertex is reachable from the root,
// and the sum of the edge weights is minimized
// Time Complexity: O(E log V)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct GabowMinArborescence {
    unit INF; GabowMinArborescence(unit INF) : INF(INF) {}
    struct Edge {
        int from, to; unit weight; int ind;
        bool operator > (const Edge &e) const { return weight > e.weight; }
        Edge &operator += (const unit &add) { weight += add; return *this; }
    };
    int vis[MAXV], path[MAXV], in[MAXV], Q[MAXV]; vector<Edge> edges, mst; UnionFindUndo<MAXV> uf; unit weight;
    vector<SkewHeapIncremental<Edge, greater<Edge>, unit>> H;
    void addEdge(int from, int to, unit weight) { edges.push_back({from, to, weight, int(edges.size())}); }
    unit run(int V, int root) {
        weight = 0; fill(vis, vis + V, -1); fill(in, in + V, -1); vis[root] = root;
        vector<tuple<int, int, vector<Edge>>> cycs; uf.init(V);
        for (int v = 0; v < V; v++) H.emplace_back(0); 
        for (auto &&e : edges) H[e.to].push(e);
        for (int s = 0; s < V; s++) {
            int qi = 0;
            for (int v = s; vis[v] == -1;) {
                if (H[v].empty()) return INF;
                Edge e = H[v].top(); weight += e.weight; H[v].increment(-e.weight); H[v].pop();
                Q[qi] = e.ind; path[qi++] = v; vis[v] = s; v = uf.find(e.from);
                if (vis[v] == s) {
                    SkewHeapIncremental<Edge, greater<Edge>, unit> temp(0); int w, t = int(uf.history.size()); vector<Edge> E;
                    do { temp.merge(H[w = path[--qi]]); E.push_back(edges[Q[qi]]); } while (uf.join(v, w));
                    H[v = uf.find(v)] = move(temp); vis[v] = -1;
                    reverse(E.begin(), E.end()); cycs.emplace_back(v, t, E);
                }
            }
            for (int i = 0; i < qi; i++) in[uf.find(edges[Q[i]].to)] = Q[i];
        }
        reverse(cycs.begin(), cycs.end());
        for (auto &&cyc : cycs) {
            int v, t; vector<Edge> E; tie(v, t, E) = cyc;
            while (int(uf.history.size()) > t) uf.undo();
            Edge inEdge = edges[in[v]];
            for (auto &&e : E) in[uf.find(e.to)] = e.ind;
            in[uf.find(inEdge.to)] = inEdge.ind;
        }
        for (int v = 0; v < V; v++) if (in[v] != -1) mst.push_back(edges[in[v]]);
        return weight;
    }
    void clear() { edges.clear(); mst.clear(); H.clear(); }
};
