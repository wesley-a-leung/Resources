#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow in a flow network using the Push Relabel algorithm
// with the highest label selection rule, gap relabelling heuristic, and a single global relabelling
// This implementation only returns the maximum flow, and does not necessarily have the actual flow
// values on the edges
// Time Complexity: O(V^2 sqrt E), faster in practice
// Memory Complexity: O(V + E)
template <const int MAXV, class unit> struct PushRelabelMaxFlowGlobal {
    struct Edge {
        int to; unit cap; int rev;
        Edge(int to, unit cap, int rev) : to(to), cap(cap), rev(rev) {}
    };
    struct LinkedList {
        struct Data {
            int val, pre, nxt;
            Data(int val, int pre, int nxt) : val(val), pre(pre), nxt(nxt) {}
        };
        vector<Data> A; int sz; LinkedList() : sz(0) {}
        int add(int v) {
            if (!A.empty()) A.back().nxt = int(A.size());
            A.emplace_back(v, int(A.size()) - 1, -1); sz++; return int(A.size()) - 1;
        }
        void rem(int i) {
            int j = A[i].pre, k = A[i].nxt; A[i].val = -1; sz--;
            if (j != -1) A[j].nxt = k;
            if (k != -1) A[k].pre = j;
            while (!A.empty() && A.back().val == -1) A.pop_back();
        }
        void clear() { A.clear(); sz = 0; }
        int size() { return sz; }
        int head() { return int(A.size()) - 1; }
        int next(int i) { return A[i].pre; }
        int get(int i) { return A[i].val; }
    };
    unit INF = (numeric_limits<unit>::max)(), EPS, maxFlow, ex[MAXV]; PushRelabelMaxFlowGlobal(unit EPS) : EPS(EPS) {}
    int h[MAXV], cnt[MAXV], q[MAXV], pos[MAXV], highest, highestActive;
    vector<Edge> adj[MAXV]; vector<int> vec[MAXV]; LinkedList lis[MAXV];
    void addEdge(int v, int w, unit vw, unit wv = 0) {
        if (v == w) return;
        adj[v].emplace_back(w, vw, int(adj[w].size())); adj[w].emplace_back(v, wv, int(adj[v].size()) - 1);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void globalRelabel(int V, int t) {
        fill(h, h + V, V); h[t] = 0; fill(cnt, cnt + V + 1, 0); int front = 0, back = 0; q[back++] = t;
        while (front < back) {
            int v = q[front++];
            for (auto &&e : adj[v]) if (h[e.to] == V && adj[e.to][e.rev].cap > EPS) cnt[h[q[back++] = e.to] = h[v] + 1]++;
        }
        for (int i = 0; i <= V; i++) { vec[i].clear(); lis[i].clear(); }
        for (int v = 0; v < V; v++) if (h[v] < V) {
            pos[v] = lis[h[v]].add(v);
            if (ex[v] > EPS) vec[h[v]].push_back(v);
        }
        highest = highestActive = h[q[back - 1]];
    }
    void push(int v, Edge &e) {
        int w = e.to; unit df = min(ex[v], e.cap); e.cap -= df; adj[w][e.rev].cap += df; ex[v] -= df; ex[w] += df;
        if (EPS < ex[w] && ex[w] <= df + EPS) vec[h[w]].push_back(w);
    }
    void discharge(int V, int v) {
        int nh = V;
        for (auto &&e : adj[v]) if (e.cap > EPS) {
            if (h[v] == h[e.to] + 1) {
                push(v, e);
                if (ex[v] <= EPS) return;
            } else nh = min(nh, h[e.to] + 1);
        }
        int hv = h[v];
        if (cnt[hv] == 1) {
            for (int i = hv; i <= highest; ++i) {
                for (int j = lis[i].head(); j != -1; j = lis[i].next(j)) h[lis[i].get(j)] = V;
                cnt[i] -= lis[i].size(); lis[i].clear();
            }
            highest = hv - 1;
        } else {
            cnt[hv]--; lis[hv].rem(pos[v]); h[v] = nh;
            if (nh == V) return;
            cnt[nh]++; pos[v] = lis[nh].add(v); highest = max(highest, highestActive = nh); vec[nh].push_back(v);
        }
    }
    unit getFlow(int V, int s, int t) {
        if (s == t) return maxFlow = 0;
        highest = highestActive = 0; fill(h, h + V, 0); h[s] = V;
        for (int v = 0; v < V; v++) if (v != s) pos[v] = lis[h[v]].add(v);
        fill(cnt, cnt + V + 1, 0); cnt[0] = V - 1; fill(ex, ex + V, 0); ex[s] = INF; ex[t] = -INF;
        for (auto &&e : adj[s]) push(s, e);
        for (globalRelabel(V, t); highestActive >= 0;) {
            if (vec[highestActive].empty()) { highestActive--; continue; }
            int v = vec[highestActive].back(); vec[highestActive].pop_back(); discharge(V, v);
        }
        return ex[t] + INF;
    }
};
