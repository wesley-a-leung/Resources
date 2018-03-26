#ifndef DATASTRUCTURES_TREES_DYNAMICLAZYSEGMENTTREE_H_
#define DATASTRUCTURES_TREES_DYNAMICLAZYSEGMENTTREE_H_

#include <bits/stdc++.h>
using namespace std;

struct DynamicLazySegmentTree {
private:
    static const int vdef = 0, ldef = 0, qdef = 0;

    static int merge(int l, int r) {
        return l + r; // sum
    }

    static int apply(int x, int v) {
        return v; // assignment
    }

    static int getSegmentVal(int x, int len) {
        return x * len; // sum
    }

    struct Node {
        Node *left = nullptr;
        Node *right = nullptr;
        int val = DynamicLazySegmentTree::vdef, lazy = DynamicLazySegmentTree::ldef;
    };

    void propogate(Node *cur, int cL, int cR) {
        if (cur->lazy != ldef) {
            int m = cL + (cR - cL) / 2;
            if (cur->left == nullptr) cur->left = new Node();
            cur->left->val = apply(cur->left->val, getSegmentVal(cur->lazy, m - cL + 1));
            cur->left->lazy = apply(cur->left->lazy, cur->lazy);
            if (cur->right == nullptr) cur->right = new Node();
            cur->right->val = apply(cur->right->val, getSegmentVal(cur->lazy, cR - m));
            cur->right->lazy = apply(cur->right->lazy, cur->lazy);
            cur->lazy = ldef;
        }
    }

    Node *root;
    int N;

    void update(Node *cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) propogate(cur, cL, cR);
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            cur->val = apply(cur->val, getSegmentVal(val, cR - cL + 1));
            cur->lazy = apply(cur->lazy, val);
            return;
        }
        int m = cL + (cR - cL) / 2;
        if (cur->left == nullptr) cur->left = new Node();
        update(cur->left, cL, m, l, r, val);
        if (cur->right == nullptr) cur->right = new Node();
        update(cur->right, m + 1, cR, l, r, val);
        cur->val = merge(cur->left->val, cur->right->val);
    }

    int query(Node *cur, int cL, int cR, int l, int r) {
        if (cur == nullptr || cL > r || cR < l) return qdef;
        if (cL != cR) propogate(cur, cL, cR);
        if (cL >= l && cR <= r) return cur->val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur->left, cL, m, l, r);
        int right = query(cur->right, m + 1, cR, l, r);
        return merge(left, right);
    }

public:
    DynamicLazySegmentTree(int size) {
        root = new Node();
        N = size;
    }

    void update(int l, int r, int val) {
        update(root, 1, N, l, r, val);
    }

    int query(int l, int r) {
        return query(root, 1, N, l, r);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_TREES_DYNAMICLAZYSEGMENTTREE_H_ */
