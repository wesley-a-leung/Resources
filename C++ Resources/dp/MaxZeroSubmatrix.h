#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the area of the largest submatrix that contains only 0s
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
template <const int MAXN, const int MAXM = MAXN> struct MaxZeroSubmatrix {
    int A[MAXN][MAXM], H[MAXN][MAXM];
    int solve(int N, int M) {
        stack<int, vector<int>> s; int ret = 0;
        for (int j = 0; j < M; j++) for (int i = N - 1; i >= 0; i--) H[i][j] = A[i][j] ? 0 : 1 + (i == N - 1 ? 0 : H[i + 1][j]);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                int minInd = j;
                while (!s.empty() && H[i][s.top()] >= H[i][j]) {
                    ret = max(ret, (j - s.top()) * (H[i][s.top()]));
                    minInd = s.top(); s.pop(); H[i][minInd] = H[i][j];
                }
                s.push(minInd);
            }
            while (!s.empty()) ret = max(ret, (M - s.top()) * H[i][s.top()]); s.pop();
        }
        return ret;
    }
};
