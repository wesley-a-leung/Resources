#ifndef DATASTRUCTURES_TREES_MATH_FENWICKTREE2D_H_
#define DATASTRUCTURES_TREES_MATH_FENWICKTREE2D_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Memory usage:  O(nm)
 *
 * @author Wesley Leung
 */
template <typename T>
struct FenwickTree2D {
private:
    int sizeX;
    int sizeY;
    T **array;

public:
    FenwickTree2D(int sizeX, int sizeY) {
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        array = new T*[sizeX + 1];
        for (int i = 0; i <= sizeX; i++) {
            array[i] = new T[sizeY + 1];
            for (int j = 0; j <= sizeY; j++) {
                array[i][j] = 0;
            }
        }
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(nm))
     *
     * @param  indX x index
     * @param  indY y index
     * @return sum
     */
    T rsq(int indX, int indY) {
        T sum = 0;
        for (int x = indX; x > 0; x -= (x & -x)) {
            for (int y = indY; y > 0; y -= (y & -y)) {
                sum += array[x][y];
            }
        }
        return sum;
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(nm))
     *
     * @param  x1 top index
     * @param  y1 left index
     * @param  x2 bottom index
     * @param  y2 right index
     * @return sum
     */
    T rsq(int x1, int y1, int x2, int y2) {
        return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1);
    }

    /**
     * Update the array at [indX][indY] and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(nm))
     *
     * @param  indX   x index
     * @param  indY   y index
     * @param  value value
     */
    void update(int indX, int indY, T value) {
        for (int x = indX; x <= sizeX; x += (x & -x)) {
            for (int y = indY; y <= sizeY; y += (y & -y)) {
                array[x][y] += value;
            }
        }
    }

    int getSizeX() {
        return sizeX;
    }

    int getSizeY() {
        return sizeY;
    }
};

#endif /* DATASTRUCTURES_TREES_MATH_FENWICKTREE2D_H_ */
