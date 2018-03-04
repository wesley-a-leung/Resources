#ifndef ALGORITHMS_SORT_QUICKSORT_H_
#define ALGORITHMS_SORT_QUICKSORT_H_

#include <bits/stdc++.h>
using namespace std;

const int INSERTION_SORT_CUTOFF = 8;
const int MEDIAN_OF_3_CUTOFF = 40;

template <typename T> inline T *median3(T *a, T *b, T *c) {
    return (*a < *b ? (*b < *c ? b : (*a < *c ? c : a)) : (*c < *b ? b : (*c < *a ? c : a)));
}

template <typename T> inline void exch(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template <typename It> void quick_sort(It st, It en) {
    int n = en - st;
    if (n <= INSERTION_SORT_CUTOFF) { // insertion sort
        for (int i = 0; i < n; i++) {
            for (int j = i; j > 0 && st[j] < st[j - 1]; j--) {
                exch(st + j, st + j - 1);
            }
        }
        return;
    } else if (n <= MEDIAN_OF_3_CUTOFF) { // median-of-3 pivot
        It m = median3(st, st + n / 2, en - 1);
        exch(st, m);
    } else { // Tukey ninther pivot
        int eps = n / 8;
        It mid = st + n / 2;
        It ninther = median3(median3(st, st + eps, st + eps + eps),
                median3(mid - eps, mid, mid + eps),
                median3(en - 1 - eps - eps, en - 1 - eps, en - 1));
        exch(st, ninther);
    }
    // Bentley-McIlroy 3-way partitioning
    int i = 0, j = n, p = 0, q = n;
    auto v = st[0];
    while (true) {
        while (st[++i] < v) if (i == n - 1) break;
        while (v < st[--j]) if (j == 0) break;
        if (i == j && st[i] == v) exch(st + (++p), st + i);
        if (i >= j) break;
        exch(st + i, st + j);
        if (st[i] == v) exch(st + (++p), st + i);
        if (st[j] == v) exch(st + (--q), st + j);
    }
    i = j + 1;
    for (int k = 0; k <= p; k++, j--) exch(st + k, st + j);
    for (int k = n - 1; k >= q; k--, i++) exch(st + k, st + i);
    sort(st, st + j + 1);
    sort(st + i, en);
}

template <typename It, class Comparator> void quick_sort(It st, It en, Comparator cmp) {
    int n = en - st;
    if (n <= INSERTION_SORT_CUTOFF) { // insertion sort
        for (int i = 0; i < n; i++) {
            for (int j = i; j > 0 && cmp(st[j], st[j - 1]); j--) {
                exch(st + j, st + j - 1);
            }
        }
        return;
    } else if (n <= MEDIAN_OF_3_CUTOFF) { // median-of-3 pivot
        It m = median3(st, st + n / 2, en - 1);
        exch(st, m);
    } else { // Tukey ninther pivot
        int eps = n / 8;
        It mid = st + n / 2;
        It ninther = median3(median3(st, st + eps, st + eps + eps),
                median3(mid - eps, mid, mid + eps),
                median3(en - 1 - eps - eps, en - 1 - eps, en - 1));
        exch(st, ninther);
    }
    // Bentley-McIlroy 3-way partitioning
    int i = 0, j = n, p = 0, q = n;
    auto v = st[0];
    while (true) {
        while (cmp(st[++i], v)) if (i == n - 1) break;
        while (cmp(v, st[--j])) if (j == 0) break;
        if (i == j && !cmp(st[i], v) && !cmp(v, st[i])) exch(st + (++p), st + i);
        if (i >= j) break;
        exch(st + i, st + j);
        if (!cmp(st[i], v) && !cmp(v, st[i])) exch(st + (++p), st + i);
        if (!cmp(st[j], v) && !cmp(v, st[j])) exch(st + (--q), st + j);
    }
    i = j + 1;
    for (int k = 0; k <= p; k++, j--) exch(st + k, st + j);
    for (int k = n - 1; k >= q; k--, i++) exch(st + k, st + i);
    sort(st, st + j + 1, cmp);
    sort(st + i, en, cmp);
}

#endif /* ALGORITHMS_SORT_QUICKSORT_H_ */
