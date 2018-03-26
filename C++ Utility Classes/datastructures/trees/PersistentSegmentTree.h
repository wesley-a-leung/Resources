#ifndef DATASTRUCTURES_TREES_PERSISTENTSEGMENTTREE_H_
#define DATASTRUCTURES_TREES_PERSISTENTSEGMENTTREE_H_

#include <bits/stdc++.h>
using namespace std;

struct PersistentSegmentTree {
private:
    static const long long vdef = 0, qdef = 0;

    static long long merge(long long l, long long r) {
        return l + r;
    }

    static long long apply(long long x, long long v) {
        return x + v;
    }

    struct Node {
        Node *left;
        Node *right;
        long long val;

        Node(long long val) {
            this->val = val;
            this->left = this->right = nullptr;
        }

        Node(Node *l, Node *r) {
            this->left = l;
            this->right = r;
            this->val = PersistentSegmentTree::merge(l->val, r->val);
        }
    };

    vector<Node*> rev;
    long long *array;
    int revInd = 0;
    int N;

    Node *build(int cL, int cR) {
        if (cL == cR) return new Node(array[cL]);
        int m = cL + (cR - cL) / 2;
        return new Node(build(cL , m), build(m + 1, cR));
    }

    Node *update(Node *cur, int cL, int cR, int ind, long long val) {
        if (cL > ind || cR < ind) return cur;
        if (cL == cR) return new Node(apply(cur->val, val));
        int m = cL + (cR - cL) / 2;
        return new Node(update(cur->left, cL, m, ind, val), update(cur->right, m + 1, cR, ind, val));
    }

    long long query(Node *cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (cL >= l && cR <= r) return cur->val;
        int m = cL + (cR - cL) / 2;
        long long left = query(cur->left, cL, m, l, r);
        long long right = query(cur->right, m + 1, cR, l, r);
        return merge(left, right);
    }

public:
    PersistentSegmentTree(int size, long long *arr, bool oneIndexed) {
        array = new long long[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - !oneIndexed];
        }
        rev.push_back(build(1, size));
        N = size;
    }

    PersistentSegmentTree(int size) {
        array = new long long[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = vdef;
        }
        rev.push_back(build(1, size));
        N = size;
    }

    void update(int ind, int val) {
        rev.push_back(update(rev[revInd++], 1, N, ind, val));
    }

    long long query(int revInd, int l, int r) {
        return query(rev[revInd], 1, N, l, r);
    }

    void revert(int x) {
        rev.push_back(rev[x]);
        revInd++;
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_TREES_PERSISTENTSEGMENTTREE_H_ */
