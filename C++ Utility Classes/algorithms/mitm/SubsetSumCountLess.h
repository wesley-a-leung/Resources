#ifndef ALGORITHMS_MITM_SUBSETSUMCOUNTLESS_H_
#define ALGORITHMS_MITM_SUBSETSUMCOUNTLESS_H_

#include <bits/stdc++.h>
using namespace std;

// solves half of the problem by iterating through each permutation
// each element can either be in the subset, or not in the subset
template <typename T> long long solveHalf(vector<T> &half, vector<T> &sum, T value) {
    long long ret = 0;
    for (int i = 1; i < (1 << (int) half.size()); i++) {
        T curSum = 0;
        for (int j = 0; j < (int) half.size(); j++) if (i & (1 << j)) curSum += half[j];
        sum.push_back(curSum);
        if (curSum <= value) ret++;
    }
    sort(sum.begin(), sum.end());
    return ret;
}

/**
 * Finds the number of subsets that sum less than or equal to a value.
 *
 * Uses a meet in the middle approach to reduce the runtime from O(N * 2 ^ N) to O(N * 2 ^ (N / 2))
 *
 * @param all the set of all elements
 * @return the number of subsets that sum less than or equal to a value
 */
template <typename T> long long solve(vector<T> &all, T value) {
    long long ret = 0;
    vector<T> even, odd;
    even.reserve(all.size() / 2 + all.size() % 2);
    odd.reserve(all.size() / 2);
    vector<T> evenSum, oddSum;
    evenSum.reserve(1 << (all.size() / 2 + all.size() % 2));
    oddSum.reserve(1 << (all.size() / 2));
    for (int i = 0; i < (int) all.size(); i++) {
        if (i % 2 == 0) even.push_back(all[i]);
        else odd.push_back(all[i]);
    }
    ret += solveHalf(even, evenSum, value);
    ret += solveHalf(odd, oddSum, value);
    if (value >= 0) ret++;
    int j = ((int) oddSum.size()) - 1;
    for (int i = 0; i < (int) evenSum.size(); i++) {
        while (j >= 0 && evenSum[i] + oddSum[j] > value) j--;
        ret += j + 1;
    }
    return ret;
}

#endif /* ALGORITHMS_MITM_SUBSETSUMCOUNTLESS_H_ */
