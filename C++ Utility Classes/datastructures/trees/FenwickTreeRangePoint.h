#ifndef DATASTRUCTURES_TREES_MATH_FENWICKTREERANGEPOINT_H_
#define DATASTRUCTURES_TREES_MATH_FENWICKTREERANGEPOINT_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * FenwickTree supporting range updates and point queries
 * Memory usage:  O(n)
 *
 * @author Wesley Leung
 */
template <typename T>
struct FenwickTreeRangePoint {
private:
    int size;
    T *array;

public:
    FenwickTreeRangePoint(int size) {
        this->size = size;
        array = new T[size + 1];
        for (int i = 0; i <= size; i++) {
            array[i] = 0;
        }
    }

    /**
     * Value at index {@code ind}.
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    T getValue(int ind) {
        T sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }

    /**
     * Update the array at ind and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
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
     * Update the array between a and b.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  a left index
     * @param  b right index
     * @param  value value
     */
    void update(int a, int b, T value) {
        update(a, value);
        update(b + 1, -value);
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_TREES_MATH_FENWICKTREERANGEPOINT_H_ */
