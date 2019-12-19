#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes small vertex covers
// Time Complexity:
//   hasCover: O(2^K KV)
//   mnCover: O(2^K (V + E))
// Memory Complexity: O(V + E)
template <const int MAXV, const int MAXE> struct VertexCover {
    int E, curE, coveredDep[MAXE]; bool inCover[MAXV], temp[MAXV]; pair<int, int> edges[MAXE];
    void init() { E = 0; }
    void addEdge(int v, int w) {
        if (v > w) swap(v, w);
        edges[E++] = make_pair(v, w);
    }
    void commonSetUp(int V) {
        sort(edges, edges + E); curE = E = unique(edges, edges + E) - edges;
        fill(inCover, inCover + V, false); fill(coveredDep, coveredDep + E, -1);
    }
    void cover(int v, int dep) {
        inCover[v] = true;
        for (int e = 0; e < E; e++) if (coveredDep[e] == -1 && (edges[e].first == v || edges[e].second == v)) {
            coveredDep[e] = dep; curE--;
        }
    }
    void uncover(int v, int dep) {
        inCover[v] = false;
        for (int e = 0; e < E; e++) if (coveredDep[e] == dep && (edges[e].first == v || edges[e].second == v)) {
            coveredDep[e] = -1; curE++;
        }
    }
    bool hasCover(int V, int K) { // returns true if there is a vertex cover of at most K vertices
        function<bool(int)> solve = [&] (int k) {
            if (curE == 0) return true;
            if (curE > k * (V - 1)) return false;
            int v = -1, w = -1;
            for (int e = 0; e < E; e++) if (coveredDep[e] == -1) { tie(v, w) = edges[e]; break; }
            cover(v, k);
            if (solve(k - 1)) return true;
            uncover(v, k); cover(w, k);
            if (solve(k - 1)) return true;
            uncover(w, k); return false;
        };
        commonSetUp(V); return solve(K);
    }
    int minCover(int V, int K) { // return the size of the minimum vertex cover of at most K vertices (or V if none exist)
        int mnCover = V;
        function<void(int)> solve = [&] (int k) {
            if (k >= mnCover) return;
            if (curE == 0) { mnCover = k; copy(inCover, inCover + V, temp); return; }
            if (k >= K) return;
            int v = -1, w = -1;
            for (int e = 0; e < E; e++) if (coveredDep[e] == -1) { tie(v, w) = edges[e]; break; }
            cover(v, k); solve(k + 1); uncover(v, k); cover(w, k); solve(k + 1); uncover(w, k);
        };
        commonSetUp(V); fill(temp, temp + V, true); solve(0); copy(temp, temp + V, inCover); return mnCover;
    }
};
