#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b and finding the maximum value of f(x) at any given x
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   addLine, getMax: O(1) amortized
//   getMaxArb: O(log N)
// Memory Complexity: O(N) where N is the total number of lines added
template <const int MAXN, class T, class Comparator = less<T>> struct ConvexHullTrick {
    Comparator cmp; T M[MAXN], B[MAXN]; int front = 0, back = 0;
    void addLine(T m, T b) { // in non decreasing order of slope, as sorted by the comparator
        while (back >= 2 && (M[back - 1] - m) * (B[back - 2] - b) <= (B[back - 1] - b) * (M[back - 2] - m)) back--;
        while (back >= 1 && !cmp(m, M[back - 1]) && !cmp(M[back - 1], m) && !cmp(b, B[back - 1])) back--;
        M[back] = m; B[back++] = b;
    }
    T getMax(T x) { // in non decreasing order of x, regardless of comparator
        while (front < back - 1 && !cmp(M[front + 1] * x + B[front + 1], M[front] * x + B[front])) front++;
        return M[front] * x + B[front];
    }
    T getMaxArb(T x) { // in arbitrary order
        int lo = 0, hi = back - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (!cmp(M[mid + 1] * x + B[mid + 1], M[mid] * x + B[mid])) lo = mid + 1;
            else hi = mid;
        }
        return M[lo] * x + B[lo];
    }
    void clear() { front = back = 0; }
};
