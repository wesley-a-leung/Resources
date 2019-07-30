#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b and finding the maximum value of f(x) at any given x
// Semi-dynamic variant (allows for queries in arbitrary order)
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   addLine O(1)
//   getMax: O(log N)
// Memory Complexity: O(N) where N is the total number of lines added
template <const int MAXN, class T, class Comparator = less<T>> struct SemiDynamicConvexHullTrick {
    Comparator cmp; T M[MAXN], B[MAXN]; int front = 0, back = 0;
    void addLine(T m, T b) { // in non decreasing order of slope, as sorted by the comparator
        while (back >= 2 && (B[back - 2] - B[back - 1]) * (m - M[back - 1]) >= (B[back - 1] - b) * (M[back - 1] - M[back - 2])) back--;
        while (back >= 1 && m == M[back - 1] && !cmp(b, B[back - 1])) back--;
        M[back] = m; B[back++] = b;
    }
    T getMax(T x) {
        int lo = 0, hi = back - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (!cmp(M[mid + 1] * x + B[mid + 1], M[mid] * x + B[mid])) lo = mid + 1;
            else hi = mid;
        }
        return M[lo] * x + B[lo];
    }
};
