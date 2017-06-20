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
        int l, r, maxVal, lazy;
    };

private:
    vector<Node> tree;
    int N;
    int *array;

    void propogate(int cur) {
        if (tree[cur].lazy != 0) {
            tree[cur * 2].maxVal += tree[cur].lazy;
            tree[cur * 2].lazy += tree[cur].lazy;
            tree[cur * 2 + 1].maxVal += tree[cur].lazy;
            tree[cur * 2 + 1].lazy += tree[cur].lazy;
            tree[cur].lazy = 0;
        }
    }

    void build(int cur, int l, int r) {
        tree[cur].l = l;
        tree[cur].r = r;
        if (l == r) {
            tree[cur].maxVal = array[l];
            return;
        }
        int m = l + (r - l) / 2;
        build(cur * 2, l , m);
        build(cur * 2 + 1, m + 1, r);
    }

    void update(int cur, int l, int r, int val) {
        if (tree[cur].l != tree[cur].r) propogate(cur);
        if (tree[cur].l > r || tree[cur].r < l) return;
        if (tree[cur].l >= l && tree[cur].r <= r) {
            tree[cur].maxVal += val;
            tree[cur].lazy += val;
            return;
        }
        update(cur * 2, l, r, val);
        update(cur * 2 + 1, l, r, val);
        tree[cur].maxVal = max(tree[cur * 2].maxVal, tree[cur * 2 + 1].maxVal);
    }

    int rMaxQ(int cur, int l, int r) {
        if (tree[cur].l != tree[cur].r) propogate(cur);
        if (tree[cur].l > r || tree[cur].r < l) return 0;
        if (tree[cur].l >= l && tree[cur].r <= r) return tree[cur].maxVal;;
        int left = rMaxQ(cur * 2, l, r);
        int right = rMaxQ(cur * 2 + 1, l, r);
        return max(left, right);
    }

public:
    LazySegmentTree(int size, int *arr): tree(4 * size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - 1];
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
        update(1, l, r, val);
    }

    int rMaxQ(int l, int r) {
        return rMaxQ(1, l, r);
    }
};

#endif /* DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_ */
