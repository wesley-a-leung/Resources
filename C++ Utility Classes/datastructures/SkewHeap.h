#ifndef DATASTRUCTURES_INCREMENTALSKEWHEAP_H_
#define DATASTRUCTURES_INCREMENTALSKEWHEAP_H_

#include <bits/stdc++.h>
using namespace std;

// convention is same as priority_queue in STL
template <typename Value, typename Comparator = less<Value>>
struct SkewHeap {
private:
    Comparator cmp;

    struct Node {
        Value val;
        Node *left = nullptr, *right = nullptr;
        Node (Value val) : val(val) {}
    };

    int sz = 0;
    Node *root = nullptr;

    Node *merge(Node *a, Node *b) {
        if (nullptr == a || nullptr == b) return nullptr == a ? b : a;
        if (cmp(a->val, b->val)) swap(a, b);
        a->right = merge(b, a->right);
        swap(a->left, a->right);
        return a;
    }

public:
    SkewHeap() {}

    bool isEmpty() {
        return nullptr == root;
    }

    Value top() {
        return root->val;
    }

    Value pop() {
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

    void merge(SkewHeap &h) {
        root = merge(root, h.root);
        sz += h.sz;
    }

    int size() {
        return sz;
    }
};

#endif /* DATASTRUCTURES_INCREMENTALSKEWHEAP_H_ */
