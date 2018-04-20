#ifndef ALGORITHMS_SORT_QUICKSORTDUAL_H_
#define ALGORITHMS_SORT_QUICKSORTDUAL_H_

#include <bits/stdc++.h>
using namespace std;

const int INSERTION_SORT_CUTOFF = 8;

template <typename It> void quick_sort_dual(It st, It en) {
    int n = en - st;
    if (n <= 1) return;
    if (n <= INSERTION_SORT_CUTOFF) { // insertion sort
        for (int i = 0; i < n; i++) {
            for (int j = i; j > 0 && st[j] < st[j - 1]; j--) {
                swap(st[j], st[j - 1]);
            }
        }
        return;
    }
    if (st[n - 1] < st[0]) swap(st[0], st[n - 1]);
    int lt = 1, gt = n - 2, i = 1;
    while (i <= gt) {
        if (st[i] < st[0]) swap(st[i++], st[lt++]);
        else if (st[n - 1] < st[i]) swap(st[i], st[gt--]);
        else i++;
    }
    swap(st[0], st[--lt]);
    swap(st[n - 1], st[++gt]);
    sort(st, st + lt);
    if (st[lt] < st[gt]) sort(st + lt + 1, st + gt);
    sort(st + gt + 1, en);
}

template <typename It, typename Comparator> void quick_sort_dual(It st, It en, Comparator cmp) {
    int n = en - st;
    if (n <= 1) return;
    if (n <= INSERTION_SORT_CUTOFF) { // insertion sort
        for (int i = 0; i < n; i++) {
            for (int j = i; j > 0 && cmp(st[j], st[j - 1]); j--) {
                swap(st[j], st[j - 1]);
            }
        }
        return;
    }
    if (cmp(st[n - 1], st[0])) swap(st[0], st[n - 1]);
    int lt = 1, gt = n - 2, i = 1;
    while (i <= gt) {
        if (cmp(st[i], st[0])) swap(st[i++], st[lt++]);
        else if (cmp(st[n - 1], st[i])) swap(st[i], st[gt--]);
        else i++;
    }
    swap(st[0], st[--lt]);
    swap(st[n - 1], st[++gt]);
    sort(st, st + lt);
    if (cmp(st[lt], st[gt])) sort(st + lt + 1, st + gt);
    sort(st + gt + 1, en);
}

#endif /* ALGORITHMS_SORT_QUICKSORTDUAL_H_ */
