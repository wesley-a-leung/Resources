#ifndef DATASTRUCTURES_TREES_FENWICKTREEMAX_H_
#define DATASTRUCTURES_TREES_FENWICKTREEMAX_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * FenwickTree supporting point updates and range queries
 * for A[i] = max(A[i], val) and querying max A[1 ... i].
 * Memory usage:  O(n)
 */
template <typename T>
struct FenwickTree {
private:
    int size;
    vector<int> array;

public:
    FenwickTree(int size) : size(size), array(size + 1, 0) {}

    /**
     * Range Max query from 1 to ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return maxVal
     */
    T rmq(int ind) {
        T maxVal = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            maxVal = max(maxVal, array[x]);
        }
        return maxVal;
    }

    /**
     * Update the array at ind to max(A[ind], val) and all the
     * affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind   index
     * @param  value value
     */
    void update(int ind, T value) {
        for (int x = ind; x <= size; x += (x & -x)) {
            array[x] = max(array[x], value);
        }
    }

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_TREES_FENWICKTREEMAX_H_ */
