#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the minimum cut for a weighted graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edges is an edge with endpoints in both subsets
// The cost of a cut is the sum of the weights of the crossing edges
// Time Complexity: O(V^3)
// Memory Complexity: O(V^2)
template <const int MAXV, class unit> struct StoerWagnerGlobalMinCut {
    unit adj[MAXV][MAXV], W[MAXV], weight, INF; bool in[MAXV], vis[MAXV], cut[MAXV];
    StoerWagnerGlobalMinCut(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) { adj[v][w] += weight; adj[w][v] += weight; }
    unit globalMinCut(int V, int root = 0) {
        weight = INF; fill(in, in + V, false);
        for (int phase = V - 1; phase > 0; phase--) {
            for (int v = 0; v < V; v++) { vis[v] = in[v]; W[v] = adj[root][v]; }
            for (int i = 0, v, last = root; i < phase; i++, last = v) {
                v = -1;
                for (int w = 0; w < V; w++) if (w != root && !vis[w] && (v == -1 || W[v] < W[w])) v = w;
                unit wv = W[v]; vis[v] = true;
                if (i == phase - 1) {
                    for (int w = 0; w < V; w++) { adj[last][w] += adj[v][w]; adj[w][last] += adj[v][w]; }
                    in[v] = true;
                    if (weight > wv) { weight = wv; copy(in, in + V, cut); }
                } else {
                    for (int w = 0; w < V; w++) W[w] += adj[v][w];
                }
            }
        }
        return weight;
    }
    void init(int V) { for (int v = 0; v < V; v++) fill(adj[v], adj[v] + V, 0); }
};

// Computes the minimum cut for a weighted sparse graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edges is an edge with endpoints in both subsets
// The cost of a cut is the sum of the weights of the crossing edges
// Time Complexity: O(V (V + E) log V)
// Memory Complexity: O(V^2 + E)
template <const int MAXV, class unit> struct StoerWagnerGlobalMinCutSparse {
    struct Edge {
        int to; unit weight; int rev; Edge(int to, unit weight, int rev) : to(to), weight(weight), rev(rev) {}
    };
    vector<Edge> adj[MAXV]; unit W[MAXV], weight, INF; bool in[MAXV], cut[MAXV];
    using heap = __gnu_pbds::priority_queue<pair<unit, int>, less<pair<unit, int>>, pairing_heap_tag>;
    typename heap::point_iterator ptr[MAXV]; StoerWagnerGlobalMinCutSparse(unit INF) : INF(INF) {}
    void addEdge(int v, int w, unit weight) {
        if (v == w) return;
        adj[v].emplace_back(w, weight, int(adj[w].size())); adj[w].emplace_back(v, weight, int(adj[v].size()) - 1);
    }
    unit globalMinCut(int V, int root = 0) {
        weight = INF; fill(in, in + V, false); fill(cut, cut + V, false);
        for (int phase = V - 1; phase > 0; phase--) {
            heap PQ; fill(ptr, ptr + V, PQ.end());
            for (int v = 0; v < V; v++) if (v != root && !in[v]) ptr[v] = PQ.push(make_pair(W[v] = 0, v));
            for (auto &&e : adj[root]) if (!in[e.to]) PQ.modify(ptr[e.to], make_pair(W[e.to] += e.weight, e.to));
            for (int i = 0, v, last = root; i < phase; i++, last = v) {
                unit w = PQ.top().first; v = PQ.top().second; PQ.pop(); ptr[v] = PQ.end();
                if (i == phase - 1) {
                    fill(W, W + V, 0);
                    for (auto &&e : adj[v]) W[e.to] += e.weight;
                    for (auto &&e : adj[last]) { e.weight += W[e.to]; adj[e.to][e.rev].weight += W[e.to]; W[e.to] = 0; }
                    for (auto &&e : adj[v]) if (W[e.to] != 0) {
                        adj[e.to][e.rev].to = last; adj[e.to][e.rev].rev = int(adj[last].size());
                        adj[last].emplace_back(e.to, e.weight, e.rev);
                    }
                    adj[v].clear(); in[v] = true;
                    if (weight > w) { weight = w; copy(in, in + V, cut); }
                } else {
                    for (auto &&e : adj[v]) if (ptr[e.to] != PQ.end()) PQ.modify(ptr[e.to], make_pair(W[e.to] += e.weight, e.to));
                }
            }
        }
        return weight;
    }
    void init(int V) { for (int v = 0; v < V; v++) adj[v].clear(); }
};
