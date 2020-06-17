#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using insertion sort
// Time Complexity: O(N^2) worse case
// Memory Complexity: O(1) additional memory

template <class It> void insertion_sort(It st, It en) {
    int n = en - st;
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && st[j] < st[j - 1]; j--) swap(st[j], st[j - 1]);
}

/////////////////// COMPARATOR SORT ///////////////////

template <class It, class Comparator> void insertion_sort(It st, It en, Comparator cmp) {
    int n = en - st;
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && cmp(st[j], st[j - 1]); j--) swap(st[j], st[j - 1]);
}
