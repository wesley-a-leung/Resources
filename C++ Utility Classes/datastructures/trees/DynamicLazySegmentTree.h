#ifndef DATASTRUCTURES_TREES_MATH_DYNAMICLAZYSEGMENTTREE_H_
#define DATASTRUCTURES_TREES_MATH_DYNAMICLAZYSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct DynamicLazySegmentTree {
    struct Node {
        Node *left = nullptr;
        Node *right = nullptr;
        int val = 0, lazy = 0;

        void propogate() {
            if (this->lazy != 0) {
                if (left == nullptr) left = new Node();
                left->val += this->lazy;
                left->lazy += this->lazy;
                if (right == nullptr) right = new Node();
                right->val += this->lazy;
                right->lazy += this->lazy;
                this->lazy = 0;
            }
        }
    };

private:
    Node *root;
    int N;

    void update(Node *cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) cur->propogate();
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            cur->val += val;
            cur->lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        if (cur->left == nullptr) cur->left = new Node();
        update(cur->left, cL, m, l, r, val);
        if (cur->right == nullptr) cur->right = new Node();
        update(cur->right, m + 1, cR, l, r, val);
        cur->val = max(cur->left->val, cur->right->val);
    }

    int query(Node *cur, int cL, int cR, int l, int r) {
        if (cur == nullptr || cL > r || cR < l) return 0;
        if (cL != cR) cur->propogate();
        if (cL >= l && cR <= r) return cur->val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur->left, cL, m, l, r);
        int right = query(cur->right, m + 1, cR, l, r);
        return max(left, right);
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

#endif /* DATASTRUCTURES_TREES_MATH_DYNAMICLAZYSEGMENTTREE_H_ */
