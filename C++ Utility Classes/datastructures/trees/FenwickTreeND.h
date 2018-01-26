#ifndef DATASTRUCTURES_TREES_FENWICKTREEND_H_
#define DATASTRUCTURES_TREES_FENWICKTREEND_H_

#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTreeND {
private:
    int prod, n;
    vector<int> dim, sufProd;
    T *array;

    T rsq(vector<int> &ind, int curDim, int pos) {
        T sum = 0;
        if (curDim == n) {
            sum += array[pos];
        } else {
            for (int i = ind[curDim]; i > 0; i -= (i & -i)) {
                sum += rsq(ind, curDim + 1, pos + (i - 1) * sufProd[curDim]);
            }
        }
        return sum;
    }

    T rsq(vector<int> &start, vector<int> &end, int curDim, int pos) {
        T sum = 0;
        if (curDim == n) {
            sum += array[pos];
        } else {
            for (int i = end[curDim]; i > 0; i -= (i & -i)) {
                sum += rsq(start, end, curDim + 1, pos + (i - 1) * sufProd[curDim]);
            }
            for (int i = start[curDim] - 1; i > 0; i -= (i & -i)) {
                sum -= rsq(start, end, curDim + 1, pos + (i - 1) * sufProd[curDim]);
            }
        }
        return sum;
    }

    void update(vector<int> &ind, int curDim, int pos, T value) {
        if (curDim == n) {
            array[pos] += value;
        } else {
            for (int i = ind[curDim]; i <= dim[curDim]; i += (i & -i)) {
                update(ind, curDim + 1, pos + (i - 1) * sufProd[curDim], value);
            }
        }
    }

public:
    FenwickTreeND(vector<int> &dim) {
        prod = 1;
        n = (int) dim.size();
        for (int i = 0; i < n; i++) {
            this->dim.push_back(dim[i]);
        }
        for (int i = n - 1; i >= 0; i--) {
            sufProd.push_back(prod);
            prod *= dim[i];
        }
        reverse(sufProd.begin(), sufProd.end());
        array = new T[prod];
        for (int i = 0; i < prod; i++) {
            array[i] = 0;
        }
    }

    T rsq(vector<int> &ind) {
        return rsq(ind, 0, 0);
    }

    T rsq(vector<int> &start, vector<int> &end) {
        return rsq(start, end, 0, 0);
    }

    void update(vector<int> &ind, T value) {
        update(ind, 0, 0, value);
    }

    int getDimension(int d) {
        return dim[d];
    }
};

#endif /* DATASTRUCTURES_TREES_FENWICKTREEND_H_ */
