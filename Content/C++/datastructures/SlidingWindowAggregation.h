#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the aggregate value of a sequence of elements over an associative
//   operation where elements can be pushed to the back and popped from the
//   front of the queue
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the operation (can also be of type
//       std::function<T(T, T)>); in practice, custom struct is faster than
///      std::function
//     Required Functions:
//       operator (l, r): merges the values l and r, must be associative
// Constructor Arguments:
//   qdef: the query default value
//   op: an instance of the Op struct
// Functions:
//   push(v): pushes the value v into the queue
//   getAgg(): returns the aggregate value of the elements in the queue
//     aggregated in the order they were pushed
//   pop(): pops from the front of the queue
// In practice, has a moderate constant
// Time Complexity:
//   constructor, getAgg: O(1)
//   push, pop: O(1) amortized
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/queue_operate_all_composite
template <class T, class Op> struct SWAG {
  vector<T> A, B; T qdef, bAgg; Op op;
  SWAG(const T &qdef, Op op = Op()) : qdef(qdef), bAgg(qdef), op(op) {}
  void push(const T &v) { B.push_back(v); bAgg = op(bAgg, v); }
  T getAgg() const { return A.empty() ? bAgg : op(A.back(), bAgg); }
  void pop() {
    if (A.empty()) for (T aAgg = bAgg = qdef; !B.empty(); B.pop_back())
      A.push_back(aAgg = op(B.back(), aAgg));
    A.pop_back();
  }
};
