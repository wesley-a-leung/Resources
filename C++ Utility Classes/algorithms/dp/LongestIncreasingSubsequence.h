#ifndef ALGORITHMS_DP_LONGESTINCREASINGSUBSEQUENCE_H_
#define ALGORITHMS_DP_LONGESTINCREASINGSUBSEQUENCE_H_

#include <bits/stdc++.h>
using namespace std;

int ceilIndex(int *a, int start, int end, int key) {
    while (end - start > 1) {
        int m = start + (end - start) / 2;
        if (a[m] >= key) end = m;
        else start = m;
    }
    return end;
}

/**
 * Method for determining the length of the longest increasing subsequence (allowing for skips)
 *
 * Takes time proportional to <em>N</em> log <em>N</em> where N is the length of the sequence.
 *
 * Takes space proportional to <em>N</em>.
 *
 * @param sequence the array of integers to check
 * @param n the length of the sequence
 * @return the length of the longest increasing subsequence
 */
int solve(int *sequence, int n) {
    int dp[n];
    int len = 0;
    for (int i = 0; i < n; i++) {
        int j = ceilIndex(dp, 0, len, sequence[i]);
        dp[j] = sequence[i];
        if (j == len) len++;
    }
    return len;
}

#endif /* ALGORITHMS_DP_LONGESTINCREASINGSUBSEQUENCE_H_ */
