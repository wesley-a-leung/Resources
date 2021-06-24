#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sample node structs for Splay, DynamicRangeOperations,
//   and LinkCutTree with different required fields required for each

// Sample node class for a single value of type T, supporting range reversals,
//   and point assignment
// Template Arguments:
//   T: the type of value to store
// Constructor Arguments:
//   v: the value of type Data to store
// Fields:
//   Data: the data type, equivalent to T
//   Lazy: the lazy type, equivalent to T
//   static const RANGE_UPDATES: a boolean indicating whether range updates
//     are permitted
//   static const RANGE_QUERIES: a boolean indicating whether range queries
//     are permitted
//   static const RANGE_REVERSALS: a boolean indicating whether range reversals
//     are permitted
//   rev: a boolean indicating whether the subtree needs to be reversed
//   sz: the number of nodes in the subtree
//   l: a pointer to the left child
//   r: a pointer to the right child
//   p: a pointer to the parent
//   val: the value of type Data being stored
// Functions:
//   update(): updates the current node's information based on its children
//   propagate(): propagates the current node's lazy information (including
//     rev) to its children
//   apply(v): applies the lazy value v to the node
//   reverse(): reverse this node's subtree (lazy flags
//     should be reversed)
// Tested:
//   https://dmoj.ca/problem/ds4
//   https://codeforces.com/contest/1288/problem/E
//   https://codeforces.com/contest/863/problem/D
//   https://codeforces.com/contest/13/problem/E
//   https://oj.uz/problem/view/JOI13_synchronization
template <class T> struct NodeVal {
  using Data = T; using Lazy = Data;
  static const bool RANGE_UPDATES = false, RANGE_QUERIES = false;
  static const bool RANGE_REVERSALS = true;
  bool rev; int sz; NodeVal *l, *r, *p; Data val;
  NodeVal(const Data &v)
      : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr), val(v) {}
  void update() {
    sz = 1;
    if (l) sz += l->sz;
    if (r) sz += r->sz;
  }
  void propagate() {
    if (rev) {
      if (l) l->reverse();
      if (r) r->reverse();
      rev = false;
    }
  }
  void apply(const Lazy &v) { val = v; }
  void reverse() { rev = !rev; swap(l, r); }
};

// Sample node class for aggregate range queries using a struct to combine
//   Data and Lazy, along with range reversals, and point assignment
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       static revData(v): reverses the value v of type Data
//     Sample Struct: supporting point increments and range max queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) {
//           return max(l, r);
//         }
//         static Data applyLazy(const Data &l, const Lazy &r) {
//           return l + r;
//         }
//         static void revData(Data &v) {}
//       };
// Constructor Arguments:
//   v: the value of type Data to store
// Fields:
//   Data: the data type, equivalent to C::Data
//   Lazy: the lazy type, equivalent to C::Lazy
//   static const RANGE_UPDATES: a boolean indicating whether range updates
//     are permitted
//   static const RANGE_QUERIES: a boolean indicating whether range queries
//     are permitted
//   static const RANGE_REVERSALS: a boolean indicating whether range reversals
//     are permitted
//   rev: a boolean flag indicating whether the subtree needs to be reversed
//   sz: the number of nodes in the subtree
//   l: a pointer to the left child
//   r: a pointer to the right child
//   p: a pointer to the parent
//   val: the value of type Data being stored
//   sbtr: the aggregate value of type Data for the subtree
// Functions:
//   update(): updates the current node's information based on its children
//   propagate(): propagates the current node's lazy information (including
//     rev) to its children
//   apply(v): applies the lazy value v to the node
//   reverse(): reverse this node's subtree (aggregate data and any
//     lazy flags should be reversed)
//   static qdef(): returns the query default value for the struct C
// Tested:
//   https://dmoj.ca/problem/dmpg17g2
//   https://dmoj.ca/problem/coi08p2
//   https://www.spoj.com/problems/QTREE2/
//   https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
template <class C> struct NodeAgg {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  static const bool RANGE_UPDATES = false, RANGE_QUERIES = true;
  static const bool RANGE_REVERSALS = true;
  bool rev; int sz; NodeAgg *l, *r, *p; Data val, sbtr;
  NodeAgg(const Data &v)
      : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr),
        val(v), sbtr(v) {}
  void update() {
    sz = 1; sbtr = val;
    if (l) { sz += l->sz; sbtr = C::merge(l->sbtr, sbtr); }
    if (r) { sz += r->sz; sbtr = C::merge(sbtr, r->sbtr); }
  }
  void propagate() {
    if (rev) {
      if (l) l->reverse();
      if (r) r->reverse();
      rev = false;
    }
  }
  void apply(const Lazy &v) {
    val = C::applyLazy(val, v); sbtr = C::applyLazy(sbtr, v);
  }
  void reverse() { rev = !rev; swap(l, r); C::revData(sbtr); }
  static Data qdef() { return C::qdef(); }
};

