#ifndef DATASTRUCTURES_TREES_FENWICKTREEQUADRATIC_H_
#define DATASTRUCTURES_TREES_FENWICKTREEQUADRATIC_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * FenwickTree supporting range updates with updates in the form of
 * adding v, 2v, 3v, ... to the interval [l, r], and range queries.
 * Memory usage:  O(3n)
 *
 * @author Wesley Leung
 */
template <typename T>
struct FenwickTreeQuadratic {
private:
    int size;
    // constant, linear, quadratic
    vector<T> con, lin, quad;

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
    FenwickTreeQuadratic(int size) : con(size + 1), lin(size + 1), quad(size + 1) {
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
    T rsq(int ind) {
        return (rsq(quad, ind) * ind * ind + rsq(lin, ind) * ind + rsq(con, ind)) / 2;
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
     * Update the array between index a and b with values v, 2v, 3v, ...
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  a left index
     * @param  b right index
     * @param  value value
     */
    void update(int a, int b, T value) {
        int s = a - 1, len = b - a + 1;
        update(quad, a, value);
        update(quad, b + 1, -value);
        update(lin, a, value * (1 - 2 * s));
        update(lin, b + 1, -value * (1 - 2 * s));
        update(con, a, value * (s * s - s));
        update(con, b + 1, -value * ((s * s - s) - len * (len + 1)));
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_TREES_FENWICKTREEQUADRATIC_H_ */
