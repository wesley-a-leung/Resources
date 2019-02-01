#pragma once
#include <bits/stdc++.h>
using namespace std;

// N dimensional Fenwick Tree
// Time Complexity:
//   constructor: O(product of dims)
//   update: O(product of logs of dims)
//   rsq: O(2^N * (product of logs of dims))
// Memory Complexity: O(product of dims)
template <typename T> struct FenwickTreeND {
    int prod, n; vector<int> dim, sufProd; vector<T> BIT;
    T rsq(const vector<int> &ind, int curDim, int pos) {
        T sum = 0;
        if (curDim == n) sum += BIT[pos];
        else for (int i = ind[curDim]; i > 0; i -= i & -i) sum += rsq(ind, curDim + 1, pos + (i - 1) * sufProd[curDim]);
        return sum;
    }
    T rsq(const vector<int> &start, const vector<int> &end, int curDim, int pos) {
        T sum = 0;
        if (curDim == n) sum += BIT[pos];
        else {
            for (int i = end[curDim]; i > 0; i -= i & -i) sum += rsq(start, end, curDim + 1, pos + (i - 1) * sufProd[curDim]);
            for (int i = start[curDim] - 1; i > 0; i -= i & -i) sum -= rsq(start, end, curDim + 1, pos + (i - 1) * sufProd[curDim]);
        }
        return sum;
    }
    void update(const vector<int> &ind, int curDim, int pos, T value) {
        if (curDim == n) BIT[pos] += value;
        else for (int i = ind[curDim]; i <= dim[curDim]; i += i & -i) update(ind, curDim + 1, pos + (i - 1) * sufProd[curDim], value);
    }
    FenwickTreeND(const vector<int> &dim) : prod(1), n(dim.size()), dim(dim) {
        for (int i = n - 1; i >= 0; i--) { sufProd.push_back(prod); prod *= dim[i]; }
        reverse(sufProd.begin(), sufProd.end());
        for (int i = 0; i < prod; i++) BIT.push_back(0);
    }
    // 1-indexed
    T rsq(const vector<int> &ind) { return rsq(ind, 0, 0); }
    T rsq(const vector<int> &start, const vector<int> &end) { return rsq(start, end, 0, 0); }
    void update(const vector<int> &ind, T value) { update(ind, 0, 0, value); }
};
