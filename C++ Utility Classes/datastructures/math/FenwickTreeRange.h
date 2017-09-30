#ifndef DATASTRUCTURES_MATH_FENWICKTREERANGE_H_
#define DATASTRUCTURES_MATH_FENWICKTREERANGE_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * FenwickTree supporting range updates and range queries
 * Memory usage:  O(2n)
 *
 * @author Wesley Leung
 */
struct FenwickTreeRange {
private:
    int size;
    vector<int> array1, array2;

    int rsq(vector<int> &array, int ind) {
        int sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }

    void update(vector<int> &array, int ind, int value) {
        for (int x = ind; x <= size; x += (x & -x)) {
            array[x] += value;
        }
    }

public:
    FenwickTreeRange(int size) : array1(size + 1), array2(size + 2) {
        this->size = size;
    }

    /**
     * Range Sum query at index ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    int rsq(int ind) {
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
    int rsq(int a, int b) {
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
    void update(int a, int b, int value) {
        update(array1, a, value);
        update(array1, b + 1, -value);
        update(array2, a, value * (a - 1));
        update(array2, b + 1, -value * b);
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_MATH_FENWICKTREERANGE_H_ */
