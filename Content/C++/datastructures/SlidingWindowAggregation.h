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
  vector<T> q; T qdef, backAgg; int front, mid; Op op;
  SWAG(const T &qdef, Op op = Op())
      : qdef(qdef), backAgg(qdef), front(0), mid(0), op(op) {}
  void push(const T &v) { q.push_back(v); backAgg = op(backAgg, v); }
  T getAgg() const { return front == mid ? backAgg : op(q[front], backAgg); }
  void pop() {
    if (front++ < mid) return;
    for (int i = int(q.size()) - 2; i >= mid; i--) q[i] = op(q[i], q[i + 1]);
    mid = q.size(); backAgg = qdef;
  }
};
