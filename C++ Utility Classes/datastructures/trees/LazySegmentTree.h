#ifndef DATASTRUCTURES_TREES_LAZYSEGMENTTREE_H_
#define DATASTRUCTURES_TREES_LAZYSEGMENTTREE_H_

#include <bits/stdc++.h>
using namespace std;

struct LazySegmentTree {
private:
    static const int vdef = 0, ldef = 0, qdef = 0;

    static int merge(int l, int r) {
        return l + r; // sum
    }

    static int apply(int x, int v) {
        return x + v; // increment
    }

    static int getSegmentVal(int x, int len) {
        return x * len; // sum
    }

    struct Node {
        int val = LazySegmentTree::vdef, lazy = LazySegmentTree::ldef;
    };

    vector<Node> tree;
    int N;
    int *array;

    void propagate(int cur, int cL, int cR) {
        if (tree[cur].lazy != ldef) {
            int m = cL + (cR - cL) / 2;
            tree[cur * 2].val = apply(tree[cur * 2].val, getSegmentVal(tree[cur].lazy, m - cL + 1));
            tree[cur * 2].lazy = apply(tree[cur * 2].lazy, tree[cur].lazy);
            tree[cur * 2 + 1].val = apply(tree[cur * 2 + 1].val, getSegmentVal(tree[cur].lazy, cR - m));
            tree[cur * 2 + 1].lazy = apply(tree[cur * 2 + 1].lazy, tree[cur].lazy);
            tree[cur].lazy = ldef;
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
        tree[cur].val = merge(tree[cur * 2].val, tree[cur * 2 + 1].val);
    }

    void update(int cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) propagate(cur, cL, cR);
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            tree[cur].val = apply(tree[cur].val, getSegmentVal(val, cR - cL + 1));
            tree[cur].lazy = apply(tree[cur].lazy, val);
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, l, r, val);
        update(cur * 2 + 1, m + 1, cR, l, r, val);
        tree[cur].val = merge(tree[cur * 2].val, tree[cur * 2 + 1].val);
    }

    int query(int cur, int cL, int cR, int l, int r) {
        if (cL != cR) propagate(cur, cL, cR);
        if (cL > r || cR < l) return qdef;
        if (cL >= l && cR <= r) return tree[cur].val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur * 2, cL, m, l, r);
        int right = query(cur * 2 + 1, m + 1, cR, l, r);
        return merge(left, right);
    }

public:
    LazySegmentTree(int size, int *arr, bool oneIndexed): tree((int) (2 * pow(2.0, ceil(log2((double) size))))) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - !oneIndexed];
        }
        build(1, 1, size);
        N = size;
    }

    LazySegmentTree(int size): tree((int) (2 * pow(2.0, ceil(log2((double) size))))) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = vdef;
        }
        build(1, 1, size);
        N = size;
    }

    void update(int l, int r, int val) {
        update(1, 1, N, l, r, val);
    }

    int query(int l, int r) {
        return query(1, 1, N, l, r);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_TREES_LAZYSEGMENTTREE_H_ */
