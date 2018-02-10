#ifndef ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMKLESS_H_
#define ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMKLESS_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Returns the maximum subarray sum of size K or less.
 *
 * @param A the array
 * @param N the length of the array
 * @param K the maximum length of the subarray window
 * @return the maximum subarray sum of size K or less
 */
int solve(int *A, int N, int K) {
    int prefix[N];
    prefix[0] = A[0];
    for (int i = 1; i < N; i++) prefix[i] = prefix[i - 1] + A[i];
    deque<int> dq;
    int ans = 0;
    for (int i = 0; i < N; i++) {
        while (!dq.empty() && dq.front() < i - K) dq.pop_front();
        while (!dq.empty() && prefix[dq.back()] >= prefix[i]) dq.pop_back();
        dq.push_back(i);
        ans = max(ans, prefix[i] - prefix[dq.front()]);
    }
    return ans;
}

#endif /* ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMKLESS_H_ */
