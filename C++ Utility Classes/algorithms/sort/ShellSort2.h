#ifndef ALGORITHMS_SORT_SHELLSORT2_H_
#define ALGORITHMS_SORT_SHELLSORT2_H_

#include <bits/stdc++.h>
using namespace std;

// O(N ^ (4 / 3)) Shell Sort

template <typename It> void shell_sort2(It st, It en) {
    int n = en - st;
    int h = 1;
    while (h < n * 4 / 9) h = h * 9 / 4 + 1;
    while (h >= 1) {
        for (int i = h; i < n; i++) {
            for (int j = i; j >= h && st[j] < st[j - h]; j -= h) {
                swap(st[j], st[j - h]);
            }
        }
        h = h * 4 / 9;
    }
}

template <typename It, typename Comparator> void shell_sort2(It st, It en, Comparator cmp) {
    int n = en - st;
    int h = 1;
    while (h < n * 4 / 9) h = h * 9 / 4 + 1;
    while (h >= 1) {
        for (int i = h; i < n; i++) {
            for (int j = i; j >= h && cmp(st[j], st[j - h]); j -= h) {
                swap(st[j], st[j - h]);
            }
        }
        h = h * 4 / 9;
    }
}

#endif /* ALGORITHMS_SORT_SHELLSORT2_H_ */
