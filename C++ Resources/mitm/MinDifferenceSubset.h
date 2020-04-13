#pragma once
#include <bits/stdc++.h>
using namespace std;

// Partitions a set into three disjoint subsets, such that at least two are non empty,
// and have the smallest absolute difference of sums between them, using a meet in the middle approach
// Time Complexity: Reduces the runtime from O(N * 3^N) to O(N * 3^(N/2))
// Memory Complexity: O(3^(N/2))
template <const int MAXN, class T> struct MinDifferenceSubset {
    T A[MAXN]; vector<int> setA, setB; // setA and setB contain the two disjoint non-empty subsets
    int POW(int N) {
        int ret = 1;
        for (int i = 0; i < N; i++) ret *= 3;
        return ret;
    }
    void solveHalf(vector<T> &half, vector<pair<T, int>> &diff) {
        int perms = POW(int(half.size()));
        for (int i = 1; i < perms; i++) {
            int cur = i; T curDiff = 0;
            for (int j = 0; j < int(half.size()); j++) {
                if (cur % 3 == 1) curDiff += half[j];
                else if (cur % 3 == 2) curDiff -= half[j];
                cur /= 3;
            }
            diff.push_back(make_pair(curDiff, i));
        }
        sort(diff.begin(), diff.end()); diff.resize(unique(diff.begin(), diff.end()) - diff.begin());
    }
    T solve(int N) { // returns the smallest absolute difference of sums, elements are stored in setA and setB
        T minDiff = numeric_limits<T>::max(); vector<T> even, odd; even.reserve(N - N / 2); odd.reserve(N / 2);
        vector<pair<T, int>> evenDiff, oddDiff; evenDiff.reserve(POW(N - N / 2)); oddDiff.reserve(POW(N / 2));
        int evenPerm = 0, oddPerm = 0; setA.clear(); setB.clear();
        for (int i = 0; i < N; i++) {
            if (i % 2 == 0) even.push_back(A[i]);
            else odd.push_back(A[i]);
        }
        solveHalf(even, evenDiff); solveHalf(odd, oddDiff); int p = 0, q = 0;
        if (int(evenDiff.size()) > 0 && abs(evenDiff[0].first) < minDiff) {
            evenPerm = evenDiff[0].second; oddPerm = 0; minDiff = abs(evenDiff[0].first);
        }
        if (int(oddDiff.size()) > 0 && abs(oddDiff[0].first) < minDiff) {
            evenPerm = 0; oddPerm = oddDiff[0].second; minDiff = abs(oddDiff[0].first);
        }
        while (p < int(evenDiff.size()) && q < int(oddDiff.size())) {
            if (abs(evenDiff[p].first - oddDiff[q].first) < minDiff) {
                evenPerm = evenDiff[p].second; oddPerm = oddDiff[q].second; minDiff = abs(evenDiff[p].first - oddDiff[q].first);
            }
            if (evenDiff[p].first < oddDiff[q].first) p++;
            else if (evenDiff[p].first > oddDiff[q].first) q++;
            else break;
        }
        for (int i = 0; i < int(even.size()); i++) {
            if (evenPerm % 3 == 1) setA.push_back(i * 2);
            else if (evenPerm % 3 == 2) setB.push_back(i * 2);
            evenPerm /= 3;
        }
        for (int i = 0; i < int(odd.size()); i++) {
            if (oddPerm % 3 == 1) setB.push_back(i * 2 + 1);
            else if (oddPerm % 3 == 2) setA.push_back(i * 2 + 1);
            oddPerm /= 3;
        }
        return minDiff;
    }
};
