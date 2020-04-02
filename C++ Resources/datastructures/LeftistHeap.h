#pragma once
#include <bits/stdc++.h>
using namespace std;

// Heap supporting merges
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, size: O(1)
//   pop, push, merge: O(log N)
template <class Value, class Comparator = less<Value>> struct LeftistHeap {
    struct Node {
        Value val; int dist; unique_ptr<Node> left, right;
        Node(const Value &v) : val(v), dist(0) {}
    };
    Comparator cmp; int cnt; unique_ptr<Node> root;
    unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
        if (!a || !b) return a ? move(a) : move(b);
        if (cmp(a->val, b->val)) a.swap(b);
        a->right = merge(move(a->right), move(b));
        if (!a->left || a->left->dist < a->right->dist) a->left.swap(a->right);
        a->dist = (a->right ? a->right->dist : 0) + 1; return a;
    }
    LeftistHeap() : cnt(0) {}
    bool empty() const { return !root; }
    Value top() const { return root->val; }
    Value pop() {
        Value ret = root->val; root = merge(move(root->left), move(root->right)); cnt--;
        return ret;
    }
    void push(const Value &val) { root = merge(move(root), make_unique<Node>(val)); cnt++; }
    void merge(LeftistHeap &h) { root = merge(move(root), move(h.root)); cnt += h.cnt; }
    int size() const { return cnt; }
};
