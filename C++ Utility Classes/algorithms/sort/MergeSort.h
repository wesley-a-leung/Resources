#ifndef ALGORITHMS_SORT_MERGESORT_H_
#define ALGORITHMS_SORT_MERGESORT_H_

#include <bits/stdc++.h>
using namespace std;

const int INSERTION_SORT_CUTOFF = 8;

template <typename T> inline void exch(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template <typename It> void merge_sort(It src_st, It src_en, It dst_st, It dst_en) {
    int n = src_en - src_st;
    if (n <= INSERTION_SORT_CUTOFF) {
        for (int i = 0; i < n; i++) {
            for (int j = i; j > 0 && dst_st[j] < dst_st[j - 1]; j--) {
                exch(dst_st + j, dst_st + j - 1);
            }
        }
        return;
    }
    int mid = (n - 1) / 2;
    merge_sort(dst_st, dst_st + mid + 1, src_st, src_st + mid + 1);
    merge_sort(dst_st + mid + 1, dst_en, src_st + mid + 1, src_en);
    if (src_st[mid + 1] >= src_st[mid]) {
        for (int i = 0; i < n; i++) dst_st[i] = src_st[i];
        return;
    }
    int i = 0, j = mid + 1;
    for (int k = 0; k < n; k++) {
        if (i > mid) dst_st[k] = src_st[j++];
        else if (j >= n) dst_st[k] = src_st[i++];
        else if (src_st[j] < src_st[i]) dst_st[k] = src_st[j++];
        else dst_st[k] = src_st[i++];
    }
}

template <typename It> void merge_sort(It st, It en) {
    typedef typename std::iterator_traits<It>::value_type T;
    int n = en - st;
    T *aux = new T[n];
    for (int i = 0; i < n; i++) aux[i] = st[i];
    merge_sort(aux, aux + n, st, en);
    delete[] (aux);
}

template <typename It, typename Comparator> void merge_sort(It src_st, It src_en, It dst_st, It dst_en, Comparator cmp) {
    int n = src_en - src_st;
    if (n <= INSERTION_SORT_CUTOFF) {
        for (int i = 0; i < n; i++) {
            for (int j = i; j > 0 && cmp(dst_st[j], dst_st[j - 1]); j--) {
                exch(dst_st + j, dst_st + j - 1);
            }
        }
        return;
    }
    int mid = (n - 1) / 2;
    merge_sort(dst_st, dst_st + mid + 1, src_st, src_st + mid + 1);
    merge_sort(dst_st + mid + 1, dst_en, src_st + mid + 1, src_en);
    if (!cmp(src_st[mid + 1], src_st[mid])) {
        for (int i = 0; i < n; i++) dst_st[i] = src_st[i];
        return;
    }
    int i = 0, j = mid + 1;
    for (int k = 0; k < n; k++) {
        if (i > mid) dst_st[k] = src_st[j++];
        else if (j >= n) dst_st[k] = src_st[i++];
        else if (cmp(src_st[j], src_st[i])) dst_st[k] = src_st[j++];
        else dst_st[k] = src_st[i++];
    }
}

template <typename It, typename Comparator> void merge_sort(It st, It en, Comparator cmp) {
    typedef typename std::iterator_traits<It>::value_type T;
    int n = en - st;
    T *aux = new T[n];
    for (int i = 0; i < n; i++) aux[i] = st[i];
    merge_sort(aux, aux + n, st, en);
    delete[] (aux);
}

#endif /* ALGORITHMS_SORT_MERGESORT_H_ */
