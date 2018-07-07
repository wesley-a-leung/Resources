#ifndef DATASTRUCTURES_TREES_LAZYIMPLICIT_TREAP_H_
#define DATASTRUCTURES_TREES_LAZYIMPLICIT_TREAP_H_

#include <bits/stdc++.h>
using namespace std;

struct LazyImplicitTreap {
private:
    seed_seq seq {
        (uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
        (uint64_t) __builtin_ia32_rdtsc(),
        (uint64_t) (uintptr_t) make_unique<char>().get()
    };
    mt19937 gen;
    uniform_real_distribution<double> dis;
    
    using T = int;
    const static T VDEF = 0;
    const static T LDEF = 0;
    
    struct Node {
        T val;
        T subtreeVal;
        T delta = LazyImplicitTreap::LDEF;
        double priority;
        int size;
        Node *left = nullptr, *right = nullptr;
        Node (T val, double priority, int size = 1) : val(val), subtreeVal(val), priority(priority), size(size) {}
    };
    
    int size(Node *x) {
        return x ? x->size : 0;
    }
    
    T val(Node *x) {
        return x ? x->val : VDEF;
    }
    
    T subtreeVal(Node *x) {
        return x ? x->subtreeVal : VDEF;
    }
    
    void apply(Node *x, T val) {
        if (x) {
            x->subtreeVal += val * size(x);
            x->val += val;
            x->delta += val;
        }
    }
    
    T merge(T a, T b) {
        return a + b;
    }
    
    void update(Node *x) {
        if (x) {
            x->size = 1;
            x->subtreeVal = x->val;
            if (x->left) {
                x->size += x->left->size;
                x->subtreeVal = merge(x->subtreeVal, x->left->subtreeVal);
            }
            if (x->right) {
                x->size += x->right->size;
                x->subtreeVal = merge(x->subtreeVal, x->right->subtreeVal);
            }
        }
    }
    
    void propagate(Node *x) {
        if (x && x->delta != LDEF) {
            if (x->left) apply(x->left, x->delta);
            if (x->right) apply(x->right, x->delta);
            x->delta = LDEF;
        }
    }
    
    void merge(Node *&x, Node *l, Node *r) {
        propagate(l);
        propagate(r);
        if (!l || !r) {
            x = l ? l : r;
        } else if (l->priority > r->priority) {
            merge(l->right, l->right, r);
            x = l;
        } else {
            merge(r->left, l, r->left);
            x = r;
        }
        update(x);
    }
    
    // ind is the size of the l tree
    void split(Node *x, Node *&l, Node *&r, int ind) {
        if (!x) {
            l = r = nullptr;
            return;
        }
        propagate(x);
        if (ind <= size(x->left)) {
            split(x->left, l, x->left, ind);
            r = x;
        } else {
            split(x->right, x->right, r, ind - size(x->left) - 1);
            l = x;
        }
        update(x);
    }
    
    Node *root = nullptr;

public:
    LazyImplicitTreap(int N) : gen(seq), dis(0.0, 1.0) {
        for (int i = 0; i < N; i++) merge(root, root, new Node(VDEF, dis(gen)));
    }

    LazyImplicitTreap(T *A, int N, bool oneIndexed) : gen(seq), dis(0.0, 1.0) {
        for (int i = oneIndexed; i < N + oneIndexed; i++) merge(root, root, new Node(A[i], dis(gen)));
    }
    
    void updateRange(int l, int r, T val) {
        Node *left = nullptr, *right = nullptr, *mid = nullptr;
        split(root, left, mid, l);
        split(mid, mid, right, r - l + 1);
        apply(mid, val);
        merge(root, left, mid);
        merge(root, root, right);
    }
    
    T queryRange(int l, int r) {
        Node *left = nullptr, *right = nullptr, *mid = nullptr;
        split(root, left, mid, l);
        split(mid, mid, right, r - l + 1);
        T ret = val(mid);
        merge(root, left, mid);
        merge(root, root, right);
        return ret;
    }
    
    int getSize() {
        return size(root);
    }
};

#endif /* DATASTRUCTURES_TREES_LAZYIMPLICIT_TREAP_H_ */
