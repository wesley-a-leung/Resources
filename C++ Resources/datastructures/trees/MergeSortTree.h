#pragma once
#include <bits/stdc++.h>
using namespace std;

// Merge Sort Tree supporting select and rank operations for a subarray
// Time Complexity:
//   init: O(N log N)
//   rank: O((log N)^2)
//   select: O((log N)^3)
// Memory Complexity: O(N log N)
template <const int MAXN, class T, class Comparator = less<T>> struct MergeSortTree {
    Comparator cmp; int N; T sorted[MAXN]; vector<T> TR[MAXN << 1];
    template <class It> void init(It st, It en) {
        N = en - st; for (int i = 0; i < N; i++) { TR[N + i] = vector<T>(1, sorted[i] = *(st + i)); }
        sort(sorted, sorted + N);
        for (int i = N - 1; i > 0; i--) {
            TR[i] = vector<T>(); TR[i].reserve(TR[i << 1].size() + TR[i << 1 | 1].size());
            merge(TR[i << 1].begin(), TR[i << 1].end(), TR[i << 1 | 1].begin(), TR[i << 1 | 1].end(), back_inserter(TR[i]), cmp);
        }
    }
    int rank(int l, int r, T k) { // counts the number of elements less than k in the range [l, r] (0-indexed)
        int ret = 0;
        for (l += N, r += N; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) { ret += lower_bound(TR[l].begin(), TR[l].end(), k, cmp) - TR[l].begin(); l++; }
            if (!(r & 1)) { ret += lower_bound(TR[r].begin(), TR[r].end(), k, cmp) - TR[r].begin(); r--; }
        }
        return ret;
    }
    int select(int l, int r, int k) { // selects the kth smallest element in the range [l, r] (0-indexed)
        int lo = 0, hi = N - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (rank(l, r, sorted[mid]) <= k) lo = mid + 1;
            else hi = mid - 1;
        }
        return sorted[hi];
    }
};
