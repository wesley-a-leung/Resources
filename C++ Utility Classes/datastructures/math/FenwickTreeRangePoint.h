/*
 * FenwickTreeRangePoint.h
 *
 *  Created on: Jul 26, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_FENWICKTREERANGEPOINT_H_
#define DATASTRUCTURES_MATH_FENWICKTREERANGEPOINT_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * FenwickTree supporting range updates and point queries
 * Memory usage:  O(n)
 *
 * @author Wesley Leung
 */
struct FenwickTreeRangePoint {
private:
    int size;
    int *array;

public:
    FenwickTreeRangePoint(int size) {
        this->size = size;
        array = new int[size + 1];
        for (int i = 0; i <= size; i++) {
            array[i] = 0;
        }
    }

    /**
     * Range Sum query from 1 to ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    int rsq(int ind) {
        int sum = 0;
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
    void update(int ind, int value) {
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
    void update(int a, int b, int value) {
        update(a, value);
        update(b + 1, -value);
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_MATH_FENWICKTREERANGEPOINT_H_ */
