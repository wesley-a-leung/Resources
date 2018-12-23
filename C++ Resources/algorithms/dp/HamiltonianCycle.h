#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the shortest Hamiltonian Cycle (returning to starting position)
// Time Complexity: O(N^2 * 2^N)
// Memory Complexity: O(N * 2^N)
template <const int MAXN, class T> struct HamiltonianCycle {
    T INF, dist[MAXN][MAXN], dp[1 << MAXN][MAXN]; int order[MAXN];
    HamiltonianCycle(T INF) : INF(INF) {}
    T solve(int N) {
        T ret = INF;
        for (int i = 0; i < (1 << N); i++) for (int j = 0; j < N; j++) dp[i][j] = INF;
        dp[1][0] = 0;
        for (int i = 1; i < (1 << N); i += 2) for (int j = 0; j < N; j++) if (i & (1 << j))
            for (int k = 1; k < N; k++) if (!(i & (1 << k))) dp[i ^ (1 << k)][k] = min(dp[i ^ (1 << k)][k], dp[i][j] + dist[j][k]);
        for (int i = 1; i < N; i++) ret = min(ret, dp[(1 << N) - 1][i] + dist[i][0]);
        int curPos = 0, curState = (1 << N) - 1;
        for (int i = N - 1; i >= 0; i--) {
            int next = -1;
            for (int j = 0; j < N; j++)
                if ((curState & (1 << j)) && (next == -1 || dp[curState][j] + dist[j][curPos] < dp[curState][next] + dist[next][curPos]))
                    next = j;
            order[i] = curPos = next; curState ^= 1 << curPos;
        }
        return ret;
    }
    void init(int N = MAXN) { for (int i = 0; i < N; i++) { fill(dist[i], dist[i] + N, 0); dist[i][i] = 0; } }
    void addEdge(int v, int w, T weight) { dist[v][w] = dist[w][v] = min(weight, dist[v][w]); }
};
