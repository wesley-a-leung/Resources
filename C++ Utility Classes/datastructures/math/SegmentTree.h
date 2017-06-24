/*
 * SegmentTree.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct SegmentTree {
    struct Node {
        int val, lazy;
    };

private:
    vector<Node> tree;
    int N;
    int *array;

    void build(int cur, int cL, int cR) {
        if (cL == cR) {
            tree[cur].val = array[cL];
            return;
        }
        int m = cL + (cR - cL) / 2;
        build(cur * 2, cL , m);
        build(cur * 2 + 1, m + 1, cR);
    }

    void update(int cur, int cL, int cR, int ind, int val) {
        if (cL > ind || cR < ind) return;
        if (cL >= ind && cR <= ind) {
            tree[cur].val += val;
            tree[cur].lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, ind, val);
        update(cur * 2 + 1, m + 1, cR, ind, val);
        tree[cur].val = max(tree[cur * 2].val, tree[cur * 2 + 1].val);
    }

    int rMaxQ(int cur, int cL, int cR, int ind) {
        if (cL > ind || cR < ind) return INT_MIN;
        if (cL >= ind && cR <= ind) return tree[cur].val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur * 2, cL, m, ind);
        int right = rMaxQ(cur * 2 + 1, m + 1, cR, ind);
        return max(left, right);
    }

public:
    SegmentTree(int size, int *arr): tree(4 * size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - 1];
        }
        build(1, 1, size);
        N = size;
    }

    SegmentTree(int size): tree(4 * size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = 0;
        }
        build(1, 1, size);
        N = size;
    }

    void update(int ind, int val) {
        update(1, 1, N, ind, val);
    }

    int rMaxQ(int ind) {
        return rMaxQ(1, 1, N, ind);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_ */
