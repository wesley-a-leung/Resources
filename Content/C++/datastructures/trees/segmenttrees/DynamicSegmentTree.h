#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top down dynamic segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   IndexType: the type of the index of the array
//   LAZY: boolean to indicate whether or not range updates are enabled
//   PERSISTENT: boolean to indicate whether or not updates are persistent
//   Combine: struct to combine data and lazy values
//     Fields:
//       Data: typedef/using for the data type
//       Lazy: typedef/using for the lazy type
//       qdef: the query default value of type Data
//       ldef (only required if LAZY is true): the lazy default value of
//         type Lazy
//     Functions:
//       merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       getSegmentVal(v, k) (only required if LAZY is true): returns the lazy
//         value v when applied over a segment of length k
//       mergeLazy(l, r) (only required if LAZY is true): returns the values
//         l of type Lazy merged with r of type Lazy, must be associative
//       getSegmentVdef(k): returns the default value over a segment of
//         length k
//     Sample Struct: Combine supports point assignments and range sum queries,
//         where the default value of each index is 1, Combine2 supports 
//         range assignments and range sum queries, where the default value of
//         each index is 1
//       struct Combine {
//         using Data = int;
//         using Lazy = int;
//         const Data qdef = 0;
//         Data merge(const Data &l, const Data &r) const { return l + r; }
//         Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//         template <class IndexType> Data getSegmentVdef(IndexType k) const {
//           return k;
//         }
//       };
//       struct Combine2 {
//         using Data = int;
//         using Lazy = int;
//         const Data qdef = 0;
//         const Lazy ldef = numeric_limits<int>::min();
//         Data merge(const Data &l, const Data &r) const { return l + r; }
//         Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//         template <class IndexType>
//         Lazy getSegmentVal(const Lazy &v, IndexType k) const {
//           return v * k;
//         }
//         Lazy mergeLazy(const Lazy &l, const Lazy &r) const { return r; }
//         template <class IndexType> Data getSegmentVdef(IndexType k) const {
//           return k;
//         }
//       };
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   update(i, v, newRoot): updates the index i with the lazy value v
//     and creates a new version if newRoot is true
//   update(i, v, newRoot): updates the range [l, r] with the lazy value v
//     and creates a new version if newRoot is true
//   query(l, r, rootInd): queries the range [l, r] for the version rootInd (or
//     the latest version of rootInd is -1), and returns the aggregate value
//   revert(rootInd): creates a new version based off of version rootInd
//   reserveNodes(k): reserves space for k nodes in the dynamic segment tree
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1) for size constructor,
//                O(N) for iteartor and generating function constructors
//   update: O(log N) amortized unless reserveNodes is called beforehand
//   query: O(log N)
// Memory Complexity: O(Q log N) for Q updates for single argument constructor,
//                    O(N + Q log N) for two argument constructor 
// Tested:
//   https://dmoj.ca/problem/ccc05s5 (LAZY = false, PERSISTENT = false)
//   https://codeforces.com/contest/1080/problem/F
//     (LAZY = false, PERSISTENT = true)
//   https://codeforces.com/contest/915/problem/E
//     (LAZY = true, PERSISTENT = false)
//   https://codeforces.com/gym/101982/problem/F
//     (LAZY = true, PERSISTENT = false)
//   https://www.spoj.com/problems/TTM/ (LAZY = true, PERSISTENT = true)
template <class IndexType, const bool LAZY, const bool PERSISTENT,
          class Combine> struct DynamicSegmentTree {
#define lazy_def template <const bool _ = LAZY> typename enable_if<_>::type
#define agg_def template <const bool _ = LAZY> typename enable_if<!_>::type
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  using Data = typename Combine::Data; using Lazy = typename Combine::Lazy;
  template <const bool _, const int __ = 0> struct Node {
    Data val; int l, r; Node(const Data &val) : val(val), l(-1), r(-1) {}
  };
  template <const int __> struct Node<true, __> {
    Data val; Lazy lz; int l, r;
    Node(const Data &val) : val(val), lz(Combine().ldef), l(-1), r(-1) {}
  };
  Combine C; IndexType N; vector<int> roots; vector<Node<LAZY>> TR;
  int makeNode(int cp, IndexType tl, IndexType tr) {
    if (~cp) TR.push_back(TR[cp]);
    else TR.emplace_back(C.getSegmentVdef(tr - tl + 1));
    return int(TR.size()) - 1;
  }
  lazy_def apply(int x, IndexType tl, IndexType tr, const Lazy &v) {
    TR[x].val = C.applyLazy(TR[x].val, C.getSegmentVal(v, tr - tl + 1));
    TR[x].lz = C.mergeLazy(TR[x].lz, v);
  }
  agg_def apply(int x, IndexType, IndexType, const Lazy &v) {
    TR[x].val = C.applyLazy(TR[x].val, v);
  }
  lazy_def propagate(int x, IndexType tl, IndexType tr) {
    if (TR[x].lz != C.ldef) {
      IndexType m = tl + (tr - tl) / 2; if (!~TR[x].l || PERSISTENT) {
        int nl = makeNode(TR[x].l, tl, m); TR[x].l = nl;
      }
      if (!~TR[x].r || PERSISTENT) {
        int nr = makeNode(TR[x].r, m + 1, tr); TR[x].r = nr;
      }
      apply(TR[x].l, tl, m, TR[x].lz); apply(TR[x].r, m + 1, tr, TR[x].lz);
      TR[x].lz = C.ldef;
    }
  }
  agg_def propagate(int, IndexType, IndexType) {}
  template <class F> int build(IndexType tl, IndexType tr, F &f) {
    int x = makeNode(-1, tl, tr);
    if (tl == tr) { TR[x].val = f(); return x; }
    IndexType m = tl + (tr - tl) / 2; int nl = build(tl, m, f); TR[x].l = nl;
    int nr = build(m + 1, tr, f); TR[x].r = nr;
    TR[x].val = C.merge(TR[TR[x].l].val, TR[TR[x].r].val); return x;
  }
  int update(int y, IndexType tl, IndexType tr, IndexType l, IndexType r,
             const Lazy &v, int trSz) {
    int x = !~y || (PERSISTENT && y < trSz) ? makeNode(y, tl, tr) : y;
    if (l <= tl && tr <= r) { apply(x, tl, tr, v); return x; }
    propagate(x, tl, tr); IndexType m = tl + (tr - tl) / 2;
    if (tl <= r && l <= m) {
      int nl = update(TR[x].l, tl, m, l, r, v, trSz); TR[x].l = nl;
    }
    if (m + 1 <= r && l <= tr) {
      int nr = update(TR[x].r, m + 1, tr, l, r, v, trSz); TR[x].r = nr;
    }
    TR[x].val = C.merge(~TR[x].l ? TR[TR[x].l].val
                                 : C.getSegmentVdef(m - tl + 1),
                        ~TR[x].r ? TR[TR[x].r].val : C.getSegmentVdef(tr - m));
    return x;
  }
  Data query(int x, IndexType tl, IndexType tr, IndexType l, IndexType r) {
    if (r < tl || tr < l) return C.qdef;
    if (!~x) return C.getSegmentVdef(tr - tl + 1);
    if (l <= tl && tr <= r) return TR[x].val;
    propagate(x, tl, tr); IndexType m = tl + (tr - tl) / 2;
    return C.merge(query(TR[x].l, tl, m, l, r),
                   query(TR[x].r, m + 1, tr, l, r));
  }
  template <class F> DynamicSegmentTree(IndexType N, F f) : N(N) {
    if (N > 0) {
      reserveNodes(N * 2 - 1); roots.push_back(build(0, N - 1, f));
    }
  }
  template <class It> DynamicSegmentTree(It st, It en)
      : DynamicSegmentTree(en - st, [&] { return *st++; }) {}
  DynamicSegmentTree(IndexType N) : N(N) { roots.push_back(-1); }
  lazy_def update(IndexType l, IndexType r, const Lazy &v, bool newRoot) {
    int nr = update(roots.back(), 0, N - 1, l, r, v, TR.size());
    if (newRoot) roots.push_back(nr);
    else roots.back() = nr;
  }
  void update(IndexType i, const Lazy &v, bool newRoot) {
    int nr = update(roots.back(), 0, N - 1, i, i, v, TR.size());
    if (newRoot) roots.push_back(nr);
    else roots.back() = nr;
  }
  Data query(IndexType l, IndexType r, int rootInd = -1) {
    return query(~rootInd ? roots[rootInd] : roots.back(), 0, N - 1, l, r);
  }
  void revert(int rootInd) { roots.push_back(roots[rootInd]); }
  void reserveNodes(int k) { TR.reserve(k); }
#undef lazy_def
#undef agg_def
};
