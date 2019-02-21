#pragma once
#include <bits/stdc++.h>
#include "../../../datastructures/IncrementalSkewHeap.h"
using namespace std;

// Computes the minimum arborescence, or directed minimum spanning tree using Gabow's variant of Edmonds' algorithm
// Time Complexity: O(E log V)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct GabowMinArborescence {
    unit INF; GabowMinArborescence(unit INF) : INF(INF) {}
    struct Edge {
        int from, to; unit weight;
        bool operator > (const Edge &e) const { return weight > e.weight; }
        Edge operator + (const unit &add) const { Edge ret; ret.from = from; ret.to = to; ret.weight = weight + add; return ret; }
    };
    int UF[MAXV], vis[MAXV]; vector<Edge> edges, mst; unit weight; vector<IncrementalSkewHeap<Edge, greater<Edge>, unit>> H;
    void addEdge(int from, int to, unit weight) { edges.push_back({from, to, weight}); }
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    bool join(int v, int w) {
        if ((v = find(v)) == (w = find(w))) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v; return true;
    }
    unit run(int V, int root) {
        weight = 0; fill(UF, UF + V, -1); fill(vis, vis + V, -1); vis[root] = root;
        for (int v = 0; v < V; v++) H.emplace_back(0); 
        for (auto &&e : edges) H[e.to].push(e);
        for (int s = 0; s < V; s++) {
            stack<int> path;
            for (int v = s; vis[v] == -1;) {
                path.push(v); vis[v] = s;
                if (H[v].empty()) return INF;
                Edge minEdge = H[v].top(); int w = find(minEdge.from);
                weight += minEdge.weight; H[v].increment(-minEdge.weight); H[v].pop();
                if (vis[w] == s) {
                    IncrementalSkewHeap<Edge, greater<Edge>, unit> temp(0); int x;
                    do { x = path.top(); path.pop(); temp.merge(H[x]); } while (join(w, x));
                    H[find(w)] = move(temp); vis[find(w)] = -1;
                }
                v = find(w);
            }
        }
        return weight;
    }
    void clear() { edges.clear(); mst.clear(); H.clear(); }
};
