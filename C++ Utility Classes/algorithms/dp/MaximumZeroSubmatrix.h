#ifndef ALGORITHMS_DP_MAXIMUMZEROSUBMATRIX_H_
#define ALGORITHMS_DP_MAXIMUMZEROSUBMATRIX_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Computes the maximum zero submatrix, given a 2D array.
 *
 * @param a the 2D array
 * @param n the number of rows int the array
 * @param m the number of columns in the array
 */
int solve(int **a, int n, int m) {
    int height[n][m];
    stack<int> s;
    int ret = 0;
    for (int j = 0; j < m; j++) {
        for (int i = n - 1; i >= 0; i--) {
            if (a[i][j] == 1) height[i][j] = 0;
            else height[i][j] = 1 + (i == n - 1 ? 0 : height[i + 1][j]);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int minIndex = j;
            while (!s.empty() && height[i][s.top()] >= height[i][j]) {
                ret = max(ret, (j - s.top()) * (height[i][s.top()]));
                minIndex = s.top();
                height[i][minIndex] = height[i][j];
                s.pop();
            }
            s.push(minIndex);
        }
        while (!s.empty()) {
            ret = max(ret, (m - s.top()) * height[i][s.top()]);
            s.pop();
        }
    }
    return ret;
}

#endif /* ALGORITHMS_DP_MAXIMUMZEROSUBMATRIX_H_ */
