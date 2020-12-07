#pragma once
#include <bits/stdc++.h>
using namespace std;

// Radix Heap
// Template Arguments:
//   Key: the type of the key, must be an unsigned integral type
//   Cmp: the comparator to compare two values of type Key,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Functions:
//   empty(): returns whether the heap is empty
//   size(): returns the number of elements in the heap
//   top(): returns the largest element in the heap based on the comparator
//   pop(): pops the largest element in the heap based on the comparator,
//     and returns that element
//   push(key): pushes the key onto the heap, must compare less than
//     the the last call to top
// In practice, has a small constant
// Time Complexity:
//   empty, size: O(1)
//   top: O(B) where B is the number of bits
//   pop, push: O(1) amortized
// Memory Complexity: O(NB)
template <class Key, class Cmp = less<Key>>
struct RadixHeap {
  static_assert(is_integral<Key>::value, "Key must be integral");
  static_assert(is_unsigned<Key>::value, "Key must be unsigned");
  static constexpr const int B = __lg(numeric_limits<Key>::max()) + 1;
  int N; Key last; vector<vector<Key>> x;
  RadixHeap() : N(0), last(0), x(B + 1) {}
  int lg(Key a) const { return a ? __lg(a) : -1; }
  void aux(Key k) { x[lg(k ^ last) + 1].push_back(k); }
  bool empty() const { return N == 0; }
  int size() const { return N; }
  Key top() {
    if (x[0].empty()) {
      int i = 0; while (x[i].empty()) i++;
      last = x[i][0]; for (int j = 1; j < int(x[i].size()); j++)
        if (Cmp()(last, x[i][j])) last = x[i][j];
      for (auto &&p : x[i]) aux(p);
      x[i].clear();
    }
    return x[0].back();
  }
  Key pop() {
    Key ret = top(); N--; x[0].pop_back(); return ret;
  }
  void push(Key key) { N++; aux(key); }
};
