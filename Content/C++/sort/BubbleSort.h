#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using bubble sort
// Time Complexity: O(N^2) worse case
// Memory Complexity: O(1) additional memory

template <class It> void bubble_sort(It st, It en) {
    int n = en - st;
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++) if (st[j + 1] < st[j]) swap(st[j + 1], st[j]);
}

/////////////////// COMPARATOR SORT ///////////////////

template <class It, class Comparator> void bubble_sort(It st, It en, Comparator cmp) {
    int n = en - st;
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++) if (cmp(st[j + 1], st[j])) swap(st[j + 1], st[j]);
}
