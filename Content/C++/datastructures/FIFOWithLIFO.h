#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports FIFO operations on a multiset of elements with queries to the
//  data structure, where the underlying data structure only supports LIFO
//  operations
// Template Arguments:
//   S: struct to maintain a multiset of elements
//   Required Fields:
//     T: the type of each element
//   Required Functions:
//     constructor(...args): takes any number of arguments (arguments are
//       passed from constructor of FIFOWithLIFO)
//     push(v): pushes the value v into the multiset
//     pop(): pops the most recent element pushed to the multiset
//     query(...args): queries the multiset
//   Sample Struct: supports queries for graph connectivity
//     struct S {
//       using T = pair<int, int>;
//       UnionFindUndo uf;
//       S(int V) : uf(V) {}
//       void push(const T &e) { uf.join(e.first, e.second); }
//       void pop() { uf.undo(); }
//       bool query(int v, int w) { return uf.connected(v, w); }
//     };
// Constructor Arguments:
//   ...args: arguments to be forwarded to the constructor of S
// Functions:
//   push(v): pushes the value v into the multiset
//   pop(): pops the least recent element pushed into the multiset
//   query(...args): inheirited from S, queries the multiset
// In practice, has a small constant
// Time Complexity:
//   push: O(P) where P is the time complexity of S.push
//   pop: O(P log N) amortized where P is the time complexity of
//     S.push and S.pop
//   query: O(T) where T is the time complexity of S.query
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1386/problem/C
template <class S> struct FIFOWithLIFO : public S {
  using T = typename S::T; using S::query; vector<pair<T, bool>> stk;
  template <class ...Args>
  FIFOWithLIFO(Args &&...args) : S(forward<Args>(args)...) {}
  void push(const T &v) { stk.emplace_back(v, false); S::push(v); }
  void pop() {
    if (!stk.back().second) {
      vector<T> A, B{stk.back().first}; stk.pop_back(); S::pop();
      for (; !stk.empty() && A.size() != B.size(); stk.pop_back(), S::pop())
        (stk.back().second ? A : B).push_back(stk.back().first);
      reverse(A.begin(), A.end()); if (!A.empty()) reverse(B.begin(), B.end());
      for (auto &&b : B) { stk.emplace_back(b, A.empty()); S::push(b); }
      for (auto &&a : A) { stk.emplace_back(a, true); S::push(a); }
    }
    stk.pop_back(); S::pop();
  }
};
