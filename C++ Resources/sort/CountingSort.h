#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using counting sort, works with integer values
// Time Complexity: O(N + K) worst case for N elements over a range of K
// Memory Complexity: O(N + K) additional memory
template <class It> void counting_sort(It st, It en) {
    int n = en - st;
    if (n == 0) return;
    auto maxVal = *st, minVal = *en;
    for (It i = st; i < en; i++) { maxVal = max(maxVal, *i); minVal = min(minVal, *i); }
    int range = maxVal - minVal + 1, *count = new int[range], *b = new int[n];
    for (int i = 0; i < range; i++) count[i] = 0;
    for (It i = st; i < en; i++) count[*i - minVal]++;
    for (int i = 1; i < range; i++) count[i] += count[i - 1];
    for (It i = st; i < en; i++) b[--count[*i - minVal]] = *i;
    for (It i = st; i < en; i++) *i = b[i - st];
    delete[] (count); delete[] (b);
}
