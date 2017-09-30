#ifndef ALGORITHMS_SEARCH_INTERPOLATIONSEARCH_H_
#define ALGORITHMS_SEARCH_INTERPOLATIONSEARCH_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Returns the index of the specified key in the specified array.
 *
 * @param  a the array of integers, must be sorted in ascending order
 * @param  n the length of the array
 * @param  key the search key
 * @return index of key in array {@code a} if present; {@code -1} otherwise
 */
int indexOf(int *a, int n, int key) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        // Key is in a[lo..hi] or not present.
        int guess = ((key - a[lo]) / (a[hi - lo])) * (hi - lo) + lo;
        if      (key < a[guess]) hi = guess - 1;
        else if (key > a[guess]) lo = guess + 1;
        else return guess;
    }
    return -1;
}

#endif /* ALGORITHMS_SEARCH_INTERPOLATIONSEARCH_H_ */
