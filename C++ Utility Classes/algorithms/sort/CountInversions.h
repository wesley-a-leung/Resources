#ifndef ALGORITHMS_SORT_COUNTINVERSIONS_H_
#define ALGORITHMS_SORT_COUNTINVERSIONS_H_

#include <bits/stdc++.h>
using namespace std;

template <typename It> long long count_inversions(It src_st, It src_en, It dst_st, It dst_en) {
    int n = src_en - src_st;
    if (n <= 1) return 0;
    int mid = (n - 1) / 2;
    long long ret = 0;
    ret += count_inversions(dst_st, dst_st + mid + 1, src_st, src_st + mid + 1);
    ret += count_inversions(dst_st + mid + 1, dst_en, src_st + mid + 1, src_en);
    int i = 0, j = mid + 1;
    for (int k = 0; k < n; k++) {
        if (i > mid) {
            dst_st[k] = src_st[j++];
        } else if (j >= n) {
            dst_st[k] = src_st[i++];
            ret += j - (mid + 1);
        } else if (src_st[j] < src_st[i]) {
            dst_st[k] = src_st[j++];
            ret += mid + 1 - i;
        } else {
            dst_st[k] = src_st[i++];
            ret += j - (mid + 1);
        }
    }
    return ret;
}

template <typename It> long long count_inversions(It st, It en) {
    typedef typename std::iterator_traits<It>::value_type T;
    int n = en - st;
    T *aux = new T[n];
    for (int i = 0; i < n; i++) aux[i] = st[i];
    long long ret = count_inversions(aux, aux + n, st, en);
    delete[] (aux);
    return ret / 2;
}

template <typename It, typename Comparator> long long count_inversions(It src_st, It src_en, It dst_st, It dst_en, Comparator cmp) {
    int n = src_en - src_st;
    if (n <= 1) return 0;
    int mid = (n - 1) / 2;
    long long ret = 0;
    ret += count_inversions(dst_st, dst_st + mid + 1, src_st, src_st + mid + 1, cmp);
    ret += count_inversions(dst_st + mid + 1, dst_en, src_st + mid + 1, src_en, cmp);
    int i = 0, j = mid + 1;
    for (int k = 0; k < n; k++) {
        if (i > mid) {
            dst_st[k] = src_st[j++];
        } else if (j >= n) {
            dst_st[k] = src_st[i++];
            ret += j - (mid + 1);
        } else if (cmp(src_st[j], src_st[i])) {
            dst_st[k] = src_st[j++];
            ret += mid + 1 - i;
        } else {
            dst_st[k] = src_st[i++];
            ret += j - (mid + 1);
        }
    }
    return ret;
}

template <typename It, typename Comparator> long long count_inversions(It st, It en, Comparator cmp) {
    typedef typename std::iterator_traits<It>::value_type T;
    int n = en - st;
    T *aux = new T[n];
    for (int i = 0; i < n; i++) aux[i] = st[i];
    long long ret = count_inversions(aux, aux + n, st, en, cmp);
    delete[] (aux);
    return ret / 2;
}

#endif /* ALGORITHMS_SORT_COUNTINVERSIONS_H_ */
