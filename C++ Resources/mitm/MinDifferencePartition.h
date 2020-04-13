#pragma once
#include <bits/stdc++.h>
using namespace std;

// Paritions a set into two disjoint non-empty subsets with the smallest absolute difference
// of sums between them, using a meet in the middle approach
// Time Complexity: Reduces the runtime from O(N * 2^N) to O(N * 2^(N/2))
// Memory Complexity: O(2^(N/2))
template <const int MAXN, class T> struct MinDifferencePartition {
    T A[MAXN]; vector<int> setA, setB; // setA and setB contain the two disjoint non-empty subsets forming a partition
    void solveHalf(vector<T> &half, vector<pair<T, int>> &diff) {
        for (int i = 0; i < (1 << int(half.size())); i++) {
            T curDiff = 0;
            for (int j = 0; j < int(half.size()); j++) {
                if ((i >> j) & 1) curDiff += half[j];
                else curDiff -= half[j];
            }
            diff.push_back(make_pair(curDiff, i));
        }
        sort(diff.begin(), diff.end()); diff.resize(unique(diff.begin(), diff.end()) - diff.begin());
    }
    T solve(int N) { // returns the smallest absolute difference of sums, set is partitioned into setA and setB
        T minDiff = numeric_limits<T>::max(); vector<T> even, odd; even.reserve(N - N / 2); odd.reserve(N / 2);
        vector<pair<T, int>> evenDiff, oddDiff; evenDiff.reserve(1 << (N - N / 2)); oddDiff.reserve(1 << (N / 2));
        int evenPerm = 0, oddPerm = 0; setA.clear(); setB.clear();
        for (int i = 0; i < N; i++) {
            if (i % 2 == 0) even.push_back(A[i]);
            else odd.push_back(A[i]);
        }
        solveHalf(even, evenDiff); solveHalf(odd, oddDiff); int p = 0, q = 0;
        while (p < int(evenDiff.size()) && q < int(oddDiff.size())) {
            if (abs(evenDiff[p].first - oddDiff[q].first) < minDiff) {
                evenPerm = evenDiff[p].second; oddPerm = oddDiff[q].second; minDiff = abs(evenDiff[p].first - oddDiff[q].first);
            }
            if (evenDiff[p].first < oddDiff[q].first) p++;
            else if (evenDiff[p].first > oddDiff[q].first) q++;
            else break;
        }
        for (int i = 0; i < (int) even.size(); i++) {
            if ((evenPerm >> i) & 1) setA.push_back(i * 2);
            else setB.push_back(i * 2);
        }
        for (int i = 0; i < (int) odd.size(); i++) {
            if ((oddPerm >> i) & 1) setB.push_back(i * 2 + 1);
            else setA.push_back(i * 2 + 1);
        }
        return minDiff;
    }
};
