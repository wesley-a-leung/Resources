#ifndef ALGORITHMS_MATH_MAXIMUMSUBARRAYSUM_H_
#define ALGORITHMS_MATH_MAXIMUMSUBARRAYSUM_H_

#include <bits/stdc++.h>

using namespace std;

int maxSum, start, end;

/**
 * Computes the subarray with the maximum sum.
 *
 * @param a the array
 * @param n the length of the array
 */
void solve(int *a, int n) {
    maxSum = 0;
    start = 0;
    end = 0;
    int curMax = 0;
    int curStart = 0;
    for (int i = 0; i < n; i++) {
        curMax += a[i];
        if (curMax < 0) {
            curMax = 0;
            curStart = i + 1;
        }
        if (maxSum < curMax) {
            maxSum = curMax;
            start = curStart;
            end = i;
        }
    }
}

#endif /* ALGORITHMS_MATH_MAXIMUMSUBARRAYSUM_H_ */
