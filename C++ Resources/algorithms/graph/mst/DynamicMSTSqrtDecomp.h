#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the minimum spanning tree after an edges's weight has been changed
// Offline Sqrt Decomposition Solution
// Time Complexity: O(V + E * (Q / B) + Q * B)
// Memory Complexity: O(V + Q)
template <const int MAXV, const int MAXE, const int MAXQ, const int BLOCKSZ, class unit> struct DynamicMSTSqrtDecomp {
    struct Edge { int v, w; unit weight; };
    int Q = 0, flag[MAXE], ind[MAXQ], curFlag, UF[2][MAXV], stamp[2], vis[2][MAXV]; unit ans[MAXQ]; pair<int, unit> q[MAXQ];
    set<pair<unit, int>> small, large; vector<Edge> edges;
    void prop(int i, int v) { if (vis[i][v] != stamp[i]) { vis[i][v] = stamp[i]; UF[i][v] = -1; } }
    int find(int i, int v) { prop(i, v); return UF[i][v] < 0 ? v : UF[i][v] = find(i, UF[i][v]); }
    bool join(int i, int v, int w) {
        if ((v = find(i, v)) == (w = find(i, w))) return false;
        if (UF[i][v] > UF[i][w]) swap(v, w);
        UF[i][v] += UF[i][w]; UF[i][w] = v; return true;
    }
    void addEdge(int v, int w, unit weight) { edges.push_back({v, w, weight}); }
    void query(int i, unit weight) { q[Q++] = {i, weight}; }
    void solve(int V) {
        int E = edges.size(); fill(flag, flag + E, -1); curFlag = 0; unit forest, mst;
        for (int i = 0; i < E; i++) large.emplace(edges[i].weight, i);
        for (int i = 0; i < 2; i++) fill(UF[i], UF[i] + V, -1);
        for (int l = 0; l < Q; l += BLOCKSZ) {
            int r = min(l + BLOCKSZ - 1, Q - 1), cnt = 0; curFlag++; stamp[0]++; small.clear();
            for (int i = l; i <= r; i++) {
                flag[q[i].first] = curFlag; join(0, edges[q[i].first].v, edges[q[i].first].w);
                small.emplace(edges[q[i].first].weight, q[i].first); 
            }
            stamp[1]++;
            for (auto &&p : large) if (flag[p.second] != curFlag && join(1, edges[p.second].v, edges[p.second].w)) ind[cnt++] = p.second;
            stamp[1]++; forest = 0;
            for (int i = 0; i < cnt; i++) {
                if (join(0, edges[ind[i]].v, edges[ind[i]].w)) {
                    join(1, edges[ind[i]].v, edges[ind[i]].w); forest += edges[ind[i]].weight;
                } else small.emplace(edges[ind[i]].weight, ind[i]);
            }
            for (int i = l; i <= r; i++) {
                large.erase({edges[q[i].first].weight, q[i].first}); small.erase({edges[q[i].first].weight, q[i].first});
                edges[q[i].first].weight = q[i].second;
                large.emplace(edges[q[i].first].weight, q[i].first); small.emplace(edges[q[i].first].weight, q[i].first);
                stamp[0]++; mst = 0;
                for (auto &&p : small) if (join(0, find(1, edges[p.second].v), find(1, edges[p.second].w))) mst += edges[p.second].weight;
                ans[i] = forest + mst;
            }
        }
    }
    void clear() { small.clear(); large.clear(); edges.clear(); Q = 0; }
};
