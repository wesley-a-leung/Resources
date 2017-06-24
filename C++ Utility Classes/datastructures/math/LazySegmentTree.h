/*
 * LazySegmentTree.h
 *
 *  Created on: Jun 19, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct LazySegmentTree {
    struct Node {
        int val, lazy;
    };

private:
    vector<Node> tree;
    int N;
    int *array;

    void propogate(int cur) {
        if (tree[cur].lazy != 0) {
            tree[cur * 2].val += tree[cur].lazy;
            tree[cur * 2].lazy += tree[cur].lazy;
            tree[cur * 2 + 1].val += tree[cur].lazy;
            tree[cur * 2 + 1].lazy += tree[cur].lazy;
            tree[cur].lazy = 0;
        }
    }

    void build(int cur, int cL, int cR) {
        if (cL == cR) {
            tree[cur].val = array[cL];
            return;
        }
        int m = cL + (cR - cL) / 2;
        build(cur * 2, cL , m);
        build(cur * 2 + 1, m + 1, cR);
    }

    void update(int cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) propogate(cur);
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            tree[cur].val += val;
            tree[cur].lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, l, r, val);
        update(cur * 2 + 1, m + 1, cR, l, r, val);
        tree[cur].val = max(tree[cur * 2].val, tree[cur * 2 + 1].val);
    }

    int rMaxQ(int cur, int cL, int cR, int l, int r) {
        if (cL != cR) propogate(cur);
        if (cL > r || cR < l) return INT_MIN;
        if (cL >= l && cR <= r) return tree[cur].val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur * 2, cL, m, l, r);
        int right = rMaxQ(cur * 2 + 1, m + 1, cR, l, r);
        return max(left, right);
    }

public:
    LazySegmentTree(int size, int *arr, bool oneIndexed): tree(4 * size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - !oneIndexed];
        }
        build(1, 1, size);
        N = size;
    }

    LazySegmentTree(int size): tree(4 * size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = 0;
        }
        build(1, 1, size);
        N = size;
    }

    void update(int l, int r, int val) {
        update(1, 1, N, l, r, val);
    }

    int rMaxQ(int l, int r) {
        return rMaxQ(1, 1, N, l, r);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_ */
