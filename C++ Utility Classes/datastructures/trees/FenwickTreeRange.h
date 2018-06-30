#ifndef DATASTRUCTURES_TREES_MATH_FENWICKTREERANGE_H_
#define DATASTRUCTURES_TREES_MATH_FENWICKTREERANGE_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * FenwickTree supporting range updates and range queries.
 * Memory usage:  O(2n)
 */
template <typename T>
struct FenwickTreeRange {
private:
    int size;
    vector<T> array1, array2;

    T rsq(vector<T> &array, int ind) {
        T sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }

    void update(vector<T> &array, int ind, T value) {
        for (int x = ind; x <= size; x += (x & -x)) {
            array[x] += value;
        }
    }

public:
    FenwickTreeRange(int size) : size(size), array1(size + 1), array2(size + 1) {}

    /**
     * Range Sum query at index ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    T rsq(int ind) {
        return rsq(array1, ind) * ind - rsq(array2, ind);
    }

    /**
     * Range Sum Query from a to b.
     * Search for the sum from array index from a to b
     * a and b are 1-indexed
     * <p>
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
     * Update the array between index a and b
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  a left index
     * @param  b right index
     * @param  value value
     */
    void update(int a, int b, T value) {
        update(array1, a, value);
        update(array1, b + 1, -value);
        update(array2, a, value * ((T) a - 1));
        update(array2, b + 1, -value * (T) b);
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_TREES_MATH_FENWICKTREERANGE_H_ */