// Sample node class for aggregate range queries and lazy range updates
//  using a struct to combine Data and Lazy, along with range reversals
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       static getSegmentVal(v, k): returns the lazy value v when applied over
//         a segment of length k
//       static mergeLazy(l, r): returns the values l of type Lazy merged with
//         r of type Lazy, must be associative
//       static revData(v): reverses the value v of type Data
//     Sample Struct: supporting range assignments and range sum queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Lazy ldef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data applyLazy(const Data &l, const Lazy &r) { return r; }
//         static Lazy getSegmentVal(const Lazy &v, int k) { return v * k; }
//         static Lazy mergeLazy(const Lazy &l, const Lazy &r) { return r; }
//         static void revData(Data &v) {}
//       };
// Constructor Arguments:
//   v: the value of type Data to store
// Fields:
//   Data: the data type, equivalent to C::Data
//   Lazy: the lazy type, equivalent to C::Lazy
//   static const RANGE_UPDATES: a boolean indicating whether range updates
//     are permitted
//   static const RANGE_QUERIES: a boolean indicating whether range queries
//     are permitted
//   static const RANGE_REVERSALS: a boolean indicating whether range reversals
//     are permitted
//   rev: a boolean flag indicating whether the subtree needs to be reversed
//   sz: the number of nodes in the subtree
//   l: a pointer to the left child
//   r: a pointer to the right child
//   p: a pointer to the parent
//   lz: the value of type Lazy to be propagated
//   val: the value of type Data being stored
//   sbtr: the aggregate value of type Data for the subtree
// Functions:
//   update(): updates the current node's information based on its children
//   propagate(): propagates the current node's lazy information (including
//     rev) to its children
//   apply(v): applies the lazy value v to the node
//   reverse(): reverse this node's subtree (aggregate data and any
//     lazy flags should be reversed)
//   static qdef(): returns the query default value for the struct C
// Tested:
//   https://dmoj.ca/problem/acc1p1
//   https://dmoj.ca/problem/noi05p2
//   https://dmoj.ca/problem/ds5easy
//   https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
template <class C> struct NodeLazyAgg {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  static const bool RANGE_UPDATES = true, RANGE_QUERIES = true;
  static const bool RANGE_REVERSALS = true;
  bool rev; int sz; NodeLazyAgg *l, *r, *p; Lazy lz; Data val, sbtr;
  NodeLazyAgg(const Data &v)
      : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr),
        lz(C::ldef()), val(v), sbtr(v) {}
  void update() {
    sz = 1; sbtr = val;
    if (l) { sz += l->sz; sbtr = C::merge(l->sbtr, sbtr); }
    if (r) { sz += r->sz; sbtr = C::merge(sbtr, r->sbtr); }
  }
  void propagate() {
    if (rev) {
      if (l) l->reverse();
      if (r) r->reverse();
      rev = false;
    }
    if (lz != C::ldef()) {
      if (l) l->apply(lz);
      if (r) r->apply(lz);
      lz = C::ldef();
    }
  }
  void apply(const Lazy &v) {
    lz = C::mergeLazy(lz, v); val = C::applyLazy(val, v);
    sbtr = C::applyLazy(sbtr, C::getSegmentVal(v, sz));
  }
  void reverse() { rev = !rev; swap(l, r); C::revData(sbtr); }
  static Data qdef() { return C::qdef(); }
};
