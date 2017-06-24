/*
 * DynamicSegmentTree.h
 *
 *  Created on: Jun 17, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_DYNAMICSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_DYNAMICSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct DynamicSegmentTree {
    struct Node {
    public:
        Node *left;
        Node *right;
        int val;
        Node(int val) {
            this->val = val;
            this->left = this->right = nullptr;
        }

        Node(Node *l, Node *r) {
            this->left = l;
            this->right = r;
            this->val = max(l->val, r->val);
        }
    };

private:
    Node *root;
    int *array;
    int N;

    Node *build(int cL, int cR) {
        if (cL == cR) return new Node(array[cL]);
        int m = cL + (cR - cL) / 2;
        return new Node(build(cL , m), build(m + 1, cR));
    }

    Node *update(Node *cur, int cL, int cR, int ind) {
        if (cL <= ind && ind <= cR) {
            if (cL == cR) return new Node(array[cL]);
            int m = cL + (cR - cL) / 2;
            return new Node(update(cur->left, cL, m, ind), update(cur->right, m + 1, cR, ind));
        }
        return cur;
    }

    int rMaxQ(Node *cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return INT_MIN;
        if (cL >= l && cR <= r) return cur->val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur->left, cL, m, l, r);
        int right = rMaxQ(cur->right, m + 1, cR, l, r);
        return max(left, right);
    }

public:
    DynamicSegmentTree(int size, int *arr, bool oneIndexed) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - !oneIndexed];
        }
        root = build(1, size);
        N = size;
    }

    DynamicSegmentTree(int size) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = 0;
        }
        root = build(1, size);
        N = size;
    }

    void update(int ind, int val) {
        array[ind] = val;
        root = update(root, 1, N, ind);
    }

    int rMaxQ(int l, int r) {
        return rMaxQ(root, 1, N, l, r);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_MATH_DYNAMICSEGMENTTREE_H_ */
