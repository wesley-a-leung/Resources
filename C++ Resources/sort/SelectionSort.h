#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using selection sort
// Time Complexity: O(N^2) worse case
// Memory Complexity: O(1) additional memory

template <class It> void selection_sort(It st, It en) {
    int n = en - st;
    for (int i = 0; i < n; i++) swap(st[i], *min_element(st + i, en));
}

/////////////////// COMPARATOR SORT ///////////////////

template <class It, class Comparator> void selection_sort(It st, It en, Comparator cmp) {
    int n = en - st;
    for (int i = 0; i < n; i++) swap(st[i], *min_element(st + i, en, cmp));
}
