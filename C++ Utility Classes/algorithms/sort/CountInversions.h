#ifndef ALGORITHMS_SORT_COUNTINVERSIONS_H_
#define ALGORITHMS_SORT_COUNTINVERSIONS_H_

#include <bits/stdc++.h>

using namespace std;

long long cnt; // number of inversions

// merges and counts inversions
void merge(int *a, int *aux, int lo, int mid, int hi) {
    for (int k = lo; k <= hi; k++) {
        aux[k] = a[k];
    }
    int i = lo, j = mid + 1;
    for (int k = lo; k <= hi; k++) {
        if (i > mid) { // no inversions if the lower half is completed
            a[k] = aux[j++];
        } else if (j > hi) { // if the upper half is completed, the number of inversions
                             // is equal to the size of the upper half
            a[k] = aux[i++];
            cnt += j - (mid + 1);
        } else if (aux[i] <= aux[j]) { // if the lower half element is smaller, the number of inversions
                                       // is equal to the number of elements taken in the upper half
            a[k] = aux[i++];
            cnt += j - (mid + 1);
        } else {                       // if the upper half element is smaller, the number of inversions
                                       // is equal to the number of elements not taken in the lower half
            a[k] = aux[j++];
            cnt += mid + 1 - i;
        }
    }
}

void countInversions(int *a, int *aux, int lo, int hi) {
    if (lo >= hi) return; // base case is interval of 1; if lo is greater than or equal to hi,
                          // then the current interval is less than 1
    int mid = lo + (hi - lo) / 2; // to prevent integer overflow
    // recursively divides into 2 subarrays
    countInversions(a, aux, lo, mid);
    countInversions(a, aux, mid + 1, hi);
    // merges subarrays and counts inversions
    merge(a, aux, lo, mid, hi);
}

/**
 * Counts the number of inversions in an array.
 *
 * @param a the array
 * @param n the length of the array
 * @return the number of inversions in {@code a}
 */
long long countInversions(int *a, int n) {
    int *aux = new int[n];
    cnt = 0;
    countInversions(a, aux, 0, n - 1);
    return cnt / 2; // since each inversion is counted twice
}

#endif /* ALGORITHMS_SORT_COUNTINVERSIONS_H_ */
