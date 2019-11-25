#pragma once
#include <bits/stdc++.h>
#include "../../../datastructures/IncrementalSkewHeap.h"
#include "../../../datastructures/UnionFind.h"
using namespace std;

// Computes the minimum arborescence, or directed minimum spanning tree using Gabow's variant of Edmonds' algorithm
// The directed minimum spanning tree is a set of edges such that every vertex is reachable from the root,
// and the sum of the edge weights is minimized
// Time Complexity: O(E log V)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct GabowMinArborescence {
    unit INF; GabowMinArborescence(unit INF) : INF(INF) {}
    struct Edge {
        int from, to; unit weight;
        bool operator > (const Edge &e) const { return weight > e.weight; }
        Edge operator + (const unit &add) const { Edge ret; ret.from = from; ret.to = to; ret.weight = weight + add; return ret; }
    };
    int vis[MAXV]; vector<Edge> edges; UnionFind<MAXV, 0> uf; unit weight; vector<IncrementalSkewHeap<Edge, greater<Edge>, unit>> H;
    void addEdge(int from, int to, unit weight) { edges.push_back({from, to, weight}); }
    unit run(int V, int root) {
        weight = 0; fill(vis, vis + V, -1); vis[root] = root; uf.init(V);
        for (int v = 0; v < V; v++) H.emplace_back(0); 
        for (auto &&e : edges) H[e.to].push(e);
        for (int s = 0; s < V; s++) {
            stack<int> path;
            for (int v = s; vis[v] == -1;) {
                path.push(v); vis[v] = s;
                if (H[v].empty()) return INF;
                Edge minEdge = H[v].top(); int w = uf.find(minEdge.from);
                weight += minEdge.weight; H[v].increment(-minEdge.weight); H[v].pop();
                if (vis[w] == s) {
                    IncrementalSkewHeap<Edge, greater<Edge>, unit> temp(0); int x;
                    do { x = path.top(); path.pop(); temp.merge(H[x]); } while (uf.join(w, x));
                    H[uf.find(w)] = move(temp); vis[uf.find(w)] = -1;
                }
                v = uf.find(w);
            }
        }
        return weight;
    }
    void clear() { edges.clear(); H.clear(); }
};
