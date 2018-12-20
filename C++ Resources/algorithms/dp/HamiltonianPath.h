#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the shortest Hamiltonian Path (does not have to return to starting position)
// Time Complexity: O(N^2 * 2^N)
// Memory Complexity: O(N * 2^N)
template <const int MAXN, class T> struct HamiltonianPath {
    T INF, dist[MAXN][MAXN], dp[1 << MAXN][MAXN]; int order[MAXN];
    HamiltonianPath(T INF) : INF(INF) {}
    T run(int N) {
        T ret = INF;
        for (int i = 0; i < (1 << N); i++) for (int j = 0; j < N; j++) dp[i][j] = INF;
        for (int i = 0; i < N; i++) dp[1 << i][i] = 0;
        for (int i = 1; i < (1 << N); i++) for (int j = 0; j < N; j++) if (i & (1 << j))
            for (int k = 0; k < N; k++) if (!(i & (1 << k))) dp[i ^ (1 << k)][k] = min(dp[i ^ (1 << k)][k], dp[i][j] + dist[j][k]);
        for (int i = 0; i < N; i++) ret = min(ret, dp[(1 << N) - 1][i]);
        int curState = (1 << N) - 1, last = -1;
        for (int i = N - 1; i >= 0; i--) {
            int next = -1;
            for (int j = 0; j < N; j++)
                if ((curState & (1 << j)) && (next == -1 || dp[curState][j] + (last == -1 ? 0 : dist[j][last]) < dp[curState][next] + (last == -1 ? 0 : dist[next][last])))
                    next = j;
            order[i] = last = next; curState ^= 1 << last;
        }
        return ret;
    }
};