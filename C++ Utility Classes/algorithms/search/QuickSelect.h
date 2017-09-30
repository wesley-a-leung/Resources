#ifndef ALGORITHMS_SEARCH_QUICKSELECT_H_
#define ALGORITHMS_SEARCH_QUICKSELECT_H_

#include <bits/stdc++.h>

using namespace std;

/**
 *  The {@code QuickSelect} class provides static methods for selecting the
 *  Kth smallest element in an array
 *  <p>
 *  Best Case: <em>N</em>
 *  <p>
 *  Average Case: <em>N</em>
 *  <p>
 *  Worse Case: <em>N^2</em>
 *  <p>
 *  Space: 1
 *
 *  @author Wesley Leung
 */
template <typename Comparable>
class QuickSelect {
private:
    static int randomPivot(int start, int end) {
        return (rand() % ((end) - (start) + 1) + (start));
    }

    static int partition(Comparable *a, int start, int end, int pivotIndex) {
        Comparable pivotValue = a[pivotIndex];
        swap(a[pivotIndex], a[end]);
        int storeIndex = start;
        for (int i = start; i < end; i++) {
            if (a[i] < pivotValue) {
                swap(a[storeIndex], a[i]);
                storeIndex++;
            }
        }
        swap(a[end], a[storeIndex]);
        return storeIndex;
    }

public:
    /**
     * Returns the {@code K}th smallest element
     *
     * @param a the array
     * @param n the length of the array
     * @param k the index to find
     * @return the {@code K}th smallest element
     */
    static Comparable select(Comparable *a, int n, int k) {
        return select(a, 0, n - 1, k);
    }

    /**
     * Returns the {@code K}th smallest element
     *
     * @param a the array
     * @param start the start index
     * @param end the end index
     * @param k the index to find
     * @return the {@code K}th smallest element
     */
    static Comparable select(Comparable *a, int start, int end, int k) {
        srand(time(0));
        if (start == end) {
            return a[start];
        }

        for (;;) {
            int pivotIndex = randomPivot(start, end);
            pivotIndex = partition(a, start, end, pivotIndex);

            if (k == pivotIndex) {
                return a[k];
            } else if (k < pivotIndex) {
                end = pivotIndex - 1;
            } else {
                start = pivotIndex + 1;
            }
        }
    }
};

#endif /* ALGORITHMS_SEARCH_QUICKSELECT_H_ */
