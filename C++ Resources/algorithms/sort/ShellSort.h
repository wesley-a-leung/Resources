#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using shell sort
// Time Complexity: O(N^(4/3)) worse case
// Memory Complexity: O(1) additional memory

template <class It> void shell_sort(It st, It en) {
    int n = en - st, h = 1;
    while (h < n * 4 / 9) h = h * 9 / 4 + 1;
    for (; h >= 1; h = h * 4 / 9) for (int i = h; i < n; i++)
        for (int j = i; j >= h && st[j] < st[j - h]; j -= h) swap(st[j], st[j - h]);
}

/////////////////// COMPARATOR SORT ///////////////////

template <class It, class Comparator> void shell_sort(It st, It en, Comparator cmp) {
    int n = en - st, h = 1;
    while (h < n * 4 / 9) h = h * 9 / 4 + 1;
    for (; h >= 1; h = h * 4 / 9) for (int i = h; i < n; i++)
        for (int j = i; j >= h && cmp(st[j], st[j - h]); j -= h) swap(st[j], st[j - h]);
}
