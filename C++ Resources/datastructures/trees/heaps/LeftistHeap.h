#pragma once
#include <bits/stdc++.h>
#include "../../../utils/MakeUnique.h"
using namespace std;

// Heap supporting merges
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, size: O(1)
//   pop, push, merge: O(log N)
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct LeftistHeap {
    struct Node {
        Value val; int dist; unique_ptr<Node> l, r; Node(const Value &v) : val(v), dist(0) {}
    };
    Comparator cmp; int cnt; unique_ptr<Node> root;
    unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
        if (!a || !b) return a ? move(a) : move(b);
        if (cmp(a->val, b->val)) a.swap(b);
        a->r = merge(move(a->r), move(b));
        if (!a->l || a->l->dist < a->r->dist) a->l.swap(a->r);
        a->dist = (a->r ? a->r->dist : 0) + 1; return move(a);
    }
    LeftistHeap() : cnt(0) {}
    bool empty() const { return !root; }
    Value top() const { return root->val; }
    Value pop() { Value ret = root->val; root = merge(move(root->l), move(root->r)); cnt--; return ret; }
    void push(const Value &val) { root = merge(move(root), make_unique<Node>(val)); cnt++; }
    void merge(LeftistHeap &h) { root = merge(move(root), move(h.root)); cnt += h.cnt; }
    int size() const { return cnt; }
};
