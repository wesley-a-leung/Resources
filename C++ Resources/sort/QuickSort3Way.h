#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sorts an array using 3-way quick sort
// Time Complexity: O(N^2) worse case, O(N log N) average case
// Memory Complexity: O(1) additional memory

const int INSERTION_SORT_CUTOFF = 8;
const int MEDIAN_OF_3_CUTOFF = 40;

template <class It> inline It median3(It a, It b, It c) {
    return (*a < *b ? (*b < *c ? b : (*a < *c ? c : a)) : (*c < *b ? b : (*c < *a ? c : a)));
}

template <class It> void quick_sort_3_way(It st, It en) {
    int n = en - st;
    if (n <= INSERTION_SORT_CUTOFF) {
        for (int i = 0; i < n; i++) for (int j = i; j > 0 && st[j] < st[j - 1]; j--)
            swap(st[j], st[j - 1]);
        return;
    } else if (n <= MEDIAN_OF_3_CUTOFF) {
        It m = median3(st, st + n / 2, en - 1); swap(*st, *m);
    } else {
        int eps = n / 8; It mid = st + n / 2;
        It ninther = median3(median3(st, st + eps, st + eps + eps),
                median3(mid - eps, mid, mid + eps),
                median3(en - 1 - eps - eps, en - 1 - eps, en - 1));
        swap(*st, *ninther);
    }
    int i = 0, j = n, p = 0, q = n; auto v = st[0];
    while (true) {
        while (st[++i] < v) if (i == n - 1) break;
        while (v < st[--j]) if (j == 0) break;
        if (i == j && st[i] == v) swap(st[++p], st[i]);
        if (i >= j) break;
        swap(st[i], st[j]);
        if (st[i] == v) swap(st[++p], st[i]);
        if (st[j] == v) swap(st[--q], st[j]);
    }
    i = j + 1;
    for (int k = 0; k <= p; k++, j--) swap(st[k], st[j]);
    for (int k = n - 1; k >= q; k--, i++) swap(st[k], st[i]);
    quick_sort_3_way(st, st + j + 1); quick_sort_3_way(st + i, en);
}

/////////////////// COMPARATOR SORT ///////////////////

template <class It, class Comparator> void quick_sort_3_way(It st, It en, Comparator cmp) {
    int n = en - st;
    if (n <= INSERTION_SORT_CUTOFF) {
        for (int i = 0; i < n; i++) for (int j = i; j > 0 && cmp(st[j], st[j - 1]); j--)
            swap(st[j], st[j - 1]);
        return;
    } else if (n <= MEDIAN_OF_3_CUTOFF) {
        It m = median3(st, st + n / 2, en - 1); swap(*st, *m);
    } else {
        int eps = n / 8; It mid = st + n / 2;
        It ninther = median3(median3(st, st + eps, st + eps + eps),
                median3(mid - eps, mid, mid + eps),
                median3(en - 1 - eps - eps, en - 1 - eps, en - 1));
        swap(*st, *ninther);
    }
    int i = 0, j = n, p = 0, q = n; auto v = st[0];
    while (true) {
        while (cmp(st[++i], v)) if (i == n - 1) break;
        while (cmp(v, st[--j])) if (j == 0) break;
        if (i == j && !cmp(st[i], v) && !cmp(v, st[i])) swap(st[++p], st[i]);
        if (i >= j) break;
        swap(st[i], st[j]);
        if (!cmp(st[i], v) && !cmp(v, st[i])) swap(st[++p], st[i]);
        if (!cmp(st[j], v) && !cmp(v, st[j])) swap(st[--q], st[j]);
    }
    i = j + 1;
    for (int k = 0; k <= p; k++, j--) swap(st[k], st[j]);
    for (int k = n - 1; k >= q; k--, i++) swap(st[k], st[i]);
    quick_sort_3_way(st, st + j + 1, cmp); quick_sort_3_way(st + i, en, cmp);
}
