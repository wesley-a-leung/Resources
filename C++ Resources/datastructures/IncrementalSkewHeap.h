#pragma once
#include <bits/stdc++.h>
using namespace std;

// Heap supporting merges and increments
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, increment, size: O(1)
//   pop, push, merge: O(log N)
template <class Value, class Comparator = less<Value>, class D_Type = Value> struct IncrementalSkewHeap {
    Comparator cmp; struct Node;
    struct Node { Value val; D_Type delta; unique_ptr<Node> left, right; Node(const Value &val) : val(val), delta(0) {} };
    int cnt = 0; unique_ptr<Node> root;
    void propagate(unique_ptr<Node> &a) {
        a->val += a->delta;
        if (a->left) a->left->delta += a->delta;
        if (a->right) a->right->delta += a->delta;
        a->delta = 0;
    }
    unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
        if (!a || !b) return a ? move(a) : move(b);
        propagate(a); propagate(b);
        if (cmp(a->val, b->val)) a.swap(b);
        a->right = merge(move(b), move(a->right)); a->left.swap(a->right);
        return a;
    }
    IncrementalSkewHeap() {}
    bool empty() const { return !root; }
    Value top() { propagate(root); return root->val; }
    Value pop() {
        propagate(root); Value ret = root->val; root = merge(move(root->left), move(root->right)); cnt--;
        return ret;
    }
    void push(const Value &val) { root = merge(move(root), make_unique<Node>(val)); cnt++; }
    void increment(const D_Type &delta) { if (root) root->delta += delta; }
    void merge(IncrementalSkewHeap &h) { root = merge(move(root), move(h.root)); cnt += h.cnt; }
    int size() const { return cnt; }
};
