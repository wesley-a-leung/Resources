#pragma once
#include <bits/stdc++.h>
#include "../../utils/MakeUnique.h"
using namespace std;

// Shortest Path Faster Algorithm for weighted graphs without negative weights
// Time Complexity: O(VE) in the worst case, O(E) on average
//   If there is a negative cycle, then it may not terminate
//   If there are negative weights, but no negative cycles, the time complexity can become exponential
// Memory Complexity: O(V + E)

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937 rng(seq);

template <const int MAXV, class unit> struct SPFA {
    unit INF, dist[MAXV]; int DQ[MAXV]; SPFA(unit INF) : INF(INF) {}
    bool inQueue[MAXV]; pair<int, unit> to[MAXV]; vector<pair<int, unit>> adj[MAXV];
    void addEdge(int v, int w, unit weight) { adj[v].emplace_back(w, weight); }
    void addBiEdge(int v, int w, unit weight) { addEdge(v, w, weight); addEdge(w, v, weight); }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    void run(int V, const vector<int> &src) {
        fill(dist, dist + V, INF); fill(to, to + V, make_pair(-1, 0)); fill(inQueue, inQueue + V, false); int front = 0, back = 0, qsize = 0;
        for (int v = 0; v < V; v++) shuffle(adj[v].begin(), adj[v].end(), rng);
        for (int s : src) {
            DQ[back++] = s; qsize++; inQueue[s] = true; dist[s] = 0;
            if (back >= MAXV) back = 0;
        }
        while (qsize > 0) {
            int v = DQ[front++]; qsize--; inQueue[v] = false;
            if (front >= MAXV) front = 0;
            for (auto &&e : adj[v]) if (dist[e.first] > dist[v] + e.second) {
                dist[e.first] = dist[v] + e.second; to[e.first] = make_pair(v, e.second);
                if (!inQueue[e.first]) {
                    if (qsize > 0 && dist[e.first] <= dist[DQ[front]]) {
                        if (front <= 0) front = MAXV;
                        DQ[--front] = e.first; qsize++;
                    } else {
                        DQ[back++] = e.first; qsize++;
                        if (back >= MAXV) back = 0;
                    }
                    inQueue[e.first] = true;
                }
            }
        }
    }
    void run(int V, int s) { run(V, vector<int>(1, s)); }
};
