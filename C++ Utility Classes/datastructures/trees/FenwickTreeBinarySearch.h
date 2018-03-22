#ifndef DATASTRUCTURES_TREES_FENWICKTREEBINARYSEARCH_H_
#define DATASTRUCTURES_TREES_FENWICKTREEBINARYSEARCH_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * FenwickTree supporting point updates and range queries, as well as
 * support for lower_bound and upper_bound binary searching the cumulative sum.
 * Memory usage:  O(n)
 *
 * @author Wesley Leung
 */
template <typename T>
struct FenwickTreeBinarySearch {
private:
    int size;
    int logSize;
    T *array;

public:
    FenwickTreeBinarySearch(int size) {
        this->size = size;
        this->logSize = 31 - __builtin_clz(size);
        array = new T[size + 1];
        for (int i = 0; i <= size; i++) {
            array[i] = 0;
        }
    }

    virtual ~FenwickTreeBinarySearch() {
        delete[](array);
    }

    /**
     * Range Sum query from 1 to ind
     * ind is 1-indexed
     *
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    T rsq(int ind) {
        T sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }

    /**
     * Range Sum Query from a to b.
     * Search for the sum from array index from a to b
     * a and b are 1-indexed
     *
     * Time-Complexity:    O(log(n))
     *
     * @param  a left index
     * @param  b right index
     * @return sum
     */
    T rsq(int a, int b) {
        return rsq(b) - rsq(a - 1);
    }

    /**
     * Update the array at ind and all the affected regions above ind.
     * ind is 1-indexed
     *
     * Time-Complexity:    O(log(n))
     *
     * @param  ind   index
     * @param  value value
     */
    void update(int ind, T value) {
        for (int x = ind; x <= size; x += (x & -x)) {
            array[x] += value;
        }
    }

    /**
     * Returns the smallest index with a cumulative sum that is greater than or equal to value.
     *
     * @param value value
     * @return the smallest index with a cumulative sum that is greater than or equal to value
     */
    int lower_bound(T value) {
        int ind = 0, i = 0;
        T sum = 0;
        for (int x = logSize; x >= 0; x--) {
            i = ind | (1 << x);
            if (i > size) continue;
            if (sum + array[i] < value) {
                ind = i;
                sum += array[i];
            }
        }
        return ind + 1;
    }

    /**
     * Returns the smallest index with a cumulative sum that is greater than value.
     *
     * @param value value
     * @return the smallest index with a cumulative sum that is greater than value
     */
    int upper_bound(T value) {
        int ind = 0, i = 0;
        T sum = 0;
        for (int x = logSize; x >= 0; x--) {
            i = ind | (1 << x);
            if (i > size) continue;
            if (sum + array[i] <= value) {
                ind = i;
                sum += array[i];
            }
        }
        return ind + 1;
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_TREES_FENWICKTREEBINARYSEARCH_H_ */
