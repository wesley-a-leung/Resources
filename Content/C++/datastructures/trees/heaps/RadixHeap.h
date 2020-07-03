#pragma once
#include <bits/stdc++.h>
using namespace std;

// Radix Heap
// All new keys pushed to the queue must be less than the last call to top
// top and pop return the maximum key
// Comparator convention is same as priority_queue in STL
// In practice, has a small constant
// Time Complexity:
//   top: O(B) where B is the number of bits
//   empty, size: O(1)
//   pop, push: O(1) amortized
// Memory Complexity: O(NB)
template <class Key, class Comparator = less<Key>>
struct RadixHeap {
  static_assert(is_integral<Key>::value, "Key must be integral");
  static_assert(is_unsigned<Key>::value, "Key must be unsigned");
  static constexpr int B = __lg(numeric_limits<Key>::max()) + 1;
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
        if (Comparator()(last, x[i][j])) last = x[i][j];
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
