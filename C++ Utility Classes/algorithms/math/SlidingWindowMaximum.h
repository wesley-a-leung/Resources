#ifndef ALGORITHMS_MATH_SLIDINGWINDOWMAXIMUM_H_
#define ALGORITHMS_MATH_SLIDINGWINDOWMAXIMUM_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Returns an array with the maximum element for each contiguous subarray of size K starting at index i
 *
 * @param A the array
 * @param N the length of the array
 * @param K the length of the subarray window
 * @return an array with the maximum element for each contiguous subarray of size K starting at index i
 */
int *compute(int *A, int N, int K) {
    int *ans = new int[N - K + 1];
    deque<int> dq;
    for (int i = 0; i < N; i++) {
        while (!dq.empty() && dq.front() <= i - K) dq.pop_front();
        while (!dq.empty() && A[dq.back()] <= A[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= K - 1) ans[i - K + 1] = A[dq.front()];
    }
    return ans;
}

#endif /* ALGORITHMS_MATH_SLIDINGWINDOWMAXIMUM_H_ */
