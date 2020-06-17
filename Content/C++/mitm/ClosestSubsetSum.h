#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the closest non-empty subset sum to a value, using a meet in the middle approach
// Time Complexity: Reduces the runtime from O(N * 2^N) to O(N * 2^(N/2))
// Memory Complexity: O(2^(N/2))
template <const int MAXN, class T> struct ClosestSubsetSum {
    T A[MAXN]; vector<int> ans; // ans contains the closest non-empty subset sum to value
    void solveHalf(vector<T> &half, vector<pair<T, int>> &sum) {
        for (int i = 1; i < (1 << int(half.size())); i++) {
            T curSum = 0;
            for (int j = 0; j < int(half.size()); j++) if (i & (1 << j)) curSum += half[j];
            sum.push_back(make_pair(curSum, i));
        }
        sort(sum.begin(), sum.end()); sum.resize(unique(sum.begin(), sum.end()) - sum.begin());
    }
    T solve(int N, T value) { // returns the closest sum, elements are stored in ans
        ans.clear(); T minDiff = numeric_limits<T>::max(), closestSum = numeric_limits<T>::max(); int evenPerm = 0, oddPerm = 0;
        vector<T> even, odd; even.reserve(N - N / 2); odd.reserve(N / 2);
        vector<pair<T, int>> evenSum, oddSum; evenSum.reserve(1 << (N - N / 2)); oddSum.reserve(1 << (N / 2));
        for (int i = 0; i < N; i++) {
            if (i % 2 == 0) even.push_back(A[i]);
            else odd.push_back(A[i]);
        }
        solveHalf(even, evenSum); solveHalf(odd, oddSum);
        if (int(evenSum.size()) > 0 && abs(value - evenSum[0].first) < minDiff) {
            evenPerm = evenSum[0].second; oddPerm = 0; closestSum = evenSum[0].first; minDiff = abs(value - evenSum[0].first);
        }
        if (int(oddSum.size()) > 0 && abs(value - oddSum[0].first) < minDiff) {
            evenPerm = 0; oddPerm = oddSum[0].second; closestSum = oddSum[0].first; minDiff = abs(value - oddSum[0].first);
        }
        for (int i = 0; i < int(evenSum.size()); i++) {
            int j = lower_bound(oddSum.begin(), oddSum.end(), make_pair(value - evenSum[i].first, -1)) - oddSum.begin();
            if (j == int(oddSum.size())) {
                if (j > 0 && abs(value - (evenSum[i].first + oddSum[j - 1].first)) < minDiff) {
                    evenPerm = evenSum[i].second; oddPerm = oddSum[j - 1].second;
                    closestSum = evenSum[i].first + oddSum[j - 1].first;
                    minDiff = abs(value - (evenSum[i].first + oddSum[j - 1].first));
                }
            } else if (evenSum[i].first + oddSum[j].first != value) {
                if (j > 0 && abs(value - (evenSum[i].first + oddSum[j - 1].first)) < minDiff) {
                    evenPerm = evenSum[i].second; oddPerm = oddSum[j - 1].second;
                    closestSum = evenSum[i].first + oddSum[j - 1].first;
                    minDiff = abs(value - (evenSum[i].first + oddSum[j - 1].first));
                }
                if (abs(value - (evenSum[i].first + oddSum[j].first)) < minDiff) {
                    evenPerm = evenSum[i].second; oddPerm = oddSum[j].second;
                    closestSum = evenSum[i].first + oddSum[j].first;
                    minDiff = abs(value - (evenSum[i].first + oddSum[j].first));
                }
            } else {
                if (abs(value - (evenSum[i].first + oddSum[j].first)) < minDiff) {
                    evenPerm = evenSum[i].second; oddPerm = oddSum[j].second;
                    closestSum = evenSum[i].first + oddSum[j].first;
                    minDiff = abs(value - (evenSum[i].first + oddSum[j].first));
                }
            }
        }
        for (int i = 0; i < int(even.size()); i++) if (evenPerm & (1 << i)) ans.push_back(i * 2);
        for (int i = 0; i < int(odd.size()); i++) if (oddPerm & (1 << i)) ans.push_back(i * 2 + 1);
        return closestSum;
    }
};
