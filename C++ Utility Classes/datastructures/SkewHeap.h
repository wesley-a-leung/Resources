#ifndef DATASTRUCTURES_INCREMENTALSKEWHEAP_H_
#define DATASTRUCTURES_INCREMENTALSKEWHEAP_H_

#include <bits/stdc++.h>
using namespace std;

// convention is same as priority_queue in STL
template <typename Value, typename Comparator = less<Value>>
struct IncrementalSkewHeap {
private:
    Comparator cmp;

    struct Node {
        Value val;
        Node *left = nullptr, *right = nullptr;
        Value delta = 0;
        Node (Value val) : val(val) {}
    };

    int sz = 0;
    Node *root = nullptr;

    void propagate(Node *a) {
        a->val += a->delta;
        if (nullptr != a->left) a->left->delta += a->delta;
        if (nullptr != a->right) a->right->delta += a->delta;
        a->delta = 0;
    }

    Node *merge(Node *a, Node *b) {
        if (nullptr == a || nullptr == b) return nullptr == a ? b : a;
        propagate(a);
        propagate(b);
        if (cmp(a->val, b->val)) swap(a, b);
        a->right = merge(b, a->right);
        swap(a->left, a->right);
        return a;
    }

public:
    IncrementalSkewHeap() {}

    bool isEmpty() {
        return nullptr == root;
    }

    Value top() {
        propagate(root);
        return root->val;
    }

    Value pop() {
        propagate(root);
        Value ret = root->val;
        Node *temp = root;
        root = merge(root->left, root->right);
        delete(temp);
        sz--;
        return ret;
    }

    void push(Value val) {
        Node *x = new Node(val);
        root = merge(root, x);
        sz++;
    }

    void increment(Value delta) {
        if (nullptr != root) root->delta += delta;
    }

    void merge(IncrementalSkewHeap &h) {
        root = merge(root, h.root);
        sz += h.sz;
    }

    int size() {
        return sz;
    }
};

#endif /* DATASTRUCTURES_INCREMENTALSKEWHEAP_H_ */
