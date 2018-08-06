#ifndef DATASTRUCTURES_TREES_EULERTOURTREAP_H_
#define DATASTRUCTURES_TREES_EULERTOURTREAP_H_

#include <bits/stdc++.h>
using namespace std;

struct EulerTourTreap {
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
    
    const static bool PRE = true;
    const static bool POST = false;
    
    struct Node {
        int vertex;
        bool type;
        T val;
        T subtreeVal;
        double priority;
        int size;
        Node *left = nullptr, *right = nullptr, *parent = nullptr;
        Node (int vertex, bool type, T val, double priority, int size = 1) : vertex(vertex), type(type), val(val), subtreeVal(val), priority(priority), size(size) {}
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
        }
    }
    
    T merge(T a, T b) {
        return a + b;
    }
    
    Node *min(Node *x) {
        if (!x) return nullptr;
        while (x->left) x = x->left;
        return x;
    }

    Node *root(Node *x) {
        if (!x) return nullptr;
        while (x->parent) x = x->parent;
        return x;
    }
    
    // 0-based
    int index(Node *x) {
        int ind = size(x->left);
        for (; x->parent; x = x->parent) if (x->parent->left != x) ind += 1 + size(x->parent->left);
        return ind;
    }
    
    void update(Node *x) {
        if (x) {
            x->size = 1;
            x->subtreeVal = x->val;
            if (x->left) {
                x->left->parent = x;
                x->size += x->left->size;
                x->subtreeVal = merge(x->subtreeVal, x->left->subtreeVal);
            }
            if (x->right) {
                x->right->parent = x;
                x->size += x->right->size;
                x->subtreeVal = merge(x->subtreeVal, x->right->subtreeVal);
            }
        }
    }
    
    void merge(Node *&x, Node *l, Node *r) {
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
        x->parent = nullptr;
        if (ind <= size(x->left)) {
            split(x->left, l, x->left, ind);
            r = x;
        } else {
            split(x->right, x->right, r, ind - size(x->left) - 1);
            l = x;
        }
        update(x);
    }
    
    int N;
    vector<Node*> pre, post;

public:
    EulerTourTreap(vector<T> &A) : gen(seq), dis(0.0, 1.0), N(A.size()) {
        pre.reserve(N);
        post.reserve(N);
        Node *dummy = nullptr;
        for (int i = 0; i < N; i++) {
            pre.push_back(new Node(i, PRE, A[i], dis(gen)));
            post.push_back(new Node(i, POST, A[i], dis(gen)));
            merge(dummy, pre.back(), post.back());
        }
    }

    EulerTourTreap(int N) : gen(seq), dis(0.0, 1.0) {
        pre.reserve(N);
        post.reserve(N);
        Node *dummy = nullptr;
        for (int i = 0; i < N; i++) {
            pre.push_back(new Node(i, PRE, VDEF, dis(gen)));
            post.push_back(new Node(i, POST, VDEF, dis(gen)));
            merge(dummy, pre.back(), post.back());
        }
    }
    
    int treeRoot(int v) {
        return min(root(pre[v]))->vertex;
    }
    
    bool connected(int v, int w) {
        return treeRoot(v) == treeRoot(w);
    }
    
    // adds an edge from v to w
    void addEdge(int v, int w) {
        Node *l = nullptr, *r = nullptr;
        split(root(pre[v]), l, r, index(pre[v]) + 1);
        merge(l, l, root(pre[w]));
        merge(l, l, r);
    }
    
    // cut a vertex from its parent
    void cutParent(int v) {
        Node *l = nullptr, *mid = nullptr, *r = nullptr;
        split(root(pre[v]), l, mid, index(pre[v]));
        split(mid, mid, r, index(post[v]) + 1);
        merge(l, l, r);
    }
    
    T getVertexValue(int v) {
        Node *l = nullptr, *mid = nullptr, *r = nullptr;
        split(root(pre[v]), l, mid, index(pre[v]));
        split(mid, mid, r, 1);
        T ret = val(mid);
        merge(l, l, mid);
        merge(l, l, r);
        return ret;
    }
    
    // value might be doubled because of pre-index and post-index nodes
    T getSubtreeValue(int v) {
        Node *l = nullptr, *mid = nullptr, *r = nullptr;
        split(root(pre[v]), l, mid, index(pre[v]));
        split(mid, mid, r, index(post[v]) + 1);
        T ret = subtreeVal(mid);
        merge(l, l, mid);
        merge(l, l, r);
        return ret;
    }
    
    void updateVertex(int v, T val) {
        Node *l = nullptr, *preV = nullptr, *mid = nullptr, *postV = nullptr, *r = nullptr;
        split(root(pre[v]), l, preV, index(pre[v]));
        split(preV, preV, mid, 1);
        split(mid, mid, postV, index(post[v]));
        split(postV, postV, r, 1);
        apply(preV, val);
        apply(postV, val);
        merge(l, l, preV);
        merge(l, l, mid);
        merge(l, l, postV);
        merge(l, l, r);
    }
};

#endif /* DATASTRUCTURES_TREES_EULERTOURTREAP_H_ */
