#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports online queries and FIFO operations on a multiset of elements,
//   where the underlying data structure only supports LIFO operations
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
//     S.push and S.pop, with S.push and S.pop being called at most O(log N)
//     times in total for each element, over all operations
//   query: O(T) where T is the time complexity of S.query
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1386/problem/C
template <class S> struct FIFOWithLIFO : public S {
  using T = typename S::T; vector<pair<T, bool>> stk; vector<T> A, B; int cntA;
  template <class ...Args>
  FIFOWithLIFO(Args &&...args) : S(forward<Args>(args)...), cntA(0) {}
  void push(const T &v, bool a = false) { stk.emplace_back(v, a); S::push(v); }
  void pop() {
    if (cntA == 0) {
      reverse(stk.begin(), stk.end()); cntA = stk.size();
      for (int i = 0; i < cntA; i++) S::pop();
      for (auto &&s : stk) { S::push(s.first); s.second = true; }
    }
    for (; !stk.back().second; stk.pop_back(), S::pop())
      B.push_back(stk.back().first);
    int m = cntA & -cntA; for (int i = 0; i < m; i++, stk.pop_back(), S::pop())
      A.push_back(stk.back().first);
    for (; !B.empty(); B.pop_back()) push(B.back());
    for (; !A.empty(); A.pop_back()) push(A.back(), true);
    stk.pop_back(); S::pop(); cntA--;
  }
};
