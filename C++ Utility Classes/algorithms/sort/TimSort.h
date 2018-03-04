#ifndef ALGORITHMS_SORT_TIMSORT_H_
#define ALGORITHMS_SORT_TIMSORT_H_

#include <bits/stdc++.h>
using namespace std;

const int INSERTION_SORT_CUTOFF = 7;

template <typename T> inline void exch(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template <typename It> void tim_sort(It st, It en) {
    typedef typename std::iterator_traits<It>::value_type T;
    int n = en - st;
    T *aux = new T[n];
    for (int i = 0; i < n; i++) aux[i] = st[i];
    bool flag = true;
    int lo, mid, hi;
    do {
        lo = mid = hi = 0;
        if (flag) {
            while (hi < n) {
                mid = lo + 1;
                while (mid < n && st[mid] >= st[mid - 1]) mid++;
                if (mid == n) {
                    hi = n;
                    break;
                }
                hi = mid + 1;
                while (hi < n && st[hi] >= st[hi - 1]) hi++;
                if (mid < n && aux[mid] >= aux[mid - 1]) {
                    for (int i = lo; i < hi; i++) st[i] = aux[i];
                } else if (hi - lo <= INSERTION_SORT_CUTOFF) {
                    for (int i = lo; i < hi; i++) st[i] = aux[i];
                    for (int i = lo; i < hi; i++) {
                        for (int j = i; j > lo && st[j] < st[j - 1]; j--) {
                            exch(st + j, st + j - 1);
                        }
                    }
                } else {
                    int i = lo, j = mid;
                    for (int k = lo; k < hi; k++) {
                        if (i >= mid) st[k] = aux[j++];
                        else if (j >= hi) st[k] = aux[i++];
                        else if (aux[j] < aux[i]) st[k] = aux[j++];
                        else st[k] = aux[i++];
                    }
                }
                if (hi < n) lo = hi;
            }
        } else {
            while (hi < n) {
                mid = lo + 1;
                while (mid < n && aux[mid] >= aux[mid - 1]) mid++;
                if (mid == n) {
                    hi = n;
                    break;
                }
                hi = mid + 1;
                while (hi < n && aux[hi] >= aux[hi - 1]) hi++;
                if (mid < n && st[mid] >= st[mid - 1]) {
                    for (int i = lo; i < hi; i++) aux[i] = st[i];
                } else if (hi - lo <= INSERTION_SORT_CUTOFF) {
                    for (int i = lo; i < hi; i++) aux[i] = st[i];
                    for (int i = lo; i < hi; i++) {
                        for (int j = i; j > lo && aux[j] < aux[j - 1]; j--) {
                            exch(aux + j, aux + j - 1);
                        }
                    }
                } else {
                    int i = lo, j = mid;
                    for (int k = lo; k < hi; k++) {
                        if (i >= mid) aux[k] = st[j++];
                        else if (j >= hi) aux[k] = st[i++];
                        else if (st[j] < st[i]) aux[k] = st[j++];
                        else aux[k] = st[i++];
                    }
                }
                if (hi < n) lo = hi;
            }
        }
        flag = !flag;
    } while (hi - lo < n);
    if (flag) for (int i = 0; i < n; i++) st[i] = aux[i];
    delete[] (aux);
}

template <typename It, class Comparator> void tim_sort(It st, It en, Comparator cmp) {
    typedef typename std::iterator_traits<It>::value_type T;
    int n = en - st;
    T *aux = new T[n];
    for (int i = 0; i < n; i++) aux[i] = st[i];
    bool flag = true;
    int lo, mid, hi;
    do {
        lo = mid = hi = 0;
        if (flag) {
            while (hi < n) {
                mid = lo + 1;
                while (mid < n && !cmp(st[mid], st[mid - 1])) mid++;
                if (mid == n) {
                    hi = n;
                    break;
                }
                hi = mid + 1;
                while (hi < n && !cmp(st[hi], st[hi - 1])) hi++;
                if (mid < n && !cmp(aux[mid], aux[mid - 1])) {
                    for (int i = lo; i < hi; i++) st[i] = aux[i];
                } else if (hi - lo <= INSERTION_SORT_CUTOFF) {
                    for (int i = lo; i < hi; i++) st[i] = aux[i];
                    for (int i = lo; i < hi; i++) {
                        for (int j = i; j > lo && cmp(st[j], st[j - 1]); j--) {
                            exch(st + j, st + j - 1);
                        }
                    }
                } else {
                    int i = lo, j = mid;
                    for (int k = lo; k < hi; k++) {
                        if (i >= mid) st[k] = aux[j++];
                        else if (j >= hi) st[k] = aux[i++];
                        else if (cmp(aux[j], aux[i])) st[k] = aux[j++];
                        else st[k] = aux[i++];
                    }
                }
                if (hi < n) lo = hi;
            }
        } else {
            while (hi < n) {
                mid = lo + 1;
                while (mid < n && !cmp(aux[mid], aux[mid - 1])) mid++;
                if (mid == n) {
                    hi = n;
                    break;
                }
                hi = mid + 1;
                while (hi < n && !cmp(aux[hi], aux[hi - 1])) hi++;
                if (mid < n && !cmp(st[mid], st[mid - 1])) {
                    for (int i = lo; i < hi; i++) aux[i] = st[i];
                } else if (hi - lo <= INSERTION_SORT_CUTOFF) {
                    for (int i = lo; i < hi; i++) aux[i] = st[i];
                    for (int i = lo; i < hi; i++) {
                        for (int j = i; j > lo && cmp(aux[j], aux[j - 1]); j--) {
                            exch(aux + j, aux + j - 1);
                        }
                    }
                } else {
                    int i = lo, j = mid;
                    for (int k = lo; k < hi; k++) {
                        if (i >= mid) aux[k] = st[j++];
                        else if (j >= hi) aux[k] = st[i++];
                        else if (cmp(st[j], st[i])) aux[k] = st[j++];
                        else aux[k] = st[i++];
                    }
                }
                if (hi < n) lo = hi;
            }
        }
        flag = !flag;
    } while (hi - lo < n);
    if (flag) for (int i = 0; i < n; i++) st[i] = aux[i];
    delete[] (aux);
}

#endif /* ALGORITHMS_SORT_TIMSORT_H_ */
