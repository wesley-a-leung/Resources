#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the minimum cut for a weighted graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edges is an edge with endpoints in both subsets
// The cost of a cut is the sum of the weights of the crossing edges
// Time Complexity: O(V (V + E) log V)
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct StoerWagnerGlobalMinCut {
    struct Edge {
        int to; unit weight; int rev;
        Edge(int to, unit weight, int rev) : to(to), weight(weight), rev(rev) {}
    };
    vector<Edge> adj[MAXV]; unit add[MAXV], dist[MAXV], weight, INF; bool vis[MAXV], cut[MAXV];
    using heap = __gnu_pbds::priority_queue<pair<unit, int>, less<pair<unit, int>>, pairing_heap_tag>;
    typename heap::point_iterator ptr[MAXV]; StoerWagnerGlobalMinCut(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) {
        if (v == w) return;
        adj[v].emplace_back(w, weight, int(adj[w].size())); adj[w].emplace_back(v, weight, int(adj[v].size()) - 1);
    }
    unit globalMinCut(int V, int root = 0) {
        weight = INF; fill(vis, vis + V, false); fill(cut, cut + V, false); vis[root] = true;
        for (int phase = V - 1; phase > 0; phase--) {
            heap PQ; fill(ptr, ptr + V, PQ.end());
            for (int v = 0; v < V; v++) if (!vis[v]) ptr[v] = PQ.push(make_pair(dist[v] = 0, v));
            for (auto &&e : adj[root]) if (!vis[e.to]) PQ.modify(ptr[e.to], make_pair(dist[e.to] += e.weight, e.to));
            for (int i = 0, v, last = root; i < phase; i++, last = v) {
                v = PQ.top().second; PQ.pop(); ptr[v] = PQ.end();
                if (i == phase - 1) {
                    fill(add, add + V, 0);
                    for (auto &&e : adj[v]) add[e.to] += e.weight;
                    for (auto &&e : adj[last]) { e.weight += add[e.to]; adj[e.to][e.rev].weight += add[e.to]; add[e.to] = 0; }
                    for (auto &&e : adj[v]) if (add[e.to] != 0) {
                        adj[e.to][e.rev].to = last; adj[e.to][e.rev].rev = int(adj[last].size());
                        adj[last].emplace_back(e.to, e.weight, e.rev);
                    }
                    adj[v].clear(); vis[v] = true;
                    if (weight > dist[v]) { weight = dist[v]; copy(vis, vis + V, cut); }
                } else {
                    for (auto &&e : adj[v]) if (ptr[e.to] != PQ.end()) PQ.modify(ptr[e.to], make_pair(dist[e.to] += e.weight, e.to));
                }
            }
        }
        return weight;
    }
    void init(int V) { for (int v = 0; v < V; v++) adj[v].clear(); }
};
