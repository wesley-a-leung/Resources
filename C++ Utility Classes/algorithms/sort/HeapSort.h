#ifndef ALGORITHMS_SORT_HEAPSORT_H_
#define ALGORITHMS_SORT_HEAPSORT_H_

#include <bits/stdc++.h>
using namespace std;

template <typename It> void heap_sort(It st, It en) {
    int n = en - st;
    for (int i = n / 2; i >= 1; i--) {
        int k = i;
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && st[j - 1] < st[j]) j++;
            if (st[k - 1] >= st[j - 1]) break;
            swap(st[k - 1], st[j - 1]);
            k = j;
        }
    }
    while (n > 1) {
        swap(st[0], st[n - 1]);
        n--;
        int k = 1;
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && st[j - 1] < st[j]) j++;
            if (st[k - 1] >= st[j - 1]) break;
            swap(st[k - 1], st[j - 1]);
            k = j;
        }
    }
}

template <typename It, typename Comparator> void heap_sort(It st, It en, Comparator cmp) {
    int n = en - st;
    for (int i = n / 2; i >= 1; i--) {
        int k = i;
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && cmp(st[j - 1], st[j])) j++;
            if (!cmp(st[k - 1], st[j - 1])) break;
            swap(st[k - 1], st[j - 1]);
            k = j;
        }
    }
    while (n > 1) {
        swap(st[0], st[n - 1]);
        n--;
        int k = 1;
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && cmp(st[j - 1], st[j])) j++;
            if (!cmp(st[k - 1], st[j - 1])) break;
            swap(st[k - 1], st[j - 1]);
            k = j;
        }
    }
}

#endif /* ALGORITHMS_SORT_HEAPSORT_H_ */
