/*
 * ReversionSegmentTree.h
 *
 *  Created on: Jun 3, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_REVERSIONSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_REVERSIONSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct ReversionSegmentTree {
    struct Node {
    public:
        Node *left;
        Node *right;
        int pre, suf, sum;
        Node(int val) {
            this->pre = val;
            this->suf = val;
            this->sum = val;
            this->left = this->right = nullptr;
        }

        Node(Node *l, Node *r) {
            this->left = l;
            this->right = r;
            this->pre = max(l->pre, r->pre + l->sum);
            this->suf = max(l->suf + r->sum, r->suf);
            this->sum = l->sum + r->sum;
        }
    };

    struct Query {
    public:
        int pre, suf, sum;
        bool isNull;
        Query() {
            this->pre = 0;
            this->suf = 0;
            this->sum = 0;
            this->isNull = true;
        }
        Query(int pre, int suf, int sum) {
            this->pre = pre;
            this->suf = suf;
            this->sum = sum;
            this->isNull = false;
        }

        Query(Query l, Query r) {
            this->pre = max(l.pre, r.pre + l.sum);
            this->suf = max(l.suf + r.sum, r.suf);
            this->sum = l.sum + r.sum;
            this->isNull = false;
        }
    };

private:
    vector<Node*> rev;
    int *arr;
    int revInd = 0;
    int N;

    Node *build(int cL, int cR) {
        if (cL == cR) return new Node(arr[cL]);
        int m = (cL + cR) >> 1;
        return new Node(build(cL , m), build(m + 1, cR));
    }

    Node *update(Node *cur, int cL, int cR, int ind) {
        if (cL <= ind && ind <= cR) {
            if (cL == cR) return new Node(arr[cL]);
            int m = (cL + cR) >> 1;
            return new Node(update(cur->left, cL, m, ind), update(cur->right, m + 1, cR, ind));
        }
        return cur;
    }

    Query query(Node *cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return Query();
        if (cL >= l && cR <= r) return Query(cur->pre, cur->suf, cur->sum);
        int m = (cL + cR) >> 1;
        Query left = query(cur->left, cL, m, l, r);
        Query right = query(cur->right, m + 1, cR, l, r);
        if (left.isNull) return right;
        if (right.isNull) return left;
        return Query(left, right);
    }

public:
    ReversionSegmentTree(int size, int *array) {
        arr = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            arr[i] = array[i - 1];
        }
        rev.push_back(build(1, size));
        N = size;
    }

    ReversionSegmentTree(int size) {
        arr = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            arr[i] = 0;
        }
        rev.push_back(build(1, size));
        N = size;
    }

    void update(int ind, int val) {
        arr[ind] = val;
        rev.push_back(update(rev[revInd++], 1, N, ind));
    }

    int maxPSA(int l, int r) {
        return query(rev[revInd], 1, N, l, r).pre;
    }

    int maxSSA(int l, int r) {
        return query(rev[revInd], 1, N, l, r).suf;
    }

    void revert(int x) {
        rev.push_back(rev[x]);
        revInd++;
    }
};

#endif /* DATASTRUCTURES_MATH_REVERSIONSEGMENTTREE_H_ */
