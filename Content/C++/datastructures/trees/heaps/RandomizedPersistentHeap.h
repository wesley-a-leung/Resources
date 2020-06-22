#pragma once
#include <bits/stdc++.h>
#if __cplusplus < 201402L
#include "../../../utils/MakeUnique.h"
#endif
using namespace std;

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937 rng(seq);

// Persistent Heap supporting merges
// comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor, empty, top, size: O(1)
//   pop, push, merge: O(log N) expected
// Memory Complexity: O(N + Q log N) for Q operations
template <class Value, class Comparator = less<Value>> struct PersistentRandomizedHeap {
    struct Node; using ptr = shared_ptr<Node>;
    struct Node {
        Value val; ptr l, r;
        Node(const Value &v, const ptr &l = ptr(), const ptr &r = ptr()) : val(v), l(l), r(r) {}
    };
    Comparator cmp; int cnt; ptr root;
    ptr merge(ptr a, ptr b) {
        if (!a || !b) return a ? a : b;
        if (cmp(a->val, b->val)) a.swap(b);
        return rng() % 2 ? make_shared<Node>(a->val, a->l, merge(a->r, b)) : make_shared<Node>(a->val, merge(a->l, b), a->r);
    }
    PersistentRandomizedHeap() : cnt(0) {}
    bool empty() const { return !root; }
    Value top() { return root->val; }
    Value pop() { Value ret = root->val; root = merge(root->l, root->r); cnt--; return ret; }
    void push(const Value &val) { root = merge(root, make_shared<Node>(val)); cnt++; }
    void merge(const PersistentRandomizedHeap &h) { root = merge(root, h.root); cnt += h.cnt; }
    int size() const { return cnt; }
};
