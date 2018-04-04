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
template <typename It> void counting_sort(It st, It en) {
    int n = en - st;
    if (n == 0) return;
    int maxVal = INT_MIN, minVal = INT_MAX;
    for (It i = st; i < en; i++) {
        maxVal = max(maxVal, *i);
        minVal = min(minVal, *i);
    }
    int *count = new int[maxVal - minVal + 1];
    for (int i = 0; i <= maxVal - minVal; i++) count[i] = 0;
    for (It i = st; i < en; i++) count[*i - minVal]++;
    for (int i = 1; i <= maxVal - minVal; i++) count[i] += count[i - 1];
    int *b = new int[n];
    for (It i = st; i < en; i++) b[--count[*i - minVal]] = *i;
    for (It i = st; i < en; i++) *i = b[i - st];
    delete[] (count);
    delete[] (b);
}

#endif /* ALGORITHMS_SORT_COUNTINGSORT_H_ */
