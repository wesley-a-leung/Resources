#ifndef DATASTRUCTURES_MATH_PERSISTENTSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_PERSISTENTSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct PersistentSegmentTree {
    struct Node {
        Node *left;
        Node *right;
        long long sum;

        Node(long long val) {
            this->sum = val;
            this->left = this->right = nullptr;
        }

        Node(Node *l, Node *r) {
            this->left = l;
            this->right = r;
            this->sum = l->sum + r->sum;
        }
    };

private:
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
        if (cL <= ind && ind <= cR) {
            if (cL == cR) return new Node(cur->sum + val);
            int m = cL + (cR - cL) / 2;
            return new Node(update(cur->left, cL, m, ind, val), update(cur->right, m + 1, cR, ind, val));
        }
        return cur;
    }

    long long query(Node *cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return 0LL;
        if (cL >= l && cR <= r) return cur->sum;
        int m = cL + (cR - cL) / 2;
        long long left = query(cur->left, cL, m, l, r);
        long long right = query(cur->right, m + 1, cR, l, r);
        return left + right;
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
            array[i] = 0LL;
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

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_MATH_PERSISTENTSEGMENTTREE_H_ */
