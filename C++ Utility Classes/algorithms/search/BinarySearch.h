#ifndef ALGORITHMS_SEARCH_BINARYSEARCH_H_
#define ALGORITHMS_SEARCH_BINARYSEARCH_H_

#include <bits/stdc++.h>
using namespace std;

template <typename Comparable>
class BinarySearch {
public:
    /**
     * Returns the index of the specified key in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of key in array {@code a} if present; {@code -1} otherwise
     */
    static int indexOf(Comparable *a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end - 1;
        int mid;
        while (lo <= hi) {
            // Key is in a[lo..hi] or not present.
            mid = lo + (hi - lo) / 2;
            if      (key < a[mid]) hi = mid - 1;
            else if (key > a[mid]) lo = mid + 1;
            else return mid;
        }
        return -1;
    }

    /**
     * Returns the index of the first element that is not less than the key
     * in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of first element not less than key in array {@code a};
     * {@code end} if key is larger than the largest element
     */
    static int lowerBound(Comparable *a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end;
        int mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (key <= a[mid]) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }

    /**
     * Returns the index of the first element that is greater than the key
     * in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of first element greater than key in array {@code a};
     * {@code end} if key is larger than or equal to the largest element
     */
    static int upperBound(Comparable *a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end;
        int mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (key >= a[mid]) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    /**
     * Returns the index of the largest element that is less than or equal to the key
     * in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of largest element less than or equal to the key in array {@code a};
     * {@code start} - 1 if no such element exists
     */
    static int floor(Comparable *a, int start, int end, Comparable key) {
        int lo = start, hi = end - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (key < a[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return hi;
    }

    /**
     * Returns the index of the smallest element that is greater than or equal to the key
     * in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of smallest element greater than or equal to the key in array {@code a};
     * {@code end} if no such element exists
     */
    static int ceiling(Comparable *a, int start, int end, Comparable key) {
        int lo = start, hi = end, mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (key > a[mid]) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    /**
     * Returns a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value
     */
    static pair<int, int> equalRange(Comparable *a, int start, int end, Comparable key) {
        return make_pair(lowerBound(a, start, end, key), upperBound(a, start, end, key));
    }
};

#endif /* ALGORITHMS_SEARCH_BINARYSEARCH_H_ */
