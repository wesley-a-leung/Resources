/*
 * DynamicLazySegmentTree.h
 *
 *  Created on: Jun 19, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_DYNAMICLAZYSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_DYNAMICLAZYSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct DynamicLazySegmentTree {
    struct Node {
        Node *left = nullptr, *right = nullptr;
        int val = 0, lazy = 0;

        Node(int val) {
            this->val = val;
        }

        Node(Node *l, Node *r) {
            this->left = l;
            this->right = r;
            this->val = max(l->val, r->val);
        }

        void propogate() {
            if (this->lazy != 0) {
                if (left != nullptr) {
                    left->val += this->lazy;
                    left->lazy += this->lazy;
                }
                if (right != nullptr) {
                    right->val += this->lazy;
                    right->lazy += this->lazy;
                }
                this->lazy = 0;
            }
        }
    };

private:
    Node *root;
    int N;
    int *array;

    Node *build(int cL, int cR) {
        if (cL == cR) return new Node(array[cL]);
        int m = cL + (cR - cL) / 2;
        return new Node(build(cL , m), build(m + 1, cR));
    }

    void update(Node *cur, int cL, int cR, int l, int r, int val) {
        cur->propogate();
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            cur->val += val;
            cur->lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur->left, cL, m, l, r, val);
        update(cur->right, m + 1, cR, l, r, val);
        cur->val = max(cur->left->val, cur->right->val);
    }

    int rMaxQ(Node *cur, int cL, int cR, int l, int r) {
        cur->propogate();
        if (cL > r || cR < l) return INT_MIN;
        if (cL >= l && cR <= r) return cur->val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur->left, cL, m, l, r);
        int right = rMaxQ(cur->right, m + 1, cR, l, r);
        return max(left, right);
    }

public:
    DynamicLazySegmentTree(int size, int *arr, bool oneIndexed) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - !oneIndexed];
        }
        root = build(1, size);
        N = size;
    }

    DynamicLazySegmentTree(int size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = 0;
        }
        root = build(1, size);
        N = size;
    }

    void update(int l, int r, int val) {
        update(root, 1, N, l, r, val);
    }

    int rMaxQ(int l, int r) {
        return rMaxQ(root, 1, N, l, r);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_MATH_DYNAMICLAZYSEGMENTTREE_H_ */
