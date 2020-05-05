#pragma once
#include <bits/stdc++.h>
using namespace std;

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937 rng(seq);

// Persistent Heap supporting merges and increments
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, increment, size: O(1)
//   pop, push, merge: O(log N) expected
// Memory Complexity: O(N + Q log N) for Q operations
template <class Value, class Comparator = less<Value>, class Delta = Value> struct PersistentRandomizedHeapIncremental {
    struct Node; using ptr = shared_ptr<Node>;
    struct Node {
        Value val; Delta delta; ptr l, r;
        Node(const Value &v, const Delta &d, const ptr &l = ptr(), const ptr &r = ptr()) : val(v), delta(d), l(l), r(r) {}
    };
    Comparator cmp; Delta ddef; int cnt; ptr root;
    void propagate(ptr &a) {
        a->val += a->delta;
        if (a->l) { a->l = make_shared<Node>(*a->l); a->l->delta += a->delta; }
        if (a->r) { a->r = make_shared<Node>(*a->r); a->r->delta += a->delta; }
        a->delta = ddef;
    }
    ptr merge(ptr a, ptr b) {
        if (!a || !b) return a ? a : b;
        propagate(a); propagate(b);
        if (cmp(a->val, b->val)) a.swap(b);
        return rng() % 2 ? make_shared<Node>(a->val, a->delta, a->l, merge(a->r, b)) : make_shared<Node>(a->val, a->delta, merge(a->l, b), a->r);
    }
    PersistentRandomizedHeapIncremental(const Delta &ddef = Delta()) : ddef(ddef), cnt(0) {}
    bool empty() const { return !root; }
    Value top() { propagate(root); return root->val; }
    Value pop() { propagate(root); Value ret = root->val; root = merge(root->l, root->r); cnt--; return ret; }
    void push(const Value &val) { root = merge(root, make_shared<Node>(val, ddef)); cnt++; }
    void increment(const Delta &delta) { if (root) { root = make_shared<Node>(*root); root->delta += delta; } }
    void merge(const PersistentRandomizedHeapIncremental &h) { root = merge(root, h.root); cnt += h.cnt; }
    int size() const { return cnt; }
};
