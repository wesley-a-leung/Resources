/*
 * LazySegmentTree.h
 *
 *  Created on: Jun 19, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_LAZYSEGMENTTREE_H_

#include <bits/stdc++.h>

#define l(x) x << 1
#define r(x) x << 1 | 1
#define mid(x, y) x + (y - x) / 2

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
            tree[l(cur)].maxVal += tree[cur].lazy;
            tree[l(cur)].lazy += tree[cur].lazy;
            tree[r(cur)].maxVal += tree[cur].lazy;
            tree[r(cur)].lazy += tree[cur].lazy;
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
        int m = mid(l, r);
        build(l(cur), l , m);
        build(r(cur), m + 1, r);
    }

    void update(int cur, int l, int r, int val) {
        if (tree[cur].l != tree[cur].r) propogate(cur);
        if (tree[cur].l > r || tree[cur].r < l) return;
        if (tree[cur].l >= l && tree[cur].r <= r) {
            tree[cur].maxVal += val;
            tree[cur].lazy += val;
            return;
        }
        update(l(cur), l, r, val);
        update(r(cur), l, r, val);
        tree[cur].maxVal = max(tree[l(cur)].maxVal, tree[r(cur)].maxVal);
    }

    int rMaxQ(int cur, int l, int r) {
        if (tree[cur].l != tree[cur].r) propogate(cur);
        if (tree[cur].l > r || tree[cur].r < l) return 0;
        if (tree[cur].l >= l && tree[cur].r <= r) return tree[cur].maxVal;;
        int left = rMaxQ(l(cur), l, r);
        int right = rMaxQ(r(cur), l, r);
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
