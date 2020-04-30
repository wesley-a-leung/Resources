#pragma once
#include <bits/stdc++.h>
using namespace std;

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937 rng(seq);

// Heap supporting merges
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, size: O(1)
//   pop, push, merge: O(log N) expected if randomized, amortized if not 
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>, const bool RANDOMIZED = false> struct SkewHeap {
    struct Node {
        Value val; unique_ptr<Node> l, r; Node(const Value &v) : val(v) {}
    };
    Comparator cmp; int cnt; unique_ptr<Node> root;
    unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
        if (!a || !b) return a ? move(a) : move(b);
        if (cmp(a->val, b->val)) a.swap(b);
        if (!RANDOMIZED || rng() % 2) a->l.swap(a->r);
        a->r = merge(move(b), move(a->r)); return move(a);
    }
    SkewHeap() : cnt(0) {}
    bool empty() const { return !root; }
    Value top() const { return root->val; }
    Value pop() { Value ret = root->val; root = merge(move(root->l), move(root->r)); cnt--; return ret; }
    void push(const Value &val) { root = merge(move(root), make_unique<Node>(val)); cnt++; }
    void merge(SkewHeap &h) { root = merge(move(root), move(h.root)); cnt += h.cnt; }
    int size() const { return cnt; }
};
