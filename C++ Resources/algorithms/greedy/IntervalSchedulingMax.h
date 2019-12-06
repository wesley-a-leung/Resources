#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the interval scheduling maximization problem
// Given a set of intervals [li, ri], find the maximum number of non overlapping intervals
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct IntervalSchedulingMax {
    pair<T, T> intervals[MAXN]; int cnt; bool marked[MAXN];
    int solve(int N) {
        sort(intervals, intervals + N, [&] (const pair<T, T> &a, const pair<T, T> &b) { return a.second < b.second; });
        fill(marked, marked + N, false); cnt = 0; T last = (numeric_limits<T>::lowest)();
        for (int i = 0; i < N; i++) if (last < intervals[i].first) { cnt++; marked[i] = true; last = intervals[i].second; }
        return cnt;
    }
};
