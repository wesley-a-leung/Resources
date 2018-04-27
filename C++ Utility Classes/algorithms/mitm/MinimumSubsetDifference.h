#ifndef ALGORITHMS_MITM_MINIMUMSUBSETDIFFERENCE_H_
#define ALGORITHMS_MITM_MINIMUMSUBSETDIFFERENCE_H_

#include <bits/stdc++.h>
using namespace std;

// solves half of the problem by iterating through each permutation
// each element can either be in the first set, the second set, or neither
template <typename T> void solveHalf(vector<T> &half, vector<pair<T, int>> &diff) {
    int perms = 1;
    for (int i = 0; i < (int) half.size(); i++) perms *= 3;
    for (int i = 1; i < perms; i++) {
        int cur = i;
        T curDiff = 0;
        for (int j = 0; j < (int) half.size(); j++) {
            if (cur % 3 == 1) curDiff += half[j];
            else if (cur % 3 == 2) curDiff -= half[j];
            cur /= 3;
        }
        diff.push_back(make_pair(curDiff, i));
    }
    sort(diff.begin(), diff.end());
    diff.resize(unique(diff.begin(), diff.end()) - diff.begin());
}

/**
 * Divides a set into two disjoint non-empty subsets with the smallest absolute difference
 * between them. The two subsets do not need to contain all the elements or be equal in size.
 *
 * Uses a meet in the middle approach to reduce the runtime from O(N * 3 ^ N) to O(N * 3 ^ (N / 2))
 *
 * @param all the set of all elements
 * @param setA the 0-based indices of the first subset
 * @param setB the 0-based indices of the second subset
 * @return returns the smallest absolute difference between the 2 sets
 */
template <typename T> T solve(vector<T> &all, vector<T> &setA, vector<T> &setB) {
    T minDiff = numeric_limits<T>::max();
    vector<T> even, odd;
    even.reserve(all.size() / 2 + all.size() % 2);
    odd.reserve(all.size() / 2);
    vector<pair<T, int>> evenDiff, oddDiff;
    evenDiff.reserve(1 << (all.size() / 2 + all.size() % 2));
    oddDiff.reserve(1 << (all.size() / 2));
    int evenPerm, oddPerm;
    for (int i = 0; i < (int) all.size(); i++) {
        if (i % 2 == 0) even.push_back(all[i]);
        else odd.push_back(all[i]);
    }
    solveHalf(even, evenDiff);
    solveHalf(odd, oddDiff);
    int p = 0, q = 0;
    if ((int) evenDiff.size() > 0) {
        if (abs(evenDiff[0].first) < minDiff) {
            evenPerm = evenDiff[0].second;
            oddPerm = 0;
            minDiff = abs(evenDiff[0].first);
        }
    }
    if ((int) oddDiff.size() > 0) {
        if (abs(oddDiff[0].first) < minDiff) {
            evenPerm = 0;
            oddPerm = oddDiff[0].second;
            minDiff = abs(oddDiff[0].first);
        }
    }
    while (p < (int) (evenDiff.size()) && q < (int) (oddDiff.size())) {
        if (abs(evenDiff[p].first - oddDiff[q].first) < minDiff) {
            evenPerm = evenDiff[p].second;
            oddPerm = oddDiff[q].second;
            minDiff = abs(evenDiff[p].first - oddDiff[q].first);
        }
        if (evenDiff[p].first < oddDiff[q].first) p++;
        else if (evenDiff[p].first > oddDiff[q].first) q++;
        else break;
    }
    for (int i = 0; i < (int) even.size(); i++) {
        if (evenPerm % 3 == 1) setA.push_back(i * 2);
        else if (evenPerm % 3 == 2) setB.push_back(i * 2);
        evenPerm /= 3;
    }
    for (int i = 0; i < (int) odd.size(); i++) {
        if (oddPerm % 3 == 1) setB.push_back(i * 2 + 1);
        else if (oddPerm % 3 == 2) setA.push_back(i * 2 + 1);
        oddPerm /= 3;
    }
    return minDiff;
}

#endif /* ALGORITHMS_MITM_MINIMUMSUBSETDIFFERENCE_H_ */
