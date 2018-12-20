#pragma once
#include <bits/stdc++.h>
using namespace std;

// Heap supporting merges
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, size: O(1)
//   pop, push, merge: O(log N)
template <class Value, class Comparator = less<Value>> struct SkewHeap {
    Comparator cmp; struct Node;
    struct Node { Value val; unique_ptr<Node> left, right; Node(Value val) : val(val) {} };
    int cnt = 0; unique_ptr<Node> root;
    unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
        if (!a || !b) return a ? move(a) : move(b);
        if (cmp(a->val, b->val)) a.swap(b);
        a->right = merge(move(b), move(a->right)); a->left.swap(a->right);
        return a;
    }
    SkewHeap() {}
    bool empty() { return !root; }
    Value top() { return root->val; }
    Value pop() {
        Value ret = root->val; root = merge(move(root->left), move(root->right)); cnt--;
        return ret;
    }
    void push(Value val) { root = merge(move(root), make_unique<Node>(val)); cnt++; }
    void merge(SkewHeap &h) { root = merge(move(root), move(h.root)); cnt += h.cnt; }
    int size() { return cnt; }
};
