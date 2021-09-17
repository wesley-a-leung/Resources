#pragma once
#include <bits/stdc++.h>
using namespace std;

// Uses divide and conquer to answer offline ranges queries over
//   a multiset where elements can be added or removed at anytime, but the
//   underlying data structure is only able to add and delete elements in LIFO
//   order, as well as save and restore its state
// Template Arguments:
//   S: struct to maintain a multiset of elements
//   Required Fields:
//     T: the type of each element
//     R: the type of the return value for each query
//     Q: the query object that contains information for each query
//   Required Functions:
//     constructor(...args): takes any number of arguments (arguments are
//       passed from LIFOSetDivAndConq::solveQueries)
//     push(v): adds the value v to the multiset
//     pop(): pops the most recent element pushed to the multiset
//     query(q): returns the answer of type R for the query q of type Q to
//       the multiset
//   Sample Struct: supports queries for the sum of element in the same
//       connected component as a vertex in a graph, where edges are
//       added and removed
//     struct S {
//       using T = pair<int, int>; using R = long long;
//       struct Q { int v; };
//       WeightedUnionFindUndo<R, plus<R>> uf;
//       S(const vector<R> &W) : uf(W) {}
//       void push(const T &e) { uf.join(e.first, e.second); }
//       void pop() { uf.undo(); }
//       R query(const Q &q) { return uf.getWeight(q.v); }
//     };
// Fields:
//   ans: a vector of integers with the answer for each query
// Functions:
//   addElement(v): adds v to the multiset
//   removeElement(v): removes v from the multiset
//   addQuery(q): adds a query of type S::Q
//   solveQueries(...args): solves all queries asked so far, with
//     ...args being passed to the constructor of S
// In practice, has a small constant
// Time Complexity:
//   addElement, removeElement: O(1) amortized
//   solveQueries: O(C + P E log E + KT)
//     for K queries and E total elements where C is the time complexity
//     of S's constructor, P is the time complexity of S.push and S.pop,
//     T is the time complexity of S.query, with S.push and S.pop being called
//     at most O(log K) times in total for each element, over all operations
// Memory Complexity: O(K + E + M) for K queries and E total elements, where
//   M is the memory complexity of S
// Tested:
//   https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum
template <class S> struct LIFOSetDivAndConq {
  using T = typename S::T; using R = typename S::R; using Q = typename S::Q;
  vector<T> add, rem; vector<Q> queries; vector<R> ans; vector<char> type;
  void addElement(const T &v) { add.push_back(v); type.push_back(1); }
  void removeElement(const T &v) { rem.push_back(v); type.push_back(-1); }
  void addQuery(const Q &q) { queries.push_back(q); type.push_back(0); }
  template <class ...Args> void solveQueries(Args &&...args) {
    int E = type.size(); ans.clear(); ans.reserve(queries.size());
    vector<T> A = add; sort(A.begin(), A.end());
    vector<pair<int, int>> events; events.reserve(E);
    vector<int> last(A.size(), INT_MAX); S s(forward<Args>(args)...);
    for (int i = 0, addInd = 0, remInd = 0, queryInd = 0; i < E; i++) {
      if (type[i] == 1) {
        int j = lower_bound(A.begin(), A.end(), add[addInd++]) - A.begin();
        events.emplace_back(j, last[j]); last[j] = i;
      } else if (type[i] == -1) {
        int j = lower_bound(A.begin(), A.end(), rem[remInd++]) - A.begin();
        events.emplace_back(j, last[j]); int temp = events[last[j]].second;
        events[last[j]].second = i; last[j] = temp;
      } else events.emplace_back(queryInd++, i);
    }
    function<void(int, int)> dc = [&] (int l, int r) {
      if (l == r) {
        if (events[l].second == l)
          ans.push_back(s.query(queries[events[l].first]));
        return;
      }
      int m = l + (r - l) / 2; int cnt = 0; for (int i = m + 1; i <= r; i++)
        if (events[i].second < l) { s.push(A[events[i].first]); cnt++; }
      dc(l, m); for (int i = 0; i < cnt; i++) s.pop();
      cnt = 0; for (int i = l; i <= m; i++)
        if (events[i].second > r) { s.push(A[events[i].first]); cnt++; }
      dc(m + 1, r); for (int i = 0; i < cnt; i++) s.pop();
    };
    if (E > 0) dc(0, E - 1);
  }
};
