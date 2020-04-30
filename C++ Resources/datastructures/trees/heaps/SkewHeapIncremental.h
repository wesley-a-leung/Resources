#pragma once
#include <bits/stdc++.h>
using namespace std;

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937 rng(seq);

// Heap supporting merges and increments
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, increment, size: O(1)
//   pop, push, merge: O(log N) expected if randomized, amortized if not
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>, class Delta = Value, const bool RANDOMIZED = false> struct SkewHeapIncremental {
    struct Node {
        Value val; Delta delta; unique_ptr<Node> l, r; Node(const Value &v, const Delta &d) : val(v), delta(d) {}
    };
    Comparator cmp; Delta ddef; int cnt; unique_ptr<Node> root;
    void propagate(const unique_ptr<Node> &a) {
        a->val = a->val + a->delta;
        if (a->l) a->l->delta = a->l->delta + a->delta;
        if (a->r) a->r->delta = a->r->delta + a->delta;
        a->delta = ddef;
    }
    unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
        if (!a || !b) return a ? move(a) : move(b);
        propagate(a); propagate(b);
        if (cmp(a->val, b->val)) a.swap(b);
        if (!RANDOMIZED || rng() % 2) a->l.swap(a->r);
        a->r = merge(move(b), move(a->r)); return move(a);
    }
    SkewHeapIncremental(const Delta &ddef = Delta()) : ddef(ddef), cnt(0) {}
    bool empty() const { return !root; }
    Value top() { propagate(root); return root->val; }
    Value pop() { propagate(root); Value ret = root->val; root = merge(move(root->l), move(root->r)); cnt--; return ret; }
    void push(const Value &val) { root = merge(move(root), make_unique<Node>(val, ddef)); cnt++; }
    void increment(const Delta &delta) { if (root) root->delta = root->delta + delta; }
    void merge(SkewHeapIncremental &h) { root = merge(move(root), move(h.root)); cnt += h.cnt; }
    int size() const { return cnt; }
};
