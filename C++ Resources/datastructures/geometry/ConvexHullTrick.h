#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b and finding the maximum value of f(x) at any given x
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   addLine, getMax: O(1)
// Memory Complexity: O(N) where N is the total number of lines added
template <const int MAXN, class T, class Comparator = less<T>> struct ConvexHullTrick {
    Comparator cmp; T M[MAXN], B[MAXN]; int front = 0, back = 0;
    void addLine(T m, T b) { // in non decreasing order of slope, as sorted by the comparator
        while (back >= 2 && (B[back - 2] - B[back - 1]) * (m - M[back - 1]) >= (B[back - 1] - b) * (M[back - 1] - M[back - 2])) back--;
        while (back >= 1 && m == M[back - 1] && !cmp(b, B[back - 1])) back--;
        M[back] = m; B[back++] = b;
    }
    T getMax(T x) { // in non decreasing order of x, regardless of comparator
        while (front < back - 1 && !cmp(M[front + 1] * x + B[front + 1], M[front] * x + B[front])) front++;
        return M[front] * x + B[front];
    }
    void clear() { front = back = 0; }
};
