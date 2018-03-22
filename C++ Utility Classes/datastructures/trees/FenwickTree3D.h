#ifndef DATASTRUCTURES_TREES_MATH_FENWICKTREE3D_H_
#define DATASTRUCTURES_TREES_MATH_FENWICKTREE3D_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Memory usage:  O(nmp)
 */
template <typename T>
struct FenwickTree3D {
private:
    int sizeX;
    int sizeY;
    int sizeZ;
    T ***array;

public:
    FenwickTree3D(int sizeX, int sizeY, int sizeZ) {
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->sizeZ = sizeZ;
        array = new T**[sizeX + 1];
        for (int i = 0; i <= sizeX; i++) {
            array[i] = new T*[sizeY + 1];
            for (int j = 0; j <= sizeY; j++) {
                array[i][j] = new T[sizeZ + 1];
                for (int k = 0; k <= sizeZ; k++) {
                     array[i][j][k] = 0;
                 }
            }
        }
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(nmp))
     *
     * @param  indX x index
     * @param  indY y index
     * @param  indZ z index
     * @return sum
     */
    T rsq(int indX, int indY, int indZ) {
        T sum = 0;
        for (int x = indX; x > 0; x -= (x & -x)) {
            for (int y = indY; y > 0; y -= (y & -y)) {
                for (int z = indZ; z > 0; z -= (z & -z)) {
                    sum += array[x][y][z];
                }
            }
        }
        return sum;
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(nmp))
     *
     * @param  x1 top index
     * @param  y1 left index
     * @param  z1 from index
     * @param  x2 bottom index
     * @param  y2 right index
     * @param  z2 back index
     * @return sum
     */
    T rsq(int x1, int y1, int z1, int x2, int y2, int z2) {
        return rsq(x2, y2, z2)
                - rsq(x1 - 1, y2, z2) - rsq(x2, y1 - 1, z2) - rsq(x2, y2, z1 - 1)
                + rsq(x1 - 1, y1 - 1, z2) + rsq(x1 - 1, y2, z1 - 1) + rsq(x2, y1 - 1, z1 - 1)
                - rsq(x1 - 1, y1 - 1, z1 - 1);
    }

    /**
     * Update the array at [indX][indY][indZ] and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(nmp))
     *
     * @param  indX   x index
     * @param  indY   y index
     * @param  indZ   z index
     * @param  value value
     */
    void update(int indX, int indY, int indZ, T value) {
        for (int x = indX; x <= sizeX; x += (x & -x)) {
            for (int y = indY; y <= sizeY; y += (y & -y)) {
                for (int z = indZ; z <= sizeZ; z += (z & -z)) {
                    array[x][y][z] += value;
                }
            }
        }
    }

    int getSizeX() {
        return sizeX;
    }

    int getSizeY() {
        return sizeY;
    }

    int getSizeZ() {
        return sizeZ;
    }
};

#endif /* DATASTRUCTURES_TREES_MATH_FENWICKTREE3D_H_ */
