#pragma once
#include <bits/stdc++.h>
using namespace std;

// Node classes for Treap and Splay trees
// A node class must contain at a minimum, the following for Treap and Splay support:
//   Data: typedef/using of the value type this node stores
//   HAS_PAR: const static bool indicating whether this node has a parent pointer
//   sz: integer representing the size of the subtree
//   l: pointer of the same node type to the left child
//   r: pointer of the same node type to the right child
//   val: Data representing the value of the node
//   update: void() that updates the size of subtree, along with any additional information
//   propagate: void() that pushes information lazily to the children
//   If HAS_PAR is true, then it should also contain the following:
//     p: pointer of the same node type to the parent
//   If the node class is used in conjunction with DynamicRangeOperations, then it must also contain the following:
//     Lazy: typedef/using of the lazy type used to update the node (even if no lazy propagation occurs)
//     RANGE_UPDATES: const static bool indicating whether range updates are supported
//     RANGE_QUERIES: const static bool indicating whether range queries are supported
//     RANGE_REVERSALS: const static bool indicating whether range reversals are supported
//     apply: void(const Lazy &v) that updates the information of the node (including optional aggregate and lazy information)
//   If RANGE_UPDATES is true, then the following are required:
//     lz: Lazy representing the lazy information that will be pushed to the child nodes
//   If RANGE_QUERIES is true, then the following are required:
//     sbtr: Data representing the aggregate data of the subtree
//     qdef: static Data() returning the query default value
//   If RANGE_REVERSALS is true, then the following are required:
//     reverse(): void() that reverses the subtree rooted at that node (aggregate data and any lazy flags should be reversed)

// Sample node class for a single value of type T, supporting range reversals, and point assignment
// Tested:
//   https://dmoj.ca/problem/ds4
//   https://codeforces.com/contest/1288/problem/E
//   https://codeforces.com/contest/863/problem/D
template <class T> struct NodeVal {
    using Data = T;
    using Lazy = Data;
    const static int RANGE_UPDATES = false;
    const static int RANGE_QUERIES = false;
    const static int RANGE_REVERSALS = true;
    const static int HAS_PAR = true;
    bool rev; int sz; NodeVal *l, *r, *p; Data val;
    NodeVal(const Data &v) : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr), val(v) {}
    void update() {
        sz = 1;
        if (l) sz += l->sz;
        if (r) sz += r->sz;
    }
    void propagate() {
        if (rev) {
            swap(l, r); rev = false;
            if (l) l->reverse();
            if (r) r->reverse();
        }
    }
    void apply(const Lazy &v) { val = v; }
    void reverse() { rev = !rev; }
};

// Sample node class for aggregate range queries using the Combine struct, along with range reversals, and point assignment
// Tested:
//   https://dmoj.ca/problem/dmpg17g2
template <class Combine> struct NodeAgg {
    using Data = typename Combine::Data;
    using Lazy = typename Combine::Lazy;
    const static int RANGE_UPDATES = false;
    const static int RANGE_QUERIES = true;
    const static int RANGE_REVERSALS = true;
    const static bool HAS_PAR = true;
    bool rev; int sz; NodeAgg *l, *r, *p; Data val, sbtr;
    NodeAgg(const Data &v) : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr), val(v), sbtr(v) {}
    void update() {
        sz = 1; sbtr = val;
        if (l) { sz += l->sz; sbtr = Combine().merge(l->sbtr, sbtr); }
        if (r) { sz += r->sz; sbtr = Combine().merge(sbtr, r->sbtr); }
    }
    void propagate() {
        if (rev) {
            swap(l, r); rev = false;
            if (l) l->reverse();
            if (r) r->reverse();
        }
    }
    void apply(const Lazy &v) { val = Combine().applyLazy(val, v); sbtr = Combine().applyLazy(sbtr, v); }
    void reverse() { rev = !rev; Combine().revData(sbtr); }
    static Data qdef() { return Combine().qdef; }
};

// Sample node class for aggregate range queries and lazy range updates using the Combine struct, along with range reversals
// Tested:
//   https://dmoj.ca/problem/acc1p1
//   https://wcipeg.com/problem/noi05p2
template <class Combine> struct NodeLazyAgg {
    using Data = typename Combine::Data;
    using Lazy = typename Combine::Lazy;
    const static int RANGE_UPDATES = true;
    const static int RANGE_QUERIES = true;
    const static int RANGE_REVERSALS = true;
    const static int HAS_PAR = true;
    bool rev; int sz; NodeLazyAgg *l, *r, *p; Lazy lz; Data val, sbtr;
    NodeLazyAgg(const Data &v) : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr), lz(Combine().ldef), val(v), sbtr(v) {}
    void update() {
        sz = 1; sbtr = val;
        if (l) { sz += l->sz; sbtr = Combine().merge(l->sbtr, sbtr); }
        if (r) { sz += r->sz; sbtr = Combine().merge(sbtr, r->sbtr); }
    }
    void propagate() {
        if (rev) {
            swap(l, r); rev = false;
            if (l) l->reverse();
            if (r) r->reverse();
        }
        if (lz != Combine().ldef) {
            if (l) l->apply(lz);
            if (r) r->apply(lz);
            lz = Combine().ldef;
        }
    }
    void apply(const Lazy &v) {
        val = Combine().applyLazy(val, v); sbtr = Combine().applyLazy(sbtr, Combine().getSegmentVal(v, sz));
        lz = Combine().mergeLazy(lz, v);
    }
    void reverse() { rev = !rev; Combine().revData(sbtr); }
    static Data qdef() { return Combine().qdef; }
};
