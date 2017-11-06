#ifndef ALGORITHMS_SORT_COUNTINGSORT_H_
#define ALGORITHMS_SORT_COUNTINGSORT_H_

#include <bits/stdc++.h>

using namespace std;

/**
 *  Sorts an array using counting sort. This sorting algorithm only works with integer values.
 *  Best Case: <em>N</em> + <em>K</em>
 *  Average Case: <em>N</em> + <em>K</em>
 *  Worse Case: <em>N</em> + <em>K</em>
 *  Space: <em>N</em> + <em>K</em>
 *
 *  @author Wesley Leung
 */

/**
 * Rearranges the array in ascending order, using the natural order.
 *
 * @param a the array to be sorted
 * @param n the length of the array
 */
void sort(int *a, int n) {
    int maxVal = 0;
    for (int i = 0; i < n; i++) maxVal = max(maxVal, a[i]);
    int count[maxVal + 1];
    for (int i = 0; i < n; i++) count[a[i]]++;
    for (int i = 1; i <= maxVal; i++) count[i] += count[i - 1];
    int b[n];
    for (int i = 0; i < n; i++) b[--count[a[i]]] = a[i];
    for (int i = 0; i < n; i++) a[i] = b[i];
}

#endif /* ALGORITHMS_SORT_COUNTINGSORT_H_ */
