#ifndef ALGORITHMS_MITM_CLOSESTSUBSETSUM_H_
#define ALGORITHMS_MITM_CLOSESTSUBSETSUM_H_

#include <bits/stdc++.h>
using namespace std;

// solves half of the problem by iterating through each permutation
// each element can either be in the subset, or not in the subset
template <typename T> void solveHalf(vector<T> &half, vector<pair<T, int>> &sum) {
    for (int i = 1; i < (1 << (int) half.size()); i++) {
        T curSum = 0;
        for (int j = 0; j < (int) half.size(); j++) if (i & (1 << j)) curSum += half[j];
        sum.push_back(make_pair(curSum, i));
    }
    sort(sum.begin(), sum.end());
    sum.resize(unique(sum.begin(), sum.end()) - sum.begin());
}

/**
 * Finds the closest non-empty subset sum to a value.
 *
 * Uses a meet in the middle approach to reduce the runtime from O(N * 2 ^ N) to O(N * 2 ^ (N / 2))
 *
 * @param all the set of all elements
 * @param subset the 0-based indices of the subset
 * @return returns the closest non-empty subset sum to the value
 */
template <typename T> T solve(vector<T> &all, T value, vector<T> &subset) {
    T minDiff = numeric_limits<T>::max(), closestSum;
    vector<T> even, odd;
    vector<pair<T, int>> evenSum, oddSum;
    int evenPerm, oddPerm;
    for (int i = 0; i < (int) all.size(); i++) {
        if (i % 2 == 0) even.push_back(all[i]);
        else odd.push_back(all[i]);
    }
    solveHalf(even, evenSum);
    solveHalf(odd, oddSum);
    if ((int) evenSum.size() > 0) {
        if (abs(value - evenSum[0].first) < minDiff) {
            evenPerm = evenSum[0].second;
            oddPerm = 0;
            closestSum = evenSum[0].first;
            minDiff = abs(value - evenSum[0].first);
        }
    }
    if ((int) oddSum.size() > 0) {
        if (abs(value - oddSum[0].first) < minDiff) {
            evenPerm = 0;
            oddPerm = oddSum[0].second;
            closestSum = oddSum[0].first;
            minDiff = abs(value - oddSum[0].first);
        }
    }
    for (int i = 0; i < (int) evenSum.size(); i++) {
        int j = lower_bound(oddSum.begin(), oddSum.end(), make_pair(value - evenSum[i].first, -1)) - oddSum.begin();
        if (j == (int) oddSum.size()) {
            if (j > 0 && abs(value - (evenSum[i].first + oddSum[j - 1].first)) < minDiff) {
                evenPerm = evenSum[i].second;
                oddPerm = oddSum[j - 1].second;
                closestSum = evenSum[i].first + oddSum[j - 1].first;
                minDiff = abs(value - (evenSum[i].first + oddSum[j - 1].first));
            }
        } else if (evenSum[i].first + oddSum[j].first != value) {
            if (j > 0 && abs(value - (evenSum[i].first + oddSum[j - 1].first)) < minDiff) {
                evenPerm = evenSum[i].second;
                oddPerm = oddSum[j - 1].second;
                closestSum = evenSum[i].first + oddSum[j - 1].first;
                minDiff = abs(value - (evenSum[i].first + oddSum[j - 1].first));
            }
            if (abs(value - (evenSum[i].first + oddSum[j].first)) < minDiff) {
                evenPerm = evenSum[i].second;
                oddPerm = oddSum[j].second;
                closestSum = evenSum[i].first + oddSum[j].first;
                minDiff = abs(value - (evenSum[i].first + oddSum[j].first));
            }
        } else {
            if (abs(value - (evenSum[i].first + oddSum[j].first)) < minDiff) {
                evenPerm = evenSum[i].second;
                oddPerm = oddSum[j].second;
                closestSum = evenSum[i].first + oddSum[j].first;
                minDiff = abs(value - (evenSum[i].first + oddSum[j].first));
            }
        }
    }
    for (int i = 0; i < (int) even.size(); i++) if (evenPerm & (1 << i)) subset.push_back(i * 2);
    for (int i = 0; i < (int) odd.size(); i++) if (oddPerm & (1 << i)) subset.push_back(i * 2 + 1);
    return closestSum;
}

#endif /* ALGORITHMS_MITM_CLOSESTSUBSETSUM_H_ */
