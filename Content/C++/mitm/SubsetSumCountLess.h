#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the number of subsets that sum less than or equal to a value
// using a meet in the middle approach
// Time Complexity: Reduces the runtime from O(N * 2^N) to O(N * 2^(N/2))
// Memory Complexity: O(2^(N/2))
template <const int MAXN, class T> struct SubsetSumCountLess {
    T A[MAXN];
    long long solveHalf(vector<T> &half, vector<T> &sum, T value) {
        long long ret = 0;
        for (int i = 1; i < (1 << int(half.size())); i++) {
            T curSum = 0;
            for (int j = 0; j < int(half.size()); j++) if (i & (1 << j)) curSum += half[j];
            sum.push_back(curSum);
            if (curSum <= value) ret++;
        }
        sort(sum.begin(), sum.end());
        return ret;
    }
    long long solve(int N, T value) {
        long long ret = 0; vector<T> even, odd; even.reserve(N - N / 2); odd.reserve(N / 2);
        vector<T> evenSum, oddSum; evenSum.reserve(1 << (N - N / 2)); oddSum.reserve(1 << (N / 2));
        for (int i = 0; i < N; i++) {
            if (i % 2 == 0) even.push_back(A[i]);
            else odd.push_back(A[i]);
        }
        ret += solveHalf(even, evenSum, value); ret += solveHalf(odd, oddSum, value);
        if (value >= 0) ret++;
        for (int i = 0, j = int(oddSum.size()) - 1; i < int(evenSum.size()); i++) {
            while (j >= 0 && evenSum[i] + oddSum[j] > value) j--;
            ret += j + 1;
        }
        return ret;
    }
};
