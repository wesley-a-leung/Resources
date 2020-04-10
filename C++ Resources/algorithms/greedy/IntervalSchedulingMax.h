#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; const T EPS = 1e-9;

bool lt(T a, T b) { return a + EPS < b; }

// Solves the interval scheduling maximization problem
// Given a set of intervals in the form [L, R), find the maximum number of non overlapping intervals
// Returns an iterator to right after the last disjoint interval
// Time Complexity: O(N)
// Memory Complexity: O(1)
bool cmp(const pair<T, T> &a, const pair<T, T> &b) { return lt(a.second, b.second); }
template <class It> It intervalSchedulingMax(It st, It en) {
    assert(is_sorted(st, en, cmp));
    It cur = st;
    for (It l = st, r; l < en; l = r, cur++) {
        *cur = *l;
        for (r = l + 1; r < en && lt(r->first, cur->second); r++);
    }
    return cur;
}
