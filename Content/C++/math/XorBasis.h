#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the basis of vectors in base 2
// Template Arguments:
//   BITS: the number of bits
// Fields:
//   basis: a vector of bitsets of length BITS, with the ith element being the
//     reduced vector with the most significant bit of i, or empty if there is
//     no such vector
//   ids: a vector of integers representing the id of the ith vector in basis,
//     or -1 if basis[i] is empty
// Functions:
//   addVector(v, id): adds the vector v to the set with and id of id
//   inSpan(v): returns whether a vector v is in the span of the basis
// In practice, has a small constant
// Time Complexity:
//   constructor, addVector, inSpan: O(BITS^2 / 64)
// Memory Complexity: O(BITS^2 / 64)
// Tested:
//   https://dmoj.ca/problem/dmopc19c5p5
template <const int BITS> struct XorBasis {
  vector<bitset<BITS>> basis; vector<int> ids;
  XorBasis() : basis(BITS), ids(BITS, -1) {}
  void addVector(bitset<BITS> v, int id) {
    for (int i = 0; i < BITS; i++) if (v[i]) {
      if (basis[i].none()) { basis[i] = v; ids[i] = id; return; }
      v ^= basis[i];
    }
  }
  bool inSpan(bitset<BITS> v) {
    for (int i = 0; i < BITS; i++) if (v[i]) v ^= basis[i];
    return v.none();
  }
};
